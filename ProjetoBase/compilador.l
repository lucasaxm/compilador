%{


#include<stdio.h>
#include<string.h>

#include "compilador.h"
#include "compilador.tab.h"

// #define IMPRIME(STR) {};
// #define IMPRIME(STR) printf("%s    # %s \n", STR, token);

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

program {
    // Regra 1
    strncpy (token, yytext, TAM_TOKEN);
    // IMPRIME("program ");
    return PROGRAM;
}

label {
    // Regra 3
    strncpy (token, yytext, TAM_TOKEN);
    // IMPRIME("label ");
    return LABEL;
}

var {
    // Regra 8
    strncpy (token, yytext, TAM_TOKEN);
    // IMPRIME("var  ");
    return VAR;
}

procedure {
    // Regra 12
    strncpy (token, yytext, TAM_TOKEN);
    // IMPRIME("procedure  ");
    return PROCEDURE;
}

function {
    // Regra 13
    strncpy (token, yytext, TAM_TOKEN);
    // IMPRIME("function  ");
    return FUNCTION;
}

begin {
    // Regra 16
          strncpy (token, yytext, TAM_TOKEN);
        //   IMPRIME("begin ");
          return T_BEGIN;
 }

end {
    // Regra 16
          strncpy (token, yytext, TAM_TOKEN);
        //   IMPRIME("end  ");
          return T_END;
 }

goto {
    // Regra 21
    strncpy (token, yytext, TAM_TOKEN);
    // IMPRIME("goto  ");
    return GOTO;
}

if {
    // Regra 22
    strncpy (token, yytext, TAM_TOKEN);
    // IMPRIME("if  ");
    return IF;
}

then {
    // Regra 22
    strncpy (token, yytext, TAM_TOKEN);
    // IMPRIME("then  ");
    return THEN;
}

else {
    // Regra 22
    strncpy (token, yytext, TAM_TOKEN);
    // IMPRIME("else  ");
    return ELSE;
}

while {
    // Regra 23
    strncpy (token, yytext, TAM_TOKEN);
    // IMPRIME("while  ");
    return WHILE;
}

integer {
    strncpy (token, yytext, TAM_TOKEN);
    // IMPRIME("integer  ");
    return INT;
}

boolean {
    strncpy (token, yytext, TAM_TOKEN);
    // IMPRIME("boolean  ");
    return BOOL;
}

:= {
    strncpy (token, yytext, TAM_TOKEN);
    // IMPRIME(" atribuicao  ");
    return ATRIBUICAO;  
 }

\; {
    strncpy (token, yytext, TAM_TOKEN);
    // IMPRIME(" pt_virgula  ");
    return PONTO_E_VIRGULA;
 }

\: {
    strncpy (token, yytext, TAM_TOKEN);
    // IMPRIME(" dois pontos ");
    return DOIS_PONTOS;
 }

\, {
    strncpy (token, yytext, TAM_TOKEN);
    // IMPRIME(" virgula  ");
    return VIRGULA;
 }


\. {
    strncpy (token, yytext, TAM_TOKEN);
    // IMPRIME(" ponto  ");
    return PONTO; 
    }

\(  {
    strncpy (token, yytext, TAM_TOKEN);
    // IMPRIME(" abre_parenteses  ");
    return ABRE_PARENTESES;
    }

\)  {
    strncpy (token, yytext, TAM_TOKEN);
    // IMPRIME(" fecha_parenteses ");
    return FECHA_PARENTESES;
 }

\* { 
    strncpy (token, yytext, TAM_TOKEN);
    // IMPRIME(" * ");
    return MULT;
 }

\+ { 
    strncpy (token, yytext, TAM_TOKEN);
    // IMPRIME(" + ");
    return MAIS;
 }

\- { 
    strncpy (token, yytext, TAM_TOKEN);
    // IMPRIME(" - ");
    return MENOS;
 }


\>  { 
  strncpy (token, yytext, TAM_TOKEN);
//   IMPRIME(" > ");
  return MAIOR;
}

\<  { 
  strncpy (token, yytext, TAM_TOKEN);
//   IMPRIME(" < ");
  return MENOR;
}

\>=  { 
  strncpy (token, yytext, TAM_TOKEN);
//   IMPRIME(" >=");
  return MAIOR_IGUAL;
}

\<=  { 
  strncpy (token, yytext, TAM_TOKEN);
//   IMPRIME(" <= ");
  return MENOR_IGUAL;
}

\<> { 
  strncpy (token, yytext, TAM_TOKEN);
//   IMPRIME(" <> ");
  return DIFERENTE;
}

\=  { 
  strncpy (token, yytext, TAM_TOKEN);
//   IMPRIME(" == ");
  return IGUAL;
}

or {
    strncpy (token, yytext, TAM_TOKEN);
    // IMPRIME(" or ");
    return OR;
}

and {
    strncpy (token, yytext, TAM_TOKEN);
    // IMPRIME(" and ");
    return AND;
}

NOT {
    strncpy (token, yytext, TAM_TOKEN);
    // IMPRIME(" NOT ");
    return NOT;
}

div {
    strncpy (token, yytext, TAM_TOKEN);
    // IMPRIME(" div ");
    return DIV;
}

true {
    strncpy (token, yytext, TAM_TOKEN);
    // IMPRIME(" true ");
    return T_TRUE;
}

false { 
    strncpy (token, yytext, TAM_TOKEN);
    // IMPRIME(" false ");
    return T_FALSE;
}

{ident} {
    strncpy (token, yytext, TAM_TOKEN);
    // IMPRIME(" ident  ");
    return IDENT;
}

{numero} {
  strncpy (token, yytext, TAM_TOKEN);
//   IMPRIME(" numero  ");
  return NUMERO;
}

%%

