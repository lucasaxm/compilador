%{
int yylex();
void yyerror(const char *s);
%}

%{
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "compilador.h"
#include "pilha.h"
#include "fila.h"

FILE* fp=NULL;

void geraCodigo (char* rot, char* comando) {
  
  if (fp == NULL) {
    fp = fopen ("MEPA", "w");
  }
  
  int comando_len = 0;
  int params_len = 0;
  char *param;
  int primeiro=1; // flag pra saber se estou no_fila parametro nos loops
  
  no_fila n = inicio(parametros);
  while (n) { // calcula tamanho da string de params sem \0 no_fila final.
    param = (char *) conteudo_fila(n);
    if (primeiro) {
      params_len+=strlen(param);
      primeiro=0;
    }
    else {
      params_len+=strlen(", ")+strlen(param);
    }
    n = proximo_no_fila(n);
  }
  
  if (params_len > 0)
    comando_len+=strlen(comando)+strlen(" ")+params_len;
  else
    comando_len+=strlen(comando);
  
  
  char comando_completo[comando_len+1];
  // char params[params_len+1];
  
  snprintf(comando_completo, comando_len+1, "%s", comando);
  
//   debug_print("%s\n", "Inicio - adiciona parametros.");
  primeiro=1;
  n = inicio(parametros);
  while (n) { // calcula tamanho da string de params sem \0 no_fila final.
    param = (char *) desenfileira(parametros);
    if (primeiro) {
        strncat(comando_completo, " ", comando_len+1);
        strncat(comando_completo, param, comando_len+1);
        // debug_print("comando_completo=[%s]\n", comando_completo);
        primeiro=0;
    }
    else {
        strncat(comando_completo, ", ", comando_len+1);
        strncat(comando_completo, param, comando_len+1);
        // debug_print("comando_completo=[%s]\n", comando_completo);
    }
    free(param);
    n = inicio(parametros);
  }
//   debug_print("%s\n", "Fim - adiciona parametros.");
  
  if ( rot ) {
    debug_print ("rot+comando_completo:[%s: %s]\n", rot, comando_completo);
    fprintf(fp, "%s: %s \n", rot, comando_completo); fflush(fp);
  } else {
    debug_print ("comando_completo=[%s]\n", comando_completo);
    fprintf(fp, "     %s\n", comando_completo); fflush(fp);
  }
  TS_imprime(tabela_simbolos);
}

int n_digitos(int n){
    if (n==0)
        return 1;
    else if (n<0)
        return floor(log10(abs(n))) + 2;
    else
        return floor(log10(abs(n))) + 1;
}

void erro(erros e){
    int msg_size=100;
    char msg[msg_size];
    switch (e){
        case ERRO_ATRIB:
            snprintf(msg, msg_size, "Linha %d | Recipiente inválido para atribuição.", nl);
            break;
        case ERRO_TPARAM:
            snprintf(msg, msg_size, "Linha %d | Tipo do parâmetro não compatível com assinatura.", nl);
            break;
        case ERRO_NPARAM:
            snprintf(msg, msg_size, "Linha %d | Número de parâmetros não coincide com assinatura.", nl);
            break;
        case ERRO_IDENT_DECL:
            snprintf(msg, msg_size, "Linha %d | Identificador já declarado.", nl);
            break;
        case ERRO_VS_NDECL:
            snprintf(msg, msg_size, "Linha %d | Variável '%s' não declarada.", nl, token); 
            break;
        case ERRO_PROC_NDECL:
            snprintf(msg, msg_size, "Linha %d | Procedimento não declarado.", nl);
            break;
        case ERRO_FUNC_NDECL:
            snprintf(msg, msg_size, "Linha %d | Função não declarada.", nl);
            break;
        case ERRO_ROT_NDECL:
            snprintf(msg, msg_size, "Linha %d | Rótulo não declarado.", nl);
            break;
        case ERRO_TINCOMPATIVEL:
            snprintf(msg, msg_size, "Linha %d | Operação com tipos incompatíveis.", nl);
            break;
        case ERRO_PARAMREF:
            snprintf(msg, msg_size, "Linha %d | Parametro passado por referencia deve ser uma variavel.", nl);
            break;
        case ERRO_DESCONHECIDO:
            snprintf(msg, msg_size, "Linha %d | Erro desconhecido.", nl);
            break;
        case ERRO_IDENT_DUPLICADO:
            snprintf(msg, msg_size, "Linha %d | Identificador duplicado.", nl);
            break;
        default:
            snprintf(msg, msg_size, "Linha %d | Erro desconhecido.", nl);
            break;
    }
    fprintf(stderr,"\n# %s\n# Abortando.\n\n",msg);
    exit(e);
    return;
}

void enfileira_param_int(int param_int){
    int param_len = n_digitos(param_int)+1;
    char *param = (char *) malloc( sizeof(char)*(param_len+1) );
    snprintf(param, param_len, "%d", param_int);
    enfileira( (void *) param, parametros);
}

void enfileira_param_string(char *param_string){
    int param_len = strlen(param_string);
    char *param = (char *) malloc( sizeof(char)*(param_len+1) );
    strncpy(param, param_string, param_len+1);
    enfileira( (void *) param, parametros);
}

void valida_tipos_binario(tipos tipo1, tipos tipo2, tipos tipo_esperado){
    debug_print("tipo1=[%d] tipo2=[%d] tipo_esperado=[%d]", tipo1, tipo2, tipo_esperado);
    if ( (tipo1 != tipo2) || (tipo1 != tipo_esperado) ){
        erro(ERRO_TINCOMPATIVEL);
    }
}

void valida_tipos_unario(tipos tipo, tipos tipo_esperado){
    debug_print("tipo=[%d] tipo_esperado=[%d]", tipo, tipo_esperado);
    if ( tipo != tipo_esperado ){
        erro(ERRO_TINCOMPATIVEL);
    }
}

char *prox_rotulo(){
    char *rot = (char *) malloc( sizeof(char)*(TAM_ROT+1));
    snprintf(rot, TAM_ROT+1, "R%.2d", d_rot++);
    return rot;
}

void aloca_mem(){
    if(num_vars <= 0)
        return;
    
    enfileira_param_int(num_vars);
    
    debug_print("%s\n","parametros:");
    imprime_fila(parametros, NULL);
    
    num_vars=0; // zera contador de variáveis
    
    geraCodigo (NULL, "AMEM");
    
}

void desaloca_mem(){
    if(TS_tamanho(tabela_simbolos) <= 0)
        return;
    
    int removidos = TS_remove_vs(nivel_lexico, tabela_simbolos);
    if (removidos==0) return;
    
    enfileira_param_int(removidos);
    
    debug_print("%s\n","parametros:");
    imprime_fila(parametros, NULL);
    
    num_vars=0; // zera contador de variáveis
    
    geraCodigo (NULL, "DMEM");
    
}

/* Decide tipo de armazenamento */
void armazena(){
    char *s_str = TS_simbolo2str(aux_atrib.s);
    debug_print("%s\n",s_str);
    free (s_str);
    switch (aux_atrib.s->base.categoria){
        case CAT_FUNC:
            enfileira_param_int(aux_atrib.s->func.nivel_lexico);
            
            enfileira_param_int(aux_atrib.s->func.deslocamento);
            
            imprime_fila(parametros, NULL);
            
            geraCodigo(NULL, "ARMZ");
            
            break;
        case CAT_PF:
            enfileira_param_int(aux_atrib.s->pf.nivel_lexico);
            
            enfileira_param_int(aux_atrib.s->pf.deslocamento);
            
            if (aux_atrib.s->pf.passagem == PASS_VAL){
                geraCodigo(NULL, "ARMZ");
            }else{
                geraCodigo(NULL, "ARMI");
            }
            
            break;
        case CAT_VS:
            enfileira_param_int(aux_atrib.s->vs.nivel_lexico);
            
            enfileira_param_int(aux_atrib.s->vs.deslocamento);
            
            geraCodigo(NULL, "ARMZ");
            
            break;
        default:
            debug_print("%s","Erro. categoria invalida.\n");
            break;
    }
}

/* Decide tipo de carrega */
void carrega(tipo_simbolo *simb){
    char *s_str = TS_simbolo2str(simb);
    debug_print("%s\n",s_str);
    free (s_str);
    param *p;
    tipos_passagem pass;
    if (tamanho_pilha(pilha_cham_subrot)>0){
        tipo_simbolo *subrot = conteudo_pilha(topo(pilha_cham_subrot));
        if (!subrot)
            erro(ERRO_FUNC_NDECL);
        if (subrot->base.categoria == CAT_PROC)
            p = busca_indice_pilha(num_params_subrot, subrot->proc.params);
        else
            p = busca_indice_pilha(num_params_subrot, subrot->func.params);
            
        if (!p) {
            erro(ERRO_NPARAM);
        }
        
        pass = p->passagem;
        debug_print("Passagem do param por %s\n", pass == PASS_VAL ? "valor" : "referencia");
    }
    else {
        pass = PASS_VAL;
    }
    switch (simb->base.categoria){
        case CAT_PF:
            enfileira_param_int(simb->pf.nivel_lexico);
            enfileira_param_int(simb->pf.deslocamento);
            if (simb->pf.passagem == PASS_VAL){ // PF VLR
                debug_print("%s\n","Simbolo eh PF VLR");
                if (pass == PASS_VAL)
                    geraCodigo(NULL, "CRVL");
                else if (pass == PASS_REF)
                    geraCodigo(NULL, "CREN");
            }
            else if (simb->pf.passagem == PASS_REF){ // PF REF
                debug_print("%s\n","Simbolo eh PF REF");
                if (pass == PASS_VAL)
                    geraCodigo(NULL, "CRVI");
                else if (pass == PASS_REF)
                    geraCodigo(NULL, "CRVL");
            }
            break;
        case CAT_VS:
            debug_print("%s\n","Simbolo eh VS");
            enfileira_param_int(simb->vs.nivel_lexico);
            enfileira_param_int(simb->vs.deslocamento);
            if (pass == PASS_VAL)
                geraCodigo(NULL, "CRVL");
            else if (pass == PASS_REF)
                geraCodigo(NULL, "CREN");
            break;
        case CAT_FUNC: // func sem ()
            debug_print("%s\n","Simbolo eh FUNC");
            if (pass == PASS_REF)
                erro(ERRO_PARAMREF);
            if(simb->func.n_params > 0)
                erro(ERRO_NPARAM);
            enfileira_param_int(1);
            geraCodigo(NULL, "AMEM"); // aloca espaco para ret da func
            enfileira_param_string(simb->func.rotulo);
            enfileira_param_int(nivel_lexico);
            geraCodigo(NULL, "CHPR");
            break;
        default:
            s_str = TS_simbolo2str(simb);
            debug_print("Simbolo com erro: [%s]\n", s_str);
            free(s_str);
            erro(ERRO_TPARAM);
            break;
    }
}

%}


%union {
    int tipo;
}


%token PROGRAM ABRE_PARENTESES FECHA_PARENTESES VIRGULA PONTO_E_VIRGULA DOIS_PONTOS PONTO T_BEGIN
%token T_END VAR IDENT ATRIBUICAO LABEL PROCEDURE FUNCTION GOTO IF THEN ELSE WHILE INT BOOL NUMERO
%token MULT MAIS MENOS MAIOR MENOR MAIOR_IGUAL MENOR_IGUAL DIFERENTE IGUAL OR AND NOT DIV T_TRUE
%token T_FALSE

%type<tipo> fator termo expressao_simples expressao

%define parse.error verbose

%%

/* FLUXO PRINCIPAL */

programa :
    {
        num_vars=0;
        nivel_lexico=0;
        d_rot=0;
        tabela_simbolos = constroi_pilha();
        pilha_rotulos_dsvs = constroi_pilha();
        pilha_decl_subrot = constroi_pilha();
        pilha_cham_subrot = constroi_pilha();
        parametros = constroi_fila();
        geraCodigo (NULL, "INPP");
    }
    PROGRAM IDENT 
    ABRE_PARENTESES lista_idents FECHA_PARENTESES PONTO_E_VIRGULA
    bloco PONTO
    {
        geraCodigo (NULL, "PARA");
    }
;

bloco : 
    parte_declara_vars
    {
        aloca_mem();
        
        char *rotulo_dsvs = prox_rotulo();
        empilha( (void *) rotulo_dsvs, pilha_rotulos_dsvs);
        
        enfileira_param_string(rotulo_dsvs); 
        
        geraCodigo (NULL, "DSVS");
    }
    parte_declara_subrotinas
    {
        char *rot_dsvs_aux = desempilha(pilha_rotulos_dsvs);
        geraCodigo(rot_dsvs_aux, "NADA");
        free(rot_dsvs_aux);
    }
    comando_composto 
    {
        desaloca_mem();
    }
;

lista_idents: lista_idents VIRGULA IDENT  
            | IDENT
;


// lista_vars:
//     lista_vars VIRGULA var
//     | var
// ;

var:
    IDENT
;

/* DECLARACAO DE SUBROTINAS */

parte_declara_subrotinas:
    parte_declara_subrotinas declara_procedimento PONTO_E_VIRGULA
    | parte_declara_subrotinas declara_funcao PONTO_E_VIRGULA
    | %empty
;

declara_procedimento:
    PROCEDURE IDENT
    {
        debug_print("Iniciando declaracao do procedimento '%s'\n", token);
        s = (tipo_simbolo *) malloc(sizeof(tipo_simbolo));
        s->proc = TS_constroi_simbolo_proc(token, ++nivel_lexico, prox_rotulo(), 0, NULL);
        TS_empilha(s, tabela_simbolos);
        empilha(s, pilha_decl_subrot);
        enfileira_param_int(nivel_lexico);
        geraCodigo(s->proc.rotulo, "ENPR");
        num_params_subrot=0;
    }
    params_formais PONTO_E_VIRGULA
    {
        TS_atualiza_params(num_params_subrot, tabela_simbolos);
    }
    bloco
    {
        tipo_simbolo *subrot = desempilha(pilha_decl_subrot);
        
        if (!subrot)
            erro(ERRO_FUNC_NDECL);
            
        empilha(subrot, pilha_decl_subrot);
        enfileira_param_int(nivel_lexico);
        
        int num_params;
        
        if (subrot->base.categoria == CAT_PROC){
            enfileira_param_int(num_params = subrot->proc.n_params);
        }
        else
            enfileira_param_int(num_params = subrot->func.n_params);
        geraCodigo(NULL, "RTPR");
        TS_remove_rtpr(desempilha(pilha_decl_subrot), tabela_simbolos);
        nivel_lexico--;
    }
;

params_formais:
    ABRE_PARENTESES declara_params FECHA_PARENTESES
    | ABRE_PARENTESES FECHA_PARENTESES
;

declara_params: declara_params PONTO_E_VIRGULA declara_param 
            | declara_param 
;

declara_param :
    VAR
    {
        aux_passagem = PASS_REF;
    }
    lista_id_param DOIS_PONTOS
    {
        aux_categoria = CAT_PF;
    }
    tipo
    |
    {
        aux_passagem = PASS_VAL;
    }
    lista_id_param DOIS_PONTOS
    {
        aux_categoria = CAT_PF;
    }
    tipo
    
;

lista_id_param:
    lista_id_param VIRGULA id_param 
    | id_param
;

id_param:
    IDENT
    { /* insere params na tabela de símbolos */
        s = (tipo_simbolo *) malloc(sizeof(tipo_simbolo));
        s->pf = TS_constroi_simbolo_pf(token, nivel_lexico, 0, TIPO_UNKNOWN, aux_passagem); // atualiza deslocamento e tipo depois
        TS_empilha(s, tabela_simbolos);
        conta_tipo++;
        num_params_subrot++;
    }
;

declara_funcao:
    FUNCTION IDENT
    {
        debug_print("Iniciando declaracao da funcao '%s'\n", token);
        s = (tipo_simbolo *) malloc(sizeof(tipo_simbolo));
        s->func = TS_constroi_simbolo_func(token, ++nivel_lexico, 0, TIPO_UNKNOWN, prox_rotulo(), 0, NULL);
        TS_empilha(s, tabela_simbolos);
        empilha(s, pilha_decl_subrot);
        enfileira_param_int(nivel_lexico);
        geraCodigo(s->func.rotulo, "ENPR");
        num_params_subrot=0;
    }
    params_formais DOIS_PONTOS 
    {
        TS_atualiza_params(num_params_subrot, tabela_simbolos);
        aux_categoria = CAT_FUNC;
    }
    tipo PONTO_E_VIRGULA bloco
    {
        tipo_simbolo *subrot = desempilha(pilha_decl_subrot);
        
        if (!subrot)
            erro(ERRO_FUNC_NDECL);
            
        empilha(subrot, pilha_decl_subrot);
        enfileira_param_int(nivel_lexico);
        
        int num_params;
        
        if (subrot->base.categoria == CAT_PROC){
            enfileira_param_int(num_params = subrot->proc.n_params);
        }
        else
            enfileira_param_int(num_params = subrot->func.n_params);
        geraCodigo(NULL, "RTPR");
        TS_remove_rtpr(desempilha(pilha_decl_subrot), tabela_simbolos);
        nivel_lexico--;
    }
;

/* DECLARACAO DE VARS */

parte_declara_vars:
    VAR
    declara_vars
    | %empty
;

declara_vars: declara_vars declara_var 
            | declara_var 
;

declara_var :
    lista_id_var DOIS_PONTOS
    {
        aux_categoria = CAT_VS;
    }
    tipo PONTO_E_VIRGULA
;

tipo :
    BOOL
    {
        TS_atualiza_tipos(TIPO_BOOL, aux_categoria, tabela_simbolos);
    }
    | INT
    {
        TS_atualiza_tipos(TIPO_INT, aux_categoria, tabela_simbolos);
    }
;

lista_id_var:
    lista_id_var VIRGULA id_var 
    | id_var
;

id_var:
    IDENT
    { /* insere vars na tabela de símbolos */
        s = (tipo_simbolo *) malloc(sizeof(tipo_simbolo));
        s->vs = TS_constroi_simbolo_vs(token, nivel_lexico, num_vars++, TIPO_UNKNOWN);
        TS_empilha(s, tabela_simbolos);
        conta_tipo++;
    }
;

/* COMANDOS */

comando_composto: 
    T_BEGIN
    {
        debug_print("%s\n", "begin comando_composto.");
    }
    comandos
    T_END
    {
        debug_print("%s\n", "end comando_composto.");
    }
;

comandos:
    comandos PONTO_E_VIRGULA comando
    | comando
;

comando:
    LABEL
    {
        has_label=1;
    }
    comando_sem_rotulo
    {
        has_label=0;
    }
    | comando_sem_rotulo
    | %empty
;

comando_sem_rotulo:
    IDENT
    {
        strncpy(ident, token, TAM_TOKEN);
    }
    comando_sem_rotulo2
;

comando_sem_rotulo2:
    atrib
    | ch_proc
;

ch_proc:
    {
        num_params_subrot=0;
        tipo_simbolo *subrot = TS_busca_procedimento(ident, tabela_simbolos);
        if (!subrot)
            erro(ERRO_PROC_NDECL);
        empilha(subrot, pilha_cham_subrot);
        char *subrot_srt = TS_simbolo2str(subrot);
        debug_print("Chamando subrotina [%s]\n", subrot_srt);
        free (subrot_srt);
    }
    passa_params
    {
        tipo_simbolo *subrot = desempilha(pilha_cham_subrot);
        if(num_params_subrot < subrot->proc.n_params)
            erro(ERRO_NPARAM);
        enfileira_param_string(subrot->proc.rotulo);
        enfileira_param_int(nivel_lexico);
        geraCodigo(NULL, "CHPR");
    }
;

passa_params:
    ABRE_PARENTESES lista_params FECHA_PARENTESES
    | ABRE_PARENTESES FECHA_PARENTESES
    | %empty
;

lista_params:
    lista_params
    VIRGULA
    {
        flag_var=0;
    }
    param
    | param
;

param:
    expressao
    {
        tipo_simbolo *subrot = conteudo_pilha(topo(pilha_cham_subrot));
        if(num_params_subrot > subrot->proc.n_params)
            erro(ERRO_NPARAM);
        param *p = busca_indice_pilha(num_params_subrot, subrot->proc.params);
        if ( (p->passagem == PASS_REF) && (!flag_var) ) { // se eh pass por ref e encontrou operacao
            erro(ERRO_PARAMREF);
        }
        if ( $1 != p->tipo ) { // se tipo do parametro != tipo da operacao
            erro(ERRO_TPARAM);
        }
        num_params_subrot++;
    }
;

atrib:
    {
        debug_print("begin atribuicao. ident=[%s]\n", ident);
        aux_atrib.s = TS_busca(ident, tabela_simbolos);
        if (aux_atrib.s == NULL) {
            erro(ERRO_VS_NDECL);
        }
        switch (aux_atrib.s->base.categoria){
            case CAT_PF:
                aux_atrib.tipo = aux_atrib.s->pf.tipo;
                break;
            case CAT_VS:
                aux_atrib.tipo = aux_atrib.s->vs.tipo;
                break;
            default:
                erro(ERRO_ATRIB);
                break;
        }
    }
    ATRIBUICAO expressao
    {
        char *aux_atrib_simb_str = TS_simbolo2str(aux_atrib.s);
        debug_print ("Regra: %s | tipo_ident=[%d] tipo_expressao=[%d]\n","atrib", aux_atrib.tipo, $3);
        debug_print ("Simbolo recipiente: %s\n", aux_atrib_simb_str);
        free (aux_atrib_simb_str);
        if ( $3 != aux_atrib.tipo ) {
            erro(ERRO_ATRIB);
        }
        armazena();
        debug_print("end atribuicao. token=[%s]\n", token);
    }
;

/* EXPRESSOES */

expressao:
    expressao_simples
    {
        // debug_print ("Regra: %s | %s | tipo_expsimp=[%d]\n","expressao","EXP_SIMP", $1);
        $$ = $1;
    }
    | expressao_simples IGUAL expressao_simples
    {
        // debug_print ("Regra: %s | %s | tipo_expsimp1=[%d] tipo_expsimp2=[%d]\n","expressao","IGUAL", $1, $3);
        if ($1 == $3){
            $$ = TIPO_BOOL;
        } else {
            erro(ERRO_TINCOMPATIVEL);
        }
        geraCodigo(NULL, "CMIG");
        flag_var=0;
    }
    | expressao_simples DIFERENTE expressao_simples
    {
        // debug_print ("Regra: %s | %s | tipo_expsimp1=[%d] tipo_expsimp2=[%d]\n","expressao","DIFERENTE", $1, $3);
        if ($1 == $3){
            $$ = TIPO_BOOL;
        } else {
            erro(ERRO_TINCOMPATIVEL);
        }
        geraCodigo(NULL, "CMDG");
        flag_var=0;
    }
    | expressao_simples MENOR expressao_simples
    {
        // debug_print ("Regra: %s | %s | tipo_expsimp1=[%d] tipo_expsimp2=[%d]\n","expressao","MENOR", $1, $3);
        if ( ($1 == $3) && ($1 == TIPO_INT) ){
            $$ = TIPO_BOOL;
        } else {
            erro(ERRO_TINCOMPATIVEL);
        }
        geraCodigo(NULL, "CMME");
        flag_var=0;
    }
    | expressao_simples MENOR_IGUAL expressao_simples
    {
        // debug_print ("Regra: %s | %s | tipo_expsimp1=[%d] tipo_expsimp2=[%d]\n","expressao","MENOR_IGUAL", $1, $3);
        if ( ($1 == $3) && ($1 == TIPO_INT) ){
            $$ = TIPO_BOOL;
        } else {
            erro(ERRO_TINCOMPATIVEL);
        }
        geraCodigo(NULL, "CMEG");
        flag_var=0;
    }
    | expressao_simples MAIOR_IGUAL expressao_simples
    {
        // debug_print ("Regra: %s | %s | tipo_expsimp1=[%d] tipo_expsimp2=[%d]\n","expressao","MAIOR_IGUAL", $1, $3);
        if ( ($1 == $3) && ($1 == TIPO_INT) ){
            $$ = TIPO_BOOL;
        } else {
            erro(ERRO_TINCOMPATIVEL);
        }
        geraCodigo(NULL, "CMAG");
        flag_var=0;
    }
    | expressao_simples MAIOR expressao_simples
    {
        // debug_print ("Regra: %s | %s | tipo_expsimp1=[%d] tipo_expsimp2=[%d]\n","expressao","MAIOR", $1, $3);
        if ( ($1 == $3) && ($1 == TIPO_INT) ){
            $$ = TIPO_BOOL;
        } else {
            erro(ERRO_TINCOMPATIVEL);
        }
        geraCodigo(NULL, "CMMA");
        flag_var=0;
    }
;

expressao_simples:
    MAIS termo
    {
        // debug_print ("Regra: %s | %s | tipo_termo=[%d]\n","expressao_simples","TERMO POS", $2);
        if ( ($2 == TIPO_INT) ){
            $$ = $2;
        } else {
            erro(ERRO_TINCOMPATIVEL);
        }
    }
    | MENOS termo
    {
        // debug_print ("Regra: %s | %s | tipo_termo=[%d]\n","expressao_simples","TERMO NEG", $2);
        if ( ($2 == TIPO_INT) ){
            $$ = $2;
        } else {
            erro(ERRO_TINCOMPATIVEL);
        }
        // TODO: PENSAR NO GERACODIGO
        flag_var=0;
    }
    | termo
    {
        // debug_print ("Regra: %s | %s | tipo_termo=[%d]\n","expressao_simples","TERMO", $1);
        $$ = $1;
    }
    | expressao_simples MAIS termo
    {
        // debug_print ("Regra: %s | %s | tipo_expsimp=[%d] tipo_termo=[%d]\n","expressao_simples","SOMA", $1, $3);
        if ( ($1 == $3) && ($1 == TIPO_INT) ){
            $$ = $1;
        } else {
            erro(ERRO_TINCOMPATIVEL);
        }
        geraCodigo(NULL, "SOMA");
        flag_var=0;
    }
    | expressao_simples MENOS termo
    {
        // debug_print ("Regra: %s | %s | tipo_expsimp=[%d] tipo_termo=[%d]\n","expressao_simples","SUBT", $1, $3);
        if ( ($1 == $3) && ($1 == TIPO_INT) ){
            $$ = $1;
        } else {
            erro(ERRO_TINCOMPATIVEL);
        }
        geraCodigo(NULL, "SUBT");
        flag_var=0;
    }
    | expressao_simples OR termo
    {
        // debug_print ("Regra: %s | %s | tipo_expsimp=[%d] tipo_termo=[%d]\n","expressao_simples","OR", $1, $3);
        if ( ($1 == $3) && ($1 == TIPO_BOOL) ){
            $$ = $1;
        } else {
            erro(ERRO_TINCOMPATIVEL);
        }
        geraCodigo(NULL, "CONJ");
        flag_var=0;
    }
;

termo:
    fator 
    {
        // debug_print ("Regra: %s | %s | tipo_fator=[%d]\n","termo","FATOR", $1);
        $$ = $1;
    }
    | termo MULT fator 
    {
        // debug_print ("Regra: %s | %s | tipo_termo=[%d] tipo_fator=[%d]\n","termo","MULT", $1, $3);
        if ( ($1 == $3) && ($1 == TIPO_INT) ){
            $$ = $1;
        } else {
            erro(ERRO_TINCOMPATIVEL);
        }
        geraCodigo(NULL, "MULT");
        flag_var=0;
    }
    | termo DIV fator 
    {
        // debug_print ("Regra: %s | %s | tipo_termo=[%d] tipo_fator=[%d]\n","termo","DIV", $1, $3);
        if ( ($1 == $3) && ($1 == TIPO_INT) ){
            $$ = $1;
        } else {
            erro(ERRO_TINCOMPATIVEL);
        }
        geraCodigo(NULL, "DIVI");
        flag_var=0;
    }
    | termo AND fator 
    {
        // debug_print ("Regra: %s | %s | tipo_termo=[%d] tipo_fator=[%d]\n","termo","AND", $1, $3);
        if ( ($1 == $3) && ($1 == TIPO_BOOL) ){
            $$ = $1;
        } else {
            erro(ERRO_TINCOMPATIVEL);
        }
        geraCodigo(NULL, "DISJ");
        flag_var=0;
    }
;

fator:
    ABRE_PARENTESES expressao FECHA_PARENTESES
    {
        $$ = $2;
        // debug_print ("Regra: %s | %s\n","fator","(EXPR)");
    }
    | NUMERO
    {
        $$ = TIPO_INT;
        enfileira_param_string(token);
        geraCodigo(NULL, "CRCT");
        flag_var=0;
        // debug_print ("Regra: %s | %s\n","fator","NUMERO");
    }
    | var
    {
        s = TS_busca(token, tabela_simbolos);
        if (s == NULL) {
            erro(ERRO_VS_NDECL);
        }
        switch (s->base.categoria){
            case CAT_PF:
                $$ = s->pf.tipo;
                flag_var=1;
                break;
            case CAT_VS:
                $$ = s->vs.tipo;
                flag_var=1;
                break;
            case CAT_FUNC:
                $$ = s->func.tipo;
                flag_var=0;
                break;
            default:
                erro(ERRO_ATRIB);
                break;
        }
        carrega(s);
        // debug_print ("Regra: %s | %s\n","fator","VAR");
    }
    | NOT fator
    {
        if ($2 == TIPO_BOOL) {
                $$ = $2;
        }
        else {
            erro(ERRO_TINCOMPATIVEL);
        }
        geraCodigo(NULL, "NEGA");
        flag_var=0;
        // debug_print ("Regra: %s | %s\n","fator","NOT");
    }
    | T_TRUE
    {
        $$ = TIPO_BOOL;
        enfileira_param_int(1);
        geraCodigo(NULL, "CRCT");
        flag_var=0;
        // debug_print ("Regra: %s | %s\n","fator","T_TRUE");
    }
    | T_FALSE
    {
        $$ = TIPO_BOOL;
        enfileira_param_int(0);
        geraCodigo(NULL, "CRCT");
        flag_var=0;
        // debug_print ("Regra: %s | %s\n","fator","T_FALSE");
    }


// regra_label:
//     NUMERO DOIS_PONTOS
// ;

%%

int main (int argc, char** argv) {
    FILE* fp;
    extern FILE* yyin;
    
    extern int yydebug;
    yydebug = 1;
    
    if (argc<2 || argc>2) {
        printf("usage compilador <arq>a %d\n", argc);
        return(-1);
    }
    
    fp=fopen (argv[1], "r");
    if (fp == NULL) {
        printf("usage compilador <arq>b\n");
        return(-1);
    }
    
    yyin=fp;
    yyparse();
    return 0;
}

