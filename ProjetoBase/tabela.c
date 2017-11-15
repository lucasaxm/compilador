#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compilador.h" // compilador.h includes tabela.h

simbolo_vs TS_constroi_simbolo_vs(char *identificador, int nivel_lexico, int deslocamento, tipos tipo){
    simbolo_vs vs;
    
    vs.identificador = (char *) malloc(sizeof(char)*(strlen(identificador)+1));
    strncpy(vs.identificador, identificador, strlen(identificador)+1);
    
    vs.categoria=CAT_VS;
    
    vs.nivel_lexico = nivel_lexico;
    
    vs.deslocamento = deslocamento;
    
    vs.tipo = tipo;
    
    debug_print("%s / VS(%d) / %d / %d / %d\n", vs.identificador, vs.categoria, vs.nivel_lexico, vs.deslocamento, vs.tipo);
    
    return vs;
}

simbolo_proc TS_constroi_simbolo_proc(char *identificador, int nivel_lexico, char *rotulo, int n_params, fila params){
    simbolo_proc proc;
    
    proc.identificador = (char *) malloc(sizeof(char)*(strlen(identificador)+1));
    strncpy(proc.identificador, identificador, strlen(identificador)+1);
    
    proc.categoria=CAT_PROC;
    
    proc.nivel_lexico = nivel_lexico;
    
    strncpy(proc.rotulo, rotulo, TAM_ROT);
    
    if (params){
        proc.n_params = n_params;
        proc.params = params;
    }
    else {
        proc.n_params = 0;
        proc.params = constroi_fila();
    }
    
    char *simb_str = TS_simbolo2str(&proc);
    debug_print("%s\n", simb_str);
    free (simb_str);
    
    return proc;
}

simbolo_pf TS_constroi_simbolo_pf(char *identificador, int nivel_lexico, int deslocamento, tipos tipo, tipos_passagem passagem){
    simbolo_pf pf;
    
    pf.identificador = (char *) malloc(sizeof(char)*(strlen(identificador)+1));
    strncpy(pf.identificador, identificador, strlen(identificador)+1);
    
    pf.categoria=CAT_PF;
    
    pf.nivel_lexico = nivel_lexico;
    
    pf.deslocamento = deslocamento;
    
    pf.tipo = tipo;
    
    pf.passagem = passagem;
    
    char *simb_str = TS_simbolo2str(&pf);
    debug_print("%s\n", simb_str);
    free (simb_str);
    
    return pf;
}

simbolo_func TS_constroi_simbolo_func(char *identificador, int nivel_lexico, int deslocamento, tipos tipo, char *rotulo, int n_params, fila params){
    simbolo_func func;
    
    func.identificador = (char *) malloc(sizeof(char)*(strlen(identificador)+1));
    strncpy(func.identificador, identificador, strlen(identificador)+1);
    
    func.categoria=CAT_FUNC;
    
    func.nivel_lexico = nivel_lexico;
    
    func.deslocamento = deslocamento;
    
    func.tipo = tipo;
    
    strncpy(func.rotulo, rotulo, TAM_ROT);
    
    if (params){
        func.n_params = n_params;
        func.params = params;
    }
    else {
        func.n_params = 0;
        func.params = constroi_fila();
    }
    
    char *simb_str = TS_simbolo2str(&func);
    debug_print("%s\n", simb_str);
    free (simb_str);
    
    return func;
}

simbolo_rot TS_constroi_simbolo_rot(char *identificador, int nivel_lexico, char *rotulo){
    simbolo_rot rot;
    
    rot.identificador = (char *) malloc(sizeof(char)*(strlen(identificador)+1));
    strncpy(rot.identificador, identificador, strlen(identificador)+1);
    
    rot.categoria=CAT_ROT;
    
    strncpy(rot.rotulo, rotulo, TAM_ROT);
    
    rot.nivel_lexico = nivel_lexico;
    
    char *simb_str = TS_simbolo2str(&rot);
    debug_print("%s\n", simb_str);
    free (simb_str);
    
    return rot;
}

void TS_imprime(pilha ts){
    imprime_pilha(ts,TS_simbolo2str);
}

char *TS_params2str(fila params){
    
    int param_str_len = strlen("{0, 0}");
    int params_str_len = param_str_len*(tamanho_fila(params));
    
    char param_str[param_str_len+1];
    char *params_str = malloc (sizeof(char)*(params_str_len+1));
    
    param *p;
    
    params_str[0]='\0';
    
    no_fila n = inicio(params);
    while (n) {
        p = (param *) conteudo_fila(n);
        snprintf(param_str, params_str_len+1, "{%d, %d}", p->tipo, p->passagem);
        strncat(params_str, param_str, params_str_len+1);
        n = proximo_no_fila(n);
    }
    return params_str;
}

char *TS_simbolo2str(void *simbolo_void){
    int str_len=0;
    tipo_simbolo *s = (tipo_simbolo *) simbolo_void;
    char *str;
    switch(s->base.categoria){
        case CAT_VS:
            str_len+=strlen(s->vs.identificador);
            str_len+=strlen(" / VS(");
            str_len+=n_digitos(s->vs.categoria);
            str_len+=strlen(") / ");
            str_len+=n_digitos(s->vs.nivel_lexico);
            str_len+=strlen(" / ");
            str_len+=n_digitos(s->vs.deslocamento);
            str_len+=strlen(" / ");
            str_len+=n_digitos(s->vs.tipo);
            str = malloc(sizeof(char)*(str_len+1));
            snprintf(str, str_len+1, "%s / VS(%d) / %d / %d / %d", s->vs.identificador, s->vs.categoria, s->vs.nivel_lexico, s->vs.deslocamento, s->vs.tipo);
            break;
        case CAT_PROC: {
            char *params_str = TS_params2str(s->proc.params);
            str_len+=strlen(s->proc.identificador);
            str_len+=strlen(" / PROC(");
            str_len+=n_digitos(s->proc.categoria);
            str_len+=strlen(") / ");
            str_len+=n_digitos(s->proc.nivel_lexico);
            str_len+=strlen(" / ");
            str_len+=n_digitos(s->proc.rotulo);
            str_len+=strlen(" / ");
            str_len+=n_digitos(s->proc.n_params);
            str_len+=strlen("{ ");
            str_len+=strlen(params_str);
            str_len+=strlen(" }");
            str = malloc(sizeof(char)*(str_len+1));
            snprintf(str, str_len+1, "%s / PROC(%d) / %d / %s / %d{ %s }", s->proc.identificador, s->proc.categoria, s->proc.nivel_lexico, s->proc.rotulo, s->proc.n_params, params_str);
            free(params_str);
            break;
        }
        case CAT_PF:
            str_len+=strlen(s->vs.identificador);
            str_len+=strlen(" / PF(");
            str_len+=n_digitos(s->pf.categoria);
            str_len+=strlen(") / ");
            str_len+=n_digitos(s->pf.nivel_lexico);
            str_len+=strlen(" / ");
            str_len+=n_digitos(s->pf.deslocamento);
            str_len+=strlen(" / ");
            str_len+=n_digitos(s->pf.tipo);
            str_len+=strlen(" / ");
            str_len+=n_digitos(s->pf.passagem);
            str = malloc(sizeof(char)*(str_len+1));
            snprintf(str, str_len+1, "%s / PF(%d) / %d / %d / %d / %d", s->pf.identificador, s->pf.categoria, s->pf.nivel_lexico, s->pf.deslocamento, s->pf.tipo, s->pf.passagem);
            break;
        case CAT_FUNC: {
            char *params_str = TS_params2str(s->proc.params);
            str_len+=strlen(s->func.identificador);
            str_len+=strlen(" / FUNC(");
            str_len+=n_digitos(s->func.categoria);
            str_len+=strlen(") / ");
            str_len+=n_digitos(s->func.nivel_lexico);
            str_len+=strlen(" / ");
            str_len+=n_digitos(s->func.deslocamento);
            str_len+=strlen(" / ");
            str_len+=n_digitos(s->func.tipo);
            str_len+=strlen(" / ");
            str_len+=n_digitos(s->func.rotulo);
            str_len+=strlen(" / ");
            str_len+=n_digitos(s->func.n_params);
            str_len+=strlen("{ ");
            str_len+=strlen(params_str);
            str_len+=strlen(" }");
            str = malloc(sizeof(char)*(str_len+1));
            snprintf(str, str_len+1, "%s / FUNC(%d) / %d / %d / %d / %s / %d{ %s }", s->func.identificador, s->func.categoria, s->func.nivel_lexico, s->func.deslocamento, s->func.tipo, s->func.rotulo, s->func.n_params, params_str);
            free (params_str);
            break;
        }
        case CAT_ROT:
            str_len+=strlen(s->rot.identificador);
            str_len+=strlen(" / ROT(");
            str_len+=n_digitos(s->rot.categoria);
            str_len+=strlen(") / ");
            str_len+=n_digitos(s->rot.nivel_lexico);
            str_len+=strlen(" / ");
            str_len+=strlen(s->rot.rotulo);
            str = malloc(sizeof(char)*(str_len+1));
            sprintf(str, "%s / ROT(%d) / %d / %s", s->rot.identificador, s->rot.categoria, s->rot.nivel_lexico, s->rot.rotulo);
            break;
    }
    return str;
}

/* Retorna 1 em sucesso e 0 em falha*/
int TS_atualiza_tipos(tipos tipo, categorias cat, pilha ts){
    no_pilha n;
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
    
    s = (tipo_simbolo *) conteudo_pilha(n);
    switch(cat){
        case CAT_VS:
            for (i=0; (i < conta_tipo) && n; i++, n=proximo_no_pilha(n)) {
                s = (tipo_simbolo *) conteudo_pilha(n);
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
            for (i=0; (i < conta_tipo) && n; i++, n=proximo_no_pilha(n)) {
                s = (tipo_simbolo *) conteudo_pilha(n);
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
                s = (tipo_simbolo *) conteudo_pilha(n);
                if (s->base.categoria == CAT_FUNC) break;
                n=proximo_no_pilha(n);
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
    no_pilha n;
    tipo_simbolo *s;
    char *simb_str;
    n = topo(ts);
    
    while ( n ) {
        s = (tipo_simbolo *) conteudo_pilha(n);
        if ( strcmp(identificador, s->base.identificador) == 0 ){
            char *simb_str_ptr = simb_str;
            simb_str = TS_simbolo2str(s);
            debug_print("Simbolo encontrado: %s.\n", simb_str);
            free (simb_str);
            return s;
        }
        n = proximo_no_pilha(n);
    }
    
    return NULL;
}

int TS_tamanho(pilha ts){
    return (int) tamanho_pilha(ts);
}

int TS_remove_vs(int nivel_lexico, pilha ts){
    no_pilha n = topo(ts);
    int i=0;
    tipo_simbolo *s;
    
    while (n){
        s = (tipo_simbolo *) conteudo_pilha(n);
        if (s->base.categoria == CAT_VS){
            free (desempilha(ts));
            i++;
        }
        else {
            break;
        }
        n = topo(ts);
    }
    
    return i;
}

void TS_empilha(tipo_simbolo *s, pilha ts){
    if (s) empilha((void *) s, ts);
}


void TS_atualiza_params(int num_params, pilha ts){
    debug_print("Atualizando %d parametros.\n", num_params);
    no_pilha n = topo(ts);
    int i=0;
    tipo_simbolo *s;
    param *p;
    tipo_simbolo *subrot = TS_busca_subrotina(ts);
    
    n = topo(ts);
    while (n){
        s = (tipo_simbolo *) conteudo_pilha(n);
        if (s->base.categoria == CAT_PF){
            
            p = (param *) malloc(sizeof(param));
            s->pf.deslocamento = -4-i;
            p->tipo = s->pf.tipo;
            p->passagem = s->pf.passagem;
            if (subrot->base.categoria == CAT_PROC){
                enfileira( (void *) p, subrot->proc.params );
                subrot->proc.n_params++;
            }
            else if (subrot->base.categoria == CAT_FUNC){
                enfileira( (void *) p, subrot->func.params );  
                subrot->func.n_params++;
            }
            
            i++;
        }
        else {
            break;
        }
        n = proximo_no_pilha(n);
    }
    debug_print("%d parametros atualizados.\n", i);
    TS_imprime(ts);
}

tipo_simbolo *TS_busca_subrotina(pilha ts){
    if (!ts)
        return NULL;
        
    no_pilha n = topo(ts);
    tipo_simbolo *subrot;
    
    while (n){ // acha simb da subrot
        subrot = (tipo_simbolo *) conteudo_pilha(n);
        if (subrot->base.categoria == CAT_PROC || subrot->base.categoria == CAT_FUNC){
            return subrot;
        }
        n = proximo_no_pilha(n);
    }
    return NULL;
}

void TS_remove_subrotina(int num_params, pilha ts){
    int i;
    for (i = 0; i < num_params+1; i++) {
        free(desempilha(ts));
    }
}