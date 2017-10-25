
// Testar se funciona corretamente o empilhamento de parâmetros
// passados por valor ou por referência.


%{
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "compilador.h"
#include "pilha.h"
#include "tabela.h"

int num_vars;
int nivel_lexico;
pilha tabela_simbolos;
tipo_simbolo *s;

void aloca_mem(){
  if(num_vars>0){
    char comando[5]="AMEM"; 
    char *parametros[3];
    // aloca variável aux pra converter num_vars pra string
    char aux[10];
    // converte num_vars pra string
    snprintf(aux, sizeof(aux), "%d", num_vars);
    // aloca memoria para primeiro param
    parametros[0] = (char *) malloc(sizeof(char)*(strlen(aux)+1));
    parametros[1] = NULL;
    parametros[2] = NULL;
    // armazena valor em parametro 1
    strncpy(parametros[0], aux, sizeof(parametros[0]));
    debug_print ("num_vars=[%d] comando=[%s] parametros[0]=[%s]\n", num_vars, comando, parametros[0]);
    num_vars=0; // zera contador de variáveis
    geraCodigo (NULL, comando, parametros);
  }  
}

%}

%token PROGRAM ABRE_PARENTESES FECHA_PARENTESES 
%token VIRGULA PONTO_E_VIRGULA DOIS_PONTOS PONTO
%token T_BEGIN T_END VAR IDENT ATRIBUICAO
%token LABEL PROCEDURE FUNCTION GOTO IF THEN ELSE WHILE INT BOOL

%%

programa :
    {
        num_vars=0;
        nivel_lexico=0;
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


/* DECLARACAO DE VARS */

parte_declara_vars:  var 
;


var         : { } VAR declara_vars
            |
;

declara_vars: declara_vars declara_var 
            | declara_var 
;

declara_var : { }
    lista_id_var DOIS_PONTOS 
    tipo 
    {
    }
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
    lista_id_var VIRGULA IDENT 
    { /* insere última var na tabela de símbolos */
        s = (tipo_simbolo *) malloc(sizeof(tipo_simbolo));
        s->vs = TS_constroi_simbolo_vs(token, nivel_lexico, num_vars++, tunknown);
        empilha((void *) s, tabela_simbolos);
    }
    | IDENT
    { /* insere vars na tabela de símbolos */
        s = (tipo_simbolo *) malloc(sizeof(tipo_simbolo));
        s->vs = TS_constroi_simbolo_vs(token, nivel_lexico, num_vars++, tunknown);
        empilha((void *) s, tabela_simbolos);
    }
;

lista_idents: lista_idents VIRGULA IDENT  
            | IDENT
;


comando_composto: T_BEGIN comandos T_END 

comandos:    
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
    
    
    // Inicia a Tabela de Símbolos
    pilha tabela_simbolos = constroi_pilha();
    
    yyin=fp;
    yyparse();
    return 0;
}

