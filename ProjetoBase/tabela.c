#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compilador.h" // compilador.h includes tabela.h

simbolo_vs TS_constroi_simbolo_vs(char *identificador, int nivel_lexico, int deslocamento, tipos tipo){
    simbolo_vs vs;
    
    vs.identificador = (char *) malloc(sizeof(char)*(strlen(identificador)+1));
    strncpy(vs.identificador, identificador, strlen(vs.identificador)+1);
    
    vs.categoria=CAT_VS;
    
    vs.nivel_lexico = nivel_lexico;
    
    vs.deslocamento = deslocamento;
    
    vs.tipo = tipo;
    
    debug_print("%s / VS(%d) / %d / %d / %d\n", vs.identificador, vs.categoria, vs.nivel_lexico, vs.deslocamento, vs.tipo);
    
    return vs;
}

simbolo_proc TS_constroi_simbolo_proc(char *identificador, categorias categoria, int nivel_lexico, char *rotulo, int n_params, param *params){
    simbolo_proc proc;
    
    proc.identificador = (char *) malloc(sizeof(char)*(strlen(identificador)+1));
    strncpy(proc.identificador, identificador, strlen(proc.identificador)+1);
    
    proc.categoria=CAT_PROC;
    
    proc.nivel_lexico = nivel_lexico;
    
    strncpy(proc.rotulo, rotulo, TAM_ROT);
    
    proc.n_params = n_params;
    
    proc.params = params;
    
    // params_str used for debug
    int params_str_max=200;
    char params_str[params_str_max];
    params_str[0]='\0';
    int i;
    for ( i=0; i < n_params; i++) {
        snprintf(params_str, params_str_max, "%s{%d, %d}", params_str, params[i].tipo, params[i].passagem);
    }
    debug_print("%s / PROC(%d) / %d / %s / %d{ %s }\n", proc.identificador, proc.categoria, proc.nivel_lexico, proc.rotulo, proc.n_params, params_str);
    
    return proc;
}

simbolo_pf TS_constroi_simbolo_pf(char *identificador, int nivel_lexico, int deslocamento, tipos tipo, tipos_passagem passagem){
    simbolo_pf pf;
    
    pf.identificador = (char *) malloc(sizeof(char)*(strlen(identificador)+1));
    strncpy(pf.identificador, identificador, strlen(pf.identificador)+1);
    
    pf.categoria=CAT_PF;
    
    pf.nivel_lexico = nivel_lexico;
    
    pf.deslocamento = deslocamento;
    
    pf.tipo = tipo;
    
    pf.passagem = passagem;
    
    debug_print("%s / PF(%d) / %d / %d / %d / %d\n", pf.identificador, pf.categoria, pf.nivel_lexico, pf.deslocamento, pf.tipo, pf.passagem);
    
    return pf;
}

simbolo_func TS_constroi_simbolo_func(char *identificador, categorias categoria, int nivel_lexico, int deslocamento, tipos tipo, char *rotulo, int n_params, param *params){
    simbolo_func func;
    
    func.identificador = (char *) malloc(sizeof(char)*(strlen(identificador)+1));
    strncpy(func.identificador, identificador, strlen(func.identificador)+1);
    
    func.categoria=CAT_FUNC;
    
    func.nivel_lexico = nivel_lexico;
    
    func.deslocamento = deslocamento;
    
    func.tipo = tipo;
    
    strncpy(func.rotulo, rotulo, TAM_ROT);
    
    func.n_params = n_params;
    
    func.params = params;
    
    // params_str used for debug
    int params_str_max=200;
    char params_str[params_str_max];
    params_str[0]='\0';
    int i;
    for ( i=0; i < n_params; i++) {
        snprintf(params_str, params_str_max, "%s{%d, %d}", params_str, params[i].tipo, params[i].passagem);
    }
    debug_print("%s / FUNC(%d) / %d / %d / %d / %s / %d{ %s }\n", func.identificador, func.categoria, func.nivel_lexico, func.deslocamento, func.tipo, func.rotulo, func.n_params, params_str);
    
    return func;
}

simbolo_rot TS_constroi_simbolo_rot(char *identificador, int nivel_lexico, char *rotulo){
    simbolo_rot rot;
    
    rot.identificador = (char *) malloc(sizeof(char)*(strlen(identificador)+1));
    strncpy(rot.identificador, identificador, strlen(rot.identificador)+1);
    
    rot.categoria=CAT_ROT;
    
    strncpy(rot.rotulo, rotulo, TAM_ROT);
    
    rot.nivel_lexico = nivel_lexico;
    
    debug_print("%s / ROT(%d) / %d / %s\n", rot.identificador, rot.categoria, rot.nivel_lexico, rot.rotulo);
    
    return rot;
}

void TS_imprime(pilha ts){
    no n;
    tipo_simbolo *s;
    char simbolo_str[1000];
    n = topo(ts);
    
    while ( n ) {
        s = (tipo_simbolo *) conteudo(n);
        TS_simbolo2str(s, simbolo_str);
        printf("%s\n",simbolo_str);
        n = proximo_no(n);
    }
}

void TS_simbolo2str(tipo_simbolo *s, char *str){
    int params_str_max=200;
    char params_str[params_str_max];
    int i;
    
    switch(s->base.categoria){
        case CAT_VS:
            sprintf(str, "%s / VS(%d) / %d / %d / %d", s->vs.identificador, s->vs.categoria, s->vs.nivel_lexico, s->vs.deslocamento, s->vs.tipo);
            break;
        case CAT_PROC:
            params_str[0]='\0';
            for ( i=0; i < s->proc.n_params; i++) {
                snprintf(params_str, params_str_max, "%s{%d, %d}", params_str, s->proc.params[i].tipo, s->proc.params[i].passagem);
            }
            sprintf(str, "%s / PROC(%d) / %d / %s / %d{ %s }", s->proc.identificador, s->proc.categoria, s->proc.nivel_lexico, s->proc.rotulo, s->proc.n_params, params_str);
            break;
        case CAT_PF:
            sprintf(str, "%s / PF(%d) / %d / %d / %d / %d", s->pf.identificador, s->pf.categoria, s->pf.nivel_lexico, s->pf.deslocamento, s->pf.tipo, s->pf.passagem);
            break;
        case CAT_FUNC:
            params_str[0]='\0';
            for ( i=0; i < s->func.n_params; i++) {
                snprintf(params_str, params_str_max, "%s{%d, %d}", params_str, s->func.params[i].tipo, s->func.params[i].passagem);
            }
            sprintf(str, "%s / FUNC(%d) / %d / %d / %d / %s / %d{ %s }", s->func.identificador, s->func.categoria, s->func.nivel_lexico, s->func.deslocamento, s->func.tipo, s->func.rotulo, s->func.n_params, params_str);
            break;
        case CAT_ROT:
            sprintf(str, "%s / ROT(%d) / %d / %s", s->rot.identificador, s->rot.categoria, s->rot.nivel_lexico, s->rot.rotulo);
            break;
    }
}

void TS_atualiza_tipos(tipos tipo, pilha ts){
    no n;
    tipo_simbolo *s;

    n = topo(ts);
    
    while ( n ) {
        s = (tipo_simbolo *) conteudo(n);
        if (s->vs.tipo == TIPO_UNKNOWN){
            s->vs.tipo = tipo;
            debug_print ("tipo da vs com ident. \"%s\" atualizado para %d\n", s->vs.identificador, s->vs.tipo);
        }
        else{
            return;
        }
        n = proximo_no(n);
    }
}

tipo_simbolo *TS_busca(char *identificador, pilha ts){
    no n;
    tipo_simbolo *s;
    char simb_str[1000];
    n = topo(ts);
    
    while ( n ) {
        s = (tipo_simbolo *) conteudo(n);
        if ( strcmp(identificador, s->base.identificador) == 0 ){
            TS_simbolo2str(s, simb_str);
            debug_print("Simbolo encontrado: %s.\n", simb_str);
            return s;
        }
        n = proximo_no(n);
    }
    
    return NULL;
}