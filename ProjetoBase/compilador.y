%{
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "compilador.h"
#include "pilha.h"

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
        case funcao:
            snprintf(parametros[0], tam_params, "%d", aux_atrib.s->func.nivel_lexico);
            snprintf(parametros[1], tam_params, "%d", aux_atrib.s->func.deslocamento);
            geraCodigo(NULL, "ARMZ", parametros);
            break;
        case param_formal:
            snprintf(parametros[0], tam_params, "%d", aux_atrib.s->pf.nivel_lexico);
            snprintf(parametros[1], tam_params, "%d", aux_atrib.s->pf.deslocamento);
            if (aux_atrib.s->pf.passagem == valor){
                geraCodigo(NULL, "ARMZ", parametros);
            }else{
                geraCodigo(NULL, "ARMI", parametros);
            }
            break;
        case var_simples:
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
        case param_formal:
            snprintf(parametros[0], tam_params, "%d", simb->pf.nivel_lexico);
            snprintf(parametros[1], tam_params, "%d", simb->pf.deslocamento);
            geraCodigo(NULL, "CRVL", parametros);
//             if (simb->pf.passagem == valor){
//                 geraCodigo(NULL, "ARMZ", parametros);
//             }else{
//                 geraCodigo(NULL, "ARMI", parametros);
//             }
            break;
        case var_simples:
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

%token PROGRAM ABRE_PARENTESES FECHA_PARENTESES VIRGULA PONTO_E_VIRGULA DOIS_PONTOS PONTO T_BEGIN
%token T_END VAR IDENT ATRIBUICAO LABEL PROCEDURE FUNCTION GOTO IF THEN ELSE WHILE INT BOOL NUMERO
%token MULT MAIS MENOS MAIOR MENOR MAIOR_IGUAL MENOR_IGUAL DIFERENTE IGUAL OR AND NOT DIV

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

declara_var : { }
    lista_id_var DOIS_PONTOS 
    tipo 
    PONTO_E_VIRGULA
;

tipo : { }
    BOOL
    {
        TS_atualiza_tipos(tboolean,tabela_simbolos);
    }
    | INT
    {
        TS_atualiza_tipos(tint,tabela_simbolos);
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
        s->vs = TS_constroi_simbolo_vs(token, nivel_lexico, num_vars++, tunknown);
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
        armazena();
        debug_print("end atribuicao. token=[%s]\n", token);
    }
;

/* EXPRESSOES */

expressao:
    expressao_simples
    {
        debug_print ("Regra: %s | %s\n","expressao","expressao_simples"); 
    }
    | expressao_simples IGUAL expressao_simples
    {
        geraCodigo(NULL, "CMIG", NULL);
        debug_print ("Regra: %s | %s\n","expressao","IGUAL");
    }
    | expressao_simples DIFERENTE expressao_simples
    {
        geraCodigo(NULL, "CMDG", NULL);
        debug_print ("Regra: %s | %s\n","expressao","DIFERENTE"); 
    }
    | expressao_simples MENOR expressao_simples
    {
        geraCodigo(NULL, "CMME", NULL);
        debug_print ("Regra: %s | %s\n","expressao","MENOR"); 
    }
    | expressao_simples MENOR_IGUAL expressao_simples
    {
        geraCodigo(NULL, "CMEG", NULL);
        debug_print ("Regra: %s | %s\n","expressao","MENOR_IGUAL"); 
    }
    | expressao_simples MAIOR_IGUAL expressao_simples
    {
        geraCodigo(NULL, "CMAG", NULL);
        debug_print ("Regra: %s | %s\n","expressao","MAIOR_IGUAL"); 
    }
    | expressao_simples MAIOR expressao_simples
    {
        geraCodigo(NULL, "CMMA", NULL);
        debug_print ("Regra: %s | %s\n","expressao","MAIOR"); 
    }
;

expressao_simples:
    MAIS termo
    {
        debug_print ("Regra: %s | %s\n","expressao_simples","TERMO POS");
    }
    | MENOS termo
    {
        // TODO: PENSAR NO GERACODIGO
        debug_print ("Regra: %s | %s\n","expressao_simples","TERMO NEG");
    }
    | termo
    {
        debug_print ("Regra: %s | %s\n","expressao_simples","TERMO");
    }
    | expressao_simples MAIS termo
    {
        geraCodigo(NULL, "SOMA", NULL);
        debug_print ("Regra: %s | %s\n","expressao_simples","SOMA");
    }
    | expressao_simples MENOS termo
    {
        geraCodigo(NULL, "SUBT", NULL);
        debug_print ("Regra: %s | %s\n","expressao_simples","SUBT");
    }
    | expressao_simples OR termo
    {
        geraCodigo(NULL, "CONJ", NULL);
        debug_print ("Regra: %s | %s\n","expressao_simples","OR");
    }
;

termo:
    fator 
    {
        debug_print ("Regra: %s | %s\n","termo","FATOR");
    }
    | termo MULT fator 
    {
        geraCodigo(NULL, "MULT", NULL);
        debug_print ("Regra: %s | %s\n","termo","MULT");
    }
    | termo DIV fator 
    {
        geraCodigo(NULL, "DIVI", NULL);
        debug_print ("Regra: %s | %s\n","termo","DIV");
    }
    | termo AND fator 
    {
        geraCodigo(NULL, "DISJ", NULL);
        debug_print ("Regra: %s | %s\n","termo","AND");
    }
;

fator:
    ABRE_PARENTESES expressao FECHA_PARENTESES { debug_print ("Regra: %s | %s\n","fator","(EXPR)"); }
    | NUMERO
    {
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
        carrega(s);
        debug_print ("Regra: %s | %s\n","fator","VAR");
    }
    | NOT fator
    {
        geraCodigo(NULL, "NEGA", NULL);
        debug_print ("Regra: %s | %s\n","fator","NOT");
    }

regra_label:
    NUMERO DOIS_PONTOS
;

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

