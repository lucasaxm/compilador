
/* -------------------------------------------------------------------
 *            Aquivo: compilador.c
 * -------------------------------------------------------------------
 *              Autor: Bruno Muller Junior
 *               Data: 08/2007
 *      Atualizado em: [15/03/2012, 08h:22m]
 *
 * -------------------------------------------------------------------
 *
 * Funções auxiliares ao compilador
 *
 * ------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compilador.h"


/* -------------------------------------------------------------------
 *  variáveis globais
 * ------------------------------------------------------------------- */

FILE* fp=NULL;

void geraCodigo (char* rot, char* comando, char **parametros) {
  
  // aloca espaco para comando + \0 em comando_completo
  char *comando_completo = (char *) malloc (sizeof(char)*5);
  // adiciona comando em comando_completo
  snprintf (comando_completo, sizeof(comando_completo), "%s", comando);
  printf ("DEBUG | compiladorF.c | geraCodigo | comando: \"%s\"\n", comando);
  
  if (fp == NULL) {
    fp = fopen ("MEPA", "w");
  }
  
  if (parametros!= NULL && parametros[0]!=NULL){
    // aloca memoria para 1 parametro + 1 espaco
    comando_completo = (char *) realloc (comando_completo, sizeof(char)*(sizeof(comando_completo)+strlen(parametros[0])+1));
    snprintf (comando_completo, sizeof(comando_completo), "%s %s", comando, parametros[0]);
    // comando_completo = "comando parametros[0]"
    printf ("DEBUG | compiladorF.c | geraCodigo | parametros[0]: \"%s\"\n", parametros[0]);
    if (parametros[1]!=NULL){
      // aloca memoria para + 1 virgula + 1 espaco + 1 parametro
      comando_completo = (char *) realloc (comando_completo, sizeof(char)*(sizeof(comando_completo)+strlen(parametros[1])+2));
      snprintf (comando_completo, sizeof(comando_completo), "%s %s, %s", comando, parametros[0], parametros[1]);
      // comando_completo = "comando parametros[0], parametros[1]"
      printf ("DEBUG | compiladorF.c | geraCodigo | parametros[1]: \"%s\"\n", parametros[1]);
      if (parametros[2]!=NULL){
        // aloca memoria para + 1 virgula + 1 espaco + 1 parametro
        comando_completo = (char *) realloc (comando_completo, sizeof(char)*(sizeof(comando_completo)+strlen(parametros[2])+2));
        snprintf (comando_completo, sizeof(comando_completo), "%s %s, %s, %s", comando, parametros[0], parametros[1], parametros[2]);
        // comando_completo = "comando parametros[0], parametros[1], parametros[2]"
        printf ("DEBUG | compiladorF.c | geraCodigo | parametros[2]: \"%s\"\n", parametros[2]);
      }
    }
  }
  
  
  if ( rot == NULL ) {
    printf ("DEBUG | compiladorF.c | geraCodigo | comando_completo: \"%s\"\n", comando_completo);
    fprintf(fp, "     %s\n", comando_completo); fflush(fp);
  } else {
    printf ("DEBUG | compiladorF.c | geraCodigo | rot+comando_completo: \"%s: %s\"\n", rot, comando_completo);
    fprintf(fp, "%s: %s \n", rot, comando_completo); fflush(fp);
  }
}

int imprimeErro ( char* erro ) {
  fprintf (stderr, "Erro na linha %d - %s\n", nl, erro);
  exit(-1);
}
