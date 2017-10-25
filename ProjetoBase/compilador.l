%{


#include<stdio.h>
#include<string.h>

#include "compilador.h"
#include "compilador.tab.h"

//#define IMPRIME(STR) {};
#define IMPRIME(STR) printf("%s    # %s \n", STR, token);

int nl = 1;  

%}

pulo_linha [\n]

descarte [\t ]+

ident [a-zAZ][a-zAZ1-9]*

numero [0-9]+

comentario "(*".*"*)"

%%

{pulo_linha} {nl++;} 

{descarte}

{comentario}

program { simbolo  = simb_program;  
    // Regra 1
    strncpy (token, yytext, TAM_TOKEN);
    IMPRIME("program ");
    return PROGRAM;
}

label { simbolo  = simb_label;  
    // Regra 3
    strncpy (token, yytext, TAM_TOKEN);
    IMPRIME("label ");
    return LABEL;
}

var { simbolo = simb_var;  
    // Regra 8
    strncpy (token, yytext, TAM_TOKEN);
    IMPRIME("var  ");
    return VAR;
}

procedure { simbolo = simb_procedure;  
    // Regra 12
    strncpy (token, yytext, TAM_TOKEN);
    IMPRIME("procedure  ");
    return PROCEDURE;
}

function { simbolo = simb_function;  
    // Regra 13
    strncpy (token, yytext, TAM_TOKEN);
    IMPRIME("function  ");
    return FUNCTION;
}

begin { simbolo = simb_begin;
    // Regra 16
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("begin ");
          return T_BEGIN;
 }

end { simbolo = simb_end;
    // Regra 16
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME("end  ");
          return T_END;
 }

goto { simbolo = simb_goto;  
    // Regra 21
    strncpy (token, yytext, TAM_TOKEN);
    IMPRIME("goto  ");
    return GOTO;
}

if { simbolo = simb_if;  
    // Regra 22
    strncpy (token, yytext, TAM_TOKEN);
    IMPRIME("if  ");
    return IF;
}

then { simbolo = simb_then;  
    // Regra 22
    strncpy (token, yytext, TAM_TOKEN);
    IMPRIME("then  ");
    return THEN;
}

else { simbolo = simb_else;  
    // Regra 22
    strncpy (token, yytext, TAM_TOKEN);
    IMPRIME("else  ");
    return ELSE;
}

while { simbolo = simb_while;  
    // Regra 23
    strncpy (token, yytext, TAM_TOKEN);
    IMPRIME("while  ");
    return WHILE;
}

integer { simbolo = simb_integer;  
    strncpy (token, yytext, TAM_TOKEN);
    IMPRIME("integer  ");
    return INT;
}

boolean { simbolo = simb_boolean;  
    strncpy (token, yytext, TAM_TOKEN);
    IMPRIME("boolean  ");
    return BOOL;
}

:= { simbolo = simb_atribuicao;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" atribuicao  ");
          return ATRIBUICAO;  
 }

\; { simbolo = simb_ponto_e_virgula;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" pt_virgula  ");
          return PONTO_E_VIRGULA;
 }

\: { simbolo = simb_dois_pontos;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" dois pontos ");
          return DOIS_PONTOS;
 }

\, { simbolo = simb_virgula;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" virgula  ");
          return VIRGULA;
 }


\. { simbolo = simb_ponto;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" ponto  ");
          return PONTO; 
    }

\(  { simbolo = simb_abre_parenteses;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" abre_parenteses  ");
          return ABRE_PARENTESES;
    }

\)  { simbolo = simb_fecha_parenteses;  
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" fecha_parenteses ");
          return FECHA_PARENTESES;
 }

{ident} { simbolo = simb_identificador; 
          strncpy (token, yytext, TAM_TOKEN);
          IMPRIME(" ident  ");
          return IDENT;
        }


%%

