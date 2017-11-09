%{
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "compilador.h"
#include "pilha.h"


void erro(erros e){
    int msg_size=100;
    char msg[msg_size];
    switch (e){
        case ERRO_ATRIB:
            snprintf(msg, msg_size, "Recipiente inválido para atribuição.");
            break;
        case ERRO_TPARAM:
            snprintf(msg, msg_size, "Tipo do parâmetro não compatível com assinatura");
            break;
        case ERRO_NPARAM:
            snprintf(msg, msg_size, "Número de parâmetros não coincide com assinatura.");
            break;
        case ERRO_IDENT_DECL:
            snprintf(msg, msg_size, "Identificador já declarado.");
            break;
        case ERRO_VS_NDECL:
            snprintf(msg, msg_size, "Variável '%s' não declarada.", token); 
            break;
        case ERRO_PROC_NDECL:
            snprintf(msg, msg_size, "Procedimento não declarado.");
            break;
        case ERRO_FUNC_NDECL:
            snprintf(msg, msg_size, "Função não declarada.");
            break;
        case ERRO_ROT_NDECL:
            snprintf(msg, msg_size, "Rótulo não declarado.");
            break;
        case ERRO_TINCOMPATIVEL:
            snprintf(msg, msg_size, "Operação com tipos incompatíveis.");
            break;
        default:
            snprintf(msg, msg_size, "Erro desconhecido.");
            break;
    }
    fprintf(stderr,"\n# %s\n# Abortando.\n\n",msg);
    TS_imprime(tabela_simbolos);
    exit(e);
    return;
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
    snprintf(s_rot, TAM_ROT+1, "R%.2d", d_rot++);
    return s_rot;
}

void aloca_mem(){
  if(num_vars>0){
    char comando[5]="AMEM"; 
    char *parametros[3];
    // aloca variável aux pra converter num_vars pra string
    char aux[10];
    // converte num_vars pra string
    snprintf(aux, sizeof(aux), "%d", num_vars);
    // aloca memoria para primeiro param
    parametros[0] = (char *) realloc( (void *) parametros[0], sizeof(char)*(strlen(aux)+1) );
    parametros[1] = NULL;
    parametros[2] = NULL;
    // armazena valor em parametro 1
    strncpy(parametros[0], aux, sizeof(parametros[0]));
    debug_print ("num_vars=[%d] comando=[%s] parametros[0]=[%s]\n", num_vars, comando, parametros[0]);
    num_vars=0; // zera contador de variáveis
    geraCodigo (NULL, comando, parametros);
  }  
}

/* Decide tipo de armazenamento */
void armazena(){
    int tam_params=10;
    parametros[0] = (char *) realloc( (void *) parametros[0], sizeof(char)*tam_params ); // tam_params char de memória
    parametros[1] = (char *) realloc( (void *) parametros[1], sizeof(char)*tam_params ); // tam_params char de memória
    parametros[2] = NULL;
    char s_str[1000];
    TS_simbolo2str(aux_atrib.s, s_str);
    debug_print("%s\n",s_str);
    switch (aux_atrib.s->base.categoria){
        case CAT_FUNC:
            snprintf(parametros[0], tam_params, "%d", aux_atrib.s->func.nivel_lexico);
            snprintf(parametros[1], tam_params, "%d", aux_atrib.s->func.deslocamento);
            geraCodigo(NULL, "ARMZ", parametros);
            break;
        case CAT_PF:
            snprintf(parametros[0], tam_params, "%d", aux_atrib.s->pf.nivel_lexico);
            snprintf(parametros[1], tam_params, "%d", aux_atrib.s->pf.deslocamento);
            if (aux_atrib.s->pf.passagem == PASS_VAL){
                geraCodigo(NULL, "ARMZ", parametros);
            }else{
                geraCodigo(NULL, "ARMI", parametros);
            }
            break;
        case CAT_VS:
            snprintf(parametros[0], tam_params, "%d", aux_atrib.s->vs.nivel_lexico);
            snprintf(parametros[1], tam_params, "%d", aux_atrib.s->vs.deslocamento);
            geraCodigo(NULL, "ARMZ", parametros);
            break;
        default:
            debug_print("%s","Erro. categoria invalida.\n");
            break;
    }
}

// /* Decide tipo de carrega */
void carrega(tipo_simbolo *simb){
    int tam_params=10;
    parametros[0] = (char *) realloc( (void *) parametros[0], sizeof(char)*tam_params ); // tam_params char de memória
    parametros[1] = (char *) realloc( (void *) parametros[1], sizeof(char)*tam_params ); // tam_params char de memória
    parametros[2] = NULL;
    char s_str[1000];
    TS_simbolo2str(simb, s_str);
    debug_print("%s\n",s_str);
    switch (simb->base.categoria){
        case CAT_PF:
            snprintf(parametros[0], tam_params, "%d", simb->pf.nivel_lexico);
            snprintf(parametros[1], tam_params, "%d", simb->pf.deslocamento);
            geraCodigo(NULL, "CRVL", parametros);
//             if (simb->pf.passagem == valor){
//                 geraCodigo(NULL, "ARMZ", parametros);
//             }else{
//                 geraCodigo(NULL, "ARMI", parametros);
//             }
            break;
        case CAT_VS:
            snprintf(parametros[0], tam_params, "%d", simb->vs.nivel_lexico);
            snprintf(parametros[1], tam_params, "%d", simb->vs.deslocamento);
            geraCodigo(NULL, "CRVL", parametros);
            break;
        default:
            debug_print("%s","Erro. categoria invalida.\n");
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

%%

/* FLUXO PRINCIPAL */

programa :
    {
        num_vars=0;
        nivel_lexico=0;
        d_rot=0;
        tabela_simbolos = constroi_pilha();
        geraCodigo (NULL, "INPP", NULL); 
    }
    PROGRAM IDENT 
    ABRE_PARENTESES lista_idents FECHA_PARENTESES PONTO_E_VIRGULA
    bloco PONTO
    {
        geraCodigo (NULL, "PARA", NULL);
        TS_imprime(tabela_simbolos);
    }
;

bloco : 
    parte_declara_vars
    {
        aloca_mem();
    }
    comando_composto 
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

/* DECLARACAO DE VARS */

parte_declara_vars:
    VAR declara_vars
    |
;

declara_vars: declara_vars declara_var 
            | declara_var 
;

declara_var :
    lista_id_var DOIS_PONTOS 
    tipo 
    PONTO_E_VIRGULA
;

tipo :
    BOOL
    {
        TS_atualiza_tipos(TIPO_BOOL,tabela_simbolos);
    }
    | INT
    {
        TS_atualiza_tipos(TIPO_INT,tabela_simbolos);
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
        empilha((void *) s, tabela_simbolos);
    }

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
;

comando_sem_rotulo:
    atrib
    |
;

atrib:
    {
        debug_print("begin atribuicao. token=[%s]\n", token);
        aux_atrib.s = TS_busca(token, tabela_simbolos);
    }
    IDENT ATRIBUICAO expressao
    {
        debug_print ("Regra: %s | tipo_ident=[%d] tipo_expressao=[%d]\n","atrib", aux_atrib.tipo, $4);
        debug_print ("$4=[%d]\n", $4);
        debug_print ("%s\n", TS_simbolo2str(aux_atrib.));
        if ( $4 != aux_atrib.tipo ) {
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
        debug_print ("Regra: %s | %s | tipo_expsimp=[%d]\n","expressao","EXP_SIMP", $1);
        $$ = $1;
    }
    | expressao_simples IGUAL expressao_simples
    {
        debug_print ("Regra: %s | %s | tipo_expsimp1=[%d] tipo_expsimp2=[%d]\n","expressao","IGUAL", $1, $3);
        if ($1 == $3){
            $$ = TIPO_BOOL;
        } else {
            erro(ERRO_TINCOMPATIVEL);
        }
        geraCodigo(NULL, "CMIG", NULL);
    }
    | expressao_simples DIFERENTE expressao_simples
    {
        debug_print ("Regra: %s | %s | tipo_expsimp1=[%d] tipo_expsimp2=[%d]\n","expressao","DIFERENTE", $1, $3);
        if ($1 == $3){
            $$ = TIPO_BOOL;
        } else {
            erro(ERRO_TINCOMPATIVEL);
        }
        geraCodigo(NULL, "CMDG", NULL);
    }
    | expressao_simples MENOR expressao_simples
    {
        debug_print ("Regra: %s | %s | tipo_expsimp1=[%d] tipo_expsimp2=[%d]\n","expressao","MENOR", $1, $3);
        if ( ($1 == $3) && ($1 == TIPO_INT) ){
            $$ = TIPO_BOOL;
        } else {
            erro(ERRO_TINCOMPATIVEL);
        }
        geraCodigo(NULL, "CMME", NULL);
    }
    | expressao_simples MENOR_IGUAL expressao_simples
    {
        debug_print ("Regra: %s | %s | tipo_expsimp1=[%d] tipo_expsimp2=[%d]\n","expressao","MENOR_IGUAL", $1, $3);
        if ( ($1 == $3) && ($1 == TIPO_INT) ){
            $$ = TIPO_BOOL;
        } else {
            erro(ERRO_TINCOMPATIVEL);
        }
        geraCodigo(NULL, "CMEG", NULL);
    }
    | expressao_simples MAIOR_IGUAL expressao_simples
    {
        debug_print ("Regra: %s | %s | tipo_expsimp1=[%d] tipo_expsimp2=[%d]\n","expressao","MAIOR_IGUAL", $1, $3);
        if ( ($1 == $3) && ($1 == TIPO_INT) ){
            $$ = TIPO_BOOL;
        } else {
            erro(ERRO_TINCOMPATIVEL);
        }
        geraCodigo(NULL, "CMAG", NULL);
    }
    | expressao_simples MAIOR expressao_simples
    {
        debug_print ("Regra: %s | %s | tipo_expsimp1=[%d] tipo_expsimp2=[%d]\n","expressao","MAIOR", $1, $3);
        if ( ($1 == $3) && ($1 == TIPO_INT) ){
            $$ = TIPO_BOOL;
        } else {
            erro(ERRO_TINCOMPATIVEL);
        }
        geraCodigo(NULL, "CMMA", NULL);
    }
;

expressao_simples:
    MAIS termo
    {
        debug_print ("Regra: %s | %s | tipo_termo=[%d]\n","expressao_simples","TERMO POS", $2);
        if ( ($2 == TIPO_INT) ){
            $$ = $2;
        } else {
            erro(ERRO_TINCOMPATIVEL);
        }
    }
    | MENOS termo
    {
        debug_print ("Regra: %s | %s | tipo_termo=[%d]\n","expressao_simples","TERMO NEG", $2);
        if ( ($2 == TIPO_INT) ){
            $$ = $2;
        } else {
            erro(ERRO_TINCOMPATIVEL);
        }
        // TODO: PENSAR NO GERACODIGO
    }
    | termo
    {
        debug_print ("Regra: %s | %s | tipo_termo=[%d]\n","expressao_simples","TERMO", $1);
        $$ = $1;
    }
    | expressao_simples MAIS termo
    {
        debug_print ("Regra: %s | %s | tipo_expsimp=[%d] tipo_termo=[%d]\n","expressao_simples","SOMA", $1, $3);
        if ( ($1 == $3) && ($1 == TIPO_INT) ){
            $$ = $1;
        } else {
            erro(ERRO_TINCOMPATIVEL);
        }
        geraCodigo(NULL, "SOMA", NULL);
    }
    | expressao_simples MENOS termo
    {
        debug_print ("Regra: %s | %s | tipo_expsimp=[%d] tipo_termo=[%d]\n","expressao_simples","SUBT", $1, $3);
        if ( ($1 == $3) && ($1 == TIPO_INT) ){
            $$ = $1;
        } else {
            erro(ERRO_TINCOMPATIVEL);
        }
        geraCodigo(NULL, "SUBT", NULL);
    }
    | expressao_simples OR termo
    {
        debug_print ("Regra: %s | %s | tipo_expsimp=[%d] tipo_termo=[%d]\n","expressao_simples","OR", $1, $3);
        if ( ($1 == $3) && ($1 == TIPO_BOOL) ){
            $$ = $1;
        } else {
            erro(ERRO_TINCOMPATIVEL);
        }
        geraCodigo(NULL, "CONJ", NULL);
    }
;

termo:
    fator 
    {
        debug_print ("Regra: %s | %s | tipo_fator=[%d]\n","termo","FATOR", $1);
        $$ = $1;
    }
    | termo MULT fator 
    {
        debug_print ("Regra: %s | %s | tipo_termo=[%d] tipo_fator=[%d]\n","termo","MULT", $1, $3);
        if ( ($1 == $3) && ($1 == TIPO_INT) ){
            $$ = $1;
        } else {
            erro(ERRO_TINCOMPATIVEL);
        }
        geraCodigo(NULL, "MULT", NULL);
    }
    | termo DIV fator 
    {
        debug_print ("Regra: %s | %s | tipo_termo=[%d] tipo_fator=[%d]\n","termo","DIV", $1, $3);
        if ( ($1 == $3) && ($1 == TIPO_INT) ){
            $$ = $1;
        } else {
            erro(ERRO_TINCOMPATIVEL);
        }
        geraCodigo(NULL, "DIVI", NULL);
    }
    | termo AND fator 
    {
        debug_print ("Regra: %s | %s | tipo_termo=[%d] tipo_fator=[%d]\n","termo","AND", $1, $3);
        if ( ($1 == $3) && ($1 == TIPO_BOOL) ){
            $$ = $1;
        } else {
            erro(ERRO_TINCOMPATIVEL);
        }
        geraCodigo(NULL, "DISJ", NULL);
    }
;

fator:
    ABRE_PARENTESES expressao FECHA_PARENTESES
    {
        $$ = $2;
        debug_print ("Regra: %s | %s\n","fator","(EXPR)");
    }
    | NUMERO
    {
        $$ = TIPO_INT;
        parametros[0] = (char *) realloc( (void *) parametros[0], sizeof(char)*TAM_TOKEN+1 );
        parametros[1] = NULL;
        parametros[2] = NULL;
        strncpy(parametros[0], token, TAM_TOKEN+1);
        geraCodigo(NULL, "CRCT", parametros);
        debug_print ("Regra: %s | %s\n","fator","NUMERO");
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
                break;
            case CAT_VS:
                $$ = s->vs.tipo;
                break;
        }
        carrega(s);
        debug_print ("Regra: %s | %s\n","fator","VAR");
    }
    | NOT fator
    {
        if ($2 == TIPO_BOOL) {
                $$ = $2;
        }
        else {
            erro(ERRO_TINCOMPATIVEL);
        }
        geraCodigo(NULL, "NEGA", NULL);
        debug_print ("Regra: %s | %s\n","fator","NOT");
    }
    | T_TRUE
    {
        $$ = TIPO_BOOL;
        parametros[0] = (char *) realloc( (void *) parametros[0], sizeof(char)*2 );
        parametros[1] = NULL;
        parametros[2] = NULL;
        snprintf(parametros[0], 2, "1");
        geraCodigo(NULL, "CRCT", parametros);
        debug_print ("Regra: %s | %s\n","fator","T_TRUE");
    }
    | T_FALSE
    {
        $$ = TIPO_BOOL;
        parametros[0] = (char *) realloc( (void *) parametros[0], sizeof(char)*2 );
        parametros[1] = NULL;
        parametros[2] = NULL;
        snprintf(parametros[0], 2, "0");
        geraCodigo(NULL, "CRCT", parametros);
        debug_print ("Regra: %s | %s\n","fator","T_FALSE");
    }


// regra_label:
//     NUMERO DOIS_PONTOS
// ;

%%

main (int argc, char** argv) {
    FILE* fp;
    extern FILE* yyin;
    
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

