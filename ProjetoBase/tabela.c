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

simbolo_proc TS_constroi_simbolo_proc(char *identificador, int nivel_lexico, char *rotulo, int n_params, param *params){
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

simbolo_func TS_constroi_simbolo_func(char *identificador, int nivel_lexico, int deslocamento, tipos tipo, char *rotulo, int n_params, param *params){
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
    imprime(ts,TS_simbolo2str);
}

void TS_simbolo2str(void *simbolo_void, char *str){
    int i;
    int str_len=0;
    tipo_simbolo *s = (tipo_simbolo *) simbolo_void;
    switch(s->base.categoria){
        case CAT_VS:
            str_len+=strlen(s->vs.identificador);
            str_len+=n_digitos(s->vs.categoria);
            str_len+=n_digitos(s->vs.nivel_lexico);
            str_len+=n_digitos(s->vs.deslocamento);
            str_len+=n_digitos(s->vs.tipo);
            str_len+=17; // outros caracteres
            str = realloc( str, sizeof(char)*(str_len+1) );
            snprintf(str, str_len+1, "%s / VS(%d) / %d / %d / %d", s->vs.identificador, s->vs.categoria, s->vs.nivel_lexico, s->vs.deslocamento, s->vs.tipo);
            break;
        case CAT_PROC: {
            int one_param_len=strlen("{0, 0}");
            int params_str_len = one_param_len*(s->proc.n_params)+1;
            char params_str[params_str_len];
            params_str[0]='\0';
            for ( i=0; i < s->proc.n_params; i++) {
                snprintf(params_str, params_str_len, "%s{%d, %d}", params_str, s->proc.params[i].tipo, s->proc.params[i].passagem);
            }
            str_len+=strlen(s->proc.identificador);
            str_len+=n_digitos(s->proc.categoria);
            str_len+=n_digitos(s->proc.nivel_lexico);
            str_len+=n_digitos(s->proc.rotulo);
            str_len+=n_digitos(s->proc.n_params);
            str_len+=strlen(params_str);
            str_len+=22;
            str = realloc( str, sizeof(char)*(str_len+1) );
            snprintf(str, str_len+1, "%s / PROC(%d) / %d / %s / %d{ %s }", s->proc.identificador, s->proc.categoria, s->proc.nivel_lexico, s->proc.rotulo, s->proc.n_params, params_str);
            break;
        }
        case CAT_PF:
            str_len+=strlen(s->vs.identificador);
            str_len+=n_digitos(s->pf.categoria);
            str_len+=n_digitos(s->pf.nivel_lexico);
            str_len+=n_digitos(s->pf.deslocamento);
            str_len+=n_digitos(s->pf.tipo);
            str_len+=n_digitos(s->pf.passagem);
            str_len+=19; // outros caracteres
            str = realloc( str, sizeof(char)*(str_len+1) );
            snprintf(str, str_len+1, "%s / PF(%d) / %d / %d / %d / %d", s->pf.identificador, s->pf.categoria, s->pf.nivel_lexico, s->pf.deslocamento, s->pf.tipo, s->pf.passagem);
            break;
        case CAT_FUNC: {
            int one_param_len=strlen("{0, 0}");
            int params_str_len = one_param_len*(s->proc.n_params)+1;
            char params_str[params_str_len];
            params_str[0]='\0';
            for ( i=0; i < s->func.n_params; i++) {
                snprintf(params_str, params_str_len, "%s{%d, %d}", params_str, s->func.params[i].tipo, s->func.params[i].passagem);
            }
            str_len+=strlen(s->func.identificador);
            str_len+=n_digitos(s->func.categoria);
            str_len+=n_digitos(s->func.nivel_lexico);
            str_len+=n_digitos(s->func.deslocamento);
            str_len+=n_digitos(s->func.tipo);
            str_len+=n_digitos(s->func.rotulo);
            str_len+=n_digitos(s->func.n_params);
            str_len+=strlen(params_str);
            str_len+=28;
            str = realloc( str, sizeof(char)*(str_len+1) );
            snprintf(str, str_len+1, "%s / FUNC(%d) / %d / %d / %d / %s / %d{ %s }", s->func.identificador, s->func.categoria, s->func.nivel_lexico, s->func.deslocamento, s->func.tipo, s->func.rotulo, s->func.n_params, params_str);
            break;
        }
        case CAT_ROT:
            str_len+=strlen(s->rot.identificador);
            str_len+=n_digitos(s->rot.categoria);
            str_len+=n_digitos(s->rot.nivel_lexico);
            str_len+=strlen(s->rot.rotulo);
            str_len+=14;
            sprintf(str, "%s / ROT(%d) / %d / %s", s->rot.identificador, s->rot.categoria, s->rot.nivel_lexico, s->rot.rotulo);
            break;
    }
}

/* Retorna 1 em sucesso e 0 em falha*/
int TS_atualiza_tipos(tipos tipo, categorias cat, pilha ts){
    no n;
    tipo_simbolo *s;
    int i;
    
    if (conta_tipo==0 && cat!=CAT_FUNC){
        debug_print("%s\n", "Nenhum simbolo para ser atualizado. Por que fui chamado?");
        return 0;
    }
    
    n=topo(ts);
    
    if (!n){
        debug_print("%s\n", "Tabela de simbolos vazia. Por que fui chamado?");
        return 0;
    }
    
    s = (tipo_simbolo *) conteudo(n);
    switch(cat){
        case CAT_VS:
            for (i=0; (i < conta_tipo) && n; i++, n=proximo_no(n)) {
                s = (tipo_simbolo *) conteudo(n);
                if (s->vs.tipo == TIPO_UNKNOWN){
                    s->vs.tipo = tipo;
                    debug_print ("tipo da vs com ident. \"%s\" atualizado para %d\n", s->vs.identificador, s->vs.tipo);
                }
                else{
                    debug_print ("tipo da vs com ident. \"%s\" ja estava definido (tipo=[%d]).\n", s->pf.identificador, s->pf.tipo);
                    return 0;
                }
            }
            break;
        case CAT_PF:
            for (i=0; (i < conta_tipo) && n; i++, n=proximo_no(n)) {
                s = (tipo_simbolo *) conteudo(n);
                if (s->pf.tipo == TIPO_UNKNOWN){
                    s->pf.tipo = tipo;
                    debug_print ("tipo do pf com ident. \"%s\" atualizado para %d\n", s->pf.identificador, s->pf.tipo);
                }
                else{
                    debug_print ("tipo do pf com ident. \"%s\" ja estava definido (tipo=[%d]).\n", s->pf.identificador, s->pf.tipo);
                    return 0;
                }
            }
            break;
        case CAT_FUNC:
            while(n){
                s = (tipo_simbolo *) conteudo(n);
                if (s->base.categoria == CAT_FUNC) break;
                n=proximo_no(n);
            }
            if(!n){
                debug_print("%s\n","Nenhuma func encontrada na tabela de simbolos.");
                return 0;
            }
            if (s->func.tipo == TIPO_UNKNOWN){
                s->func.tipo = tipo;
                debug_print ("tipo da func com ident. \"%s\" atualizado para %d\n", s->func.identificador, s->func.tipo);
            }
            else{
                debug_print ("tipo da func com ident. \"%s\" ja estava definido (tipo=[%d]).\n", s->pf.identificador, s->pf.tipo);
                return 0;
            }
            break;
    }
    conta_tipo = 0;
    return 1;
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

int TS_tamanho(pilha ts){
    return (int) tamanho_pilha(ts);
}

int TS_remove_vs(int nivel_lexico, pilha ts){
    // if (TS_tamanho(ts) <= 0)
    //     return 0;
        
    no n = topo(ts);
    int i=0;
    tipo_simbolo *s;
    
    while (n){
        s = (tipo_simbolo *) conteudo(n);
        if (s->base.categoria == CAT_VS){
            free (desempilha(ts));
            i++;
        }
        else {
            break;
        }
    }
    
    return i;
}