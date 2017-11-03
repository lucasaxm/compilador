
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
  
  int comando_len = 4;
  char *comando_completo;
  // // adiciona comando em comando_completo
  // snprintf (comando_completo, sizeof(comando_completo), "%s", comando);
  // debug_print ("comando=[%s]\n", comando);
  
  if (fp == NULL) {
    fp = fopen ("MEPA", "w");
  }
  
  if (parametros!= NULL && parametros[0]!=NULL){
    
    
    // tamanho para + 1 espaco + 1 parametro
    comando_len = comando_len + 1 + strlen(parametros[0]);
    debug_print ("parametros[0]=[%s]\n", parametros[0]);
    
    if (parametros[1]!=NULL){
      
      // tamanho para + 1 virgula + 1 espaco + 1 parametro
      comando_len = comando_len + 2 + strlen(parametros[1]);
      debug_print ("parametros[1]=[%s]\n", parametros[1]);
      
      if (parametros[2]!=NULL){
        
        // tamanho para + 1 virgula + 1 espaco + 1 parametro
        comando_len = comando_len + 2 + strlen(parametros[2]);
        debug_print ("parametros[2]=[%s]\n", parametros[2]);
        
        /* 3 parametros */
        comando_completo = (char *) malloc(sizeof(char)*(comando_len+1));
        // comando_completo = "comando parametros[0], parametros[1], parametros[2]"
        snprintf (comando_completo, comando_len+1, "%s %s, %s, %s", comando, parametros[0], parametros[1], parametros[2]);
        
      } else { // 2 parametros
      
        comando_completo = (char *) malloc(sizeof(char)*(comando_len+1));
        // comando_completo = "comando parametros[0], parametros[1]"
        snprintf (comando_completo, comando_len+1, "%s %s, %s", comando, parametros[0], parametros[1]);
        
      }
    } else { // 1 parametro
    
      comando_completo = (char *) malloc(sizeof(char)*(comando_len+1));
      // comando_completo = "comando parametros[0]"
      snprintf (comando_completo, comando_len+1, "%s %s", comando, parametros[0]);
      
    }
  } else { // sem parametro
  
    comando_completo = (char *) malloc(sizeof(char)*(comando_len+1));
    snprintf (comando_completo, comando_len+1, "%s", comando);
    
  }
  
  debug_print("comando_len=[%d]\n",comando_len);
  if ( has_label ) {
    debug_print ("rot+comando_completo:[%s: %s]\n", rot, comando_completo);
    fprintf(fp, "%s: %s \n", rot, comando_completo); fflush(fp);
  } else {
    debug_print ("comando_completo=[%s]\n", comando_completo);
    fprintf(fp, "     %s\n", comando_completo); fflush(fp);
    has_label=0;
  }
}

int imprimeErro ( char* erro ) {
  fprintf (stderr, "Erro na linha %d - %s\n", nl, erro);
  exit(-1);
}
