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

simbolo_proc TS_constroi_simbolo_proc(char *identificador, int nivel_lexico, char *rotulo, int n_params, pilha params){
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
        proc.params = constroi_pilha();
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

simbolo_func TS_constroi_simbolo_func(char *identificador, int nivel_lexico, int deslocamento, tipos tipo, char *rotulo, int n_params, pilha params){
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
        func.params = constroi_pilha();
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
    
    strncpy(rot.rotulo, rotulo, TAM_ROT+1);
    
    rot.nivel_lexico = nivel_lexico;
    
    char *simb_str = TS_simbolo2str(&rot);
    debug_print("%s\n", simb_str);
    free (simb_str);
    
    return rot;
}

void TS_imprime(pilha ts){
    imprime_pilha(ts,TS_simbolo2str);
}

char *TS_params2str(pilha params){
    
    int param_str_len = strlen("{0, 0}");
    int params_str_len = param_str_len*(tamanho_pilha(params));
    
    char param_str[param_str_len+1];
    char *params_str = malloc (sizeof(char)*(params_str_len+1));
    
    param *p;
    
    params_str[0]='\0';
    
    no_pilha n = topo(params);
    while (n) {
        p = (param *) conteudo_pilha(n);
        snprintf(param_str, params_str_len+1, "{%d, %d}", p->tipo, p->passagem);
        strncat(params_str, param_str, params_str_len+1);
        n = proximo_no_pilha(n);
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
            char *params_str = TS_params2str(s->func.params);
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

void TS_atualiza_tipos(tipos tipo, categorias cat, pilha ts){
    no_pilha n;
    tipo_simbolo *s;
    int i;
    
    if (conta_tipo==0 && cat!=CAT_FUNC){
        debug_print("%s\n", "Nenhum simbolo para ser atualizado. Por que fui chamado?");
        return;
    }
    
    n=topo(ts);
    
    if (!n){
        debug_print("%s\n", "Tabela de simbolos vazia. Por que fui chamado?");
        return;
    }
    
    s = (tipo_simbolo *) conteudo_pilha(n);
    switch(cat){
        case CAT_VS:
            for (i=0; (i < conta_tipo) && n; i++, n=proximo_no_pilha(n)) {
                s = (tipo_simbolo *) conteudo_pilha(n);
                if (s->vs.tipo == TIPO_UNKNOWN){
                    s->vs.tipo = tipo;
                    debug_print ("Tipo da vs com ident. \"%s\" atualizado para %d\n", s->vs.identificador, s->vs.tipo);
                }
                else{
                    debug_print ("Tipo da vs com ident. \"%s\" ja estava definido (tipo=[%d]).\n", s->pf.identificador, s->pf.tipo);
                    erro(ERRO_TINCOMPATIVEL);
                }
            }
            break;
        case CAT_PF:
            for (i=0; (i < conta_tipo) && n; i++, n=proximo_no_pilha(n)) {
                s = (tipo_simbolo *) conteudo_pilha(n);
                if (s->pf.tipo == TIPO_UNKNOWN){
                    s->pf.tipo = tipo;
                    debug_print ("Tipo do pf com ident. \"%s\" atualizado para %d\n", s->pf.identificador, s->pf.tipo);
                }
                else{
                    debug_print ("Tipo do pf com ident. \"%s\" ja estava definido (tipo=[%d]).\n", s->pf.identificador, s->pf.tipo);
                    erro(ERRO_TINCOMPATIVEL);
                }
            }
            break;
        case CAT_FUNC:
            s = conteudo_pilha(topo(pilha_decl_subrot));
            if(!s){
                debug_print("%s\n", "Nenhuma funcao sendo declarada.");
                erro(ERRO_FUNC_NDECL);
            }
            if (s->func.tipo == TIPO_UNKNOWN){
                s->func.tipo = tipo;
                debug_print ("Tipo da func com ident. \"%s\" atualizado para %d\n", s->func.identificador, s->func.tipo);
            }
            else{
                debug_print ("Tipo da func com ident. \"%s\" ja estava definido (tipo=[%d]).\n", s->pf.identificador, s->pf.tipo);
                erro(ERRO_TINCOMPATIVEL);
            }
            break;
    }
    conta_tipo = 0;
}

tipo_simbolo *TS_busca(char *identificador, pilha ts){
    no_pilha n;
    tipo_simbolo *s;
    char *simb_str;
    n = topo(ts);
    
    while ( n ) {
        s = (tipo_simbolo *) conteudo_pilha(n);
        if ( strcmp(identificador, s->base.identificador) == 0 ){
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
    int removidos=0;
    int i=0;
    tipo_simbolo *s;
    char *s_str;
    debug_print("Removendo VS de nivel lexico %d da tabela de simbolos.\n", nivel_lexico);
    while (n){
        s = (tipo_simbolo *) conteudo_pilha(n);
        if ( (s->base.categoria == CAT_VS) && (s->vs.nivel_lexico==nivel_lexico) ){
            
            s_str = TS_simbolo2str(s);
            debug_print("Simbolo [%s] indice %d sera removido.\n",s_str, i);
            free (s_str);
            
            n = proximo_no_pilha(n);
            tipo_simbolo *s_removido = remove_indice_pilha(i,ts);
            
            s_str = TS_simbolo2str(s_removido);
            debug_print("Simbolo [%s] removido da tabela de simbolos.\n",s_str);
            free (s_str);
            
            free (s_removido);
            i--;
            removidos++;
        }
        else {
            n = proximo_no_pilha(n);
        }
        i++;
    }
    debug_print("%d simbolos removidos.\n", removidos);
    
    return removidos;
}

int TS_conta_vs(int nivel_lexico, pilha ts){
    no_pilha n = topo(ts);
    int contador=0;
    tipo_simbolo *s;
    char *s_str;
    debug_print("Contando VS de nivel lexico %d da tabela de simbolos.\n", nivel_lexico);
    while (n){
        s = (tipo_simbolo *) conteudo_pilha(n);
        if ( (s->base.categoria == CAT_VS) && (s->vs.nivel_lexico==nivel_lexico) ){
            
            s_str = TS_simbolo2str(s);
            debug_print("Simbolo [%s] encontrado.\n",s_str);
            free (s_str);
            
            n = proximo_no_pilha(n);
            
            contador++;
        }
        else {
            n = proximo_no_pilha(n);
        }
    }
    debug_print("%d VS encontradas.\n", contador);
    
    return contador;
}

void TS_empilha(tipo_simbolo *s, pilha ts){
    if (!s)
        erro(ERRO_DESCONHECIDO);
        
    char *ident1 = s->base.identificador;
    categorias cat1 = s->base.categoria;
    int nl1;
    
    tipo_simbolo *s2;
    char *ident2;
    categorias cat2;
    int nl2;
    
    no_pilha n = topo(ts);
    while (n) {
        s2 = (tipo_simbolo *) conteudo_pilha(n);
        n = proximo_no_pilha(n);
        ident2 = s2->base.identificador;
        cat2 = s2->base.categoria;
        if (strcmp(ident1, ident2)==0) {
            switch (cat1) {
                case CAT_VS:
                    nl1 = s->vs.nivel_lexico;
                    switch (cat2) {
                        case CAT_VS:
                            nl2 = s2->vs.nivel_lexico;
                            
                            if(nl1==nl2) { // VS mesmo nivel
                                debug_print("Identificador duplicado=[%s]\n",ident1);
                                TS_imprime(ts);
                                erro(ERRO_IDENT_DUPLICADO);
                            }
                            
                            break;
                        case CAT_PF:
                            nl2 = s2->pf.nivel_lexico;
                            
                            if(nl1==nl2) { // PF mesmo nivel
                                debug_print("Identificador duplicado=[%s]\n",ident1);
                                TS_imprime(ts);
                                erro(ERRO_IDENT_DUPLICADO);
                            }
                            
                            break;
                        case CAT_PROC:
                            nl2 = s2->proc.nivel_lexico;
                            
                            if (nl1+1==nl2) { // PROC 1 nivel maior que VS
                                debug_print("Identificador duplicado=[%s]\n",ident1);
                                TS_imprime(ts);
                                erro(ERRO_IDENT_DUPLICADO);
                            }
                                
                            break;
                        case CAT_FUNC:
                            nl2 = s2->func.nivel_lexico;
                            
                            if ((nl1+1==nl2) || (nl1==nl2)) { // FUNC 1 nivel maior ou igual que VS
                                debug_print("Identificador duplicado=[%s]\n",ident1);
                                TS_imprime(ts);
                                erro(ERRO_IDENT_DUPLICADO);
                            }
                            
                            break;
                    }
                    break;
                case CAT_PF: // Igual VS
                    nl1 = s->pf.nivel_lexico;
                    switch (cat2) {
                        case CAT_VS:
                            nl2 = s2->vs.nivel_lexico;
                            
                            if(nl1==nl2) { // VS mesmo nivel
                                debug_print("Identificador duplicado=[%s]\n",ident1);
                                TS_imprime(ts);
                                erro(ERRO_IDENT_DUPLICADO);
                            }
                            
                            break;
                        case CAT_PF:
                            nl2 = s2->pf.nivel_lexico;
                            
                            if(nl1==nl2) { // PF mesmo nivel (quer dizer que sao da mesma subrot)
                                debug_print("Identificador duplicado=[%s]\n",ident1);
                                TS_imprime(ts);
                                erro(ERRO_IDENT_DUPLICADO);
                            }
                            
                            break;
                        case CAT_PROC:
                            nl2 = s2->proc.nivel_lexico;
                            
                            if (nl1+1==nl2) { // PROC 1 nivel maior que PF
                                debug_print("Identificador duplicado=[%s]\n",ident1);
                                TS_imprime(ts);
                                erro(ERRO_IDENT_DUPLICADO);
                            }
                            
                            break;
                        case CAT_FUNC:
                            nl2 = s2->func.nivel_lexico;
                            
                            if ((nl1+1==nl2) || (nl1==nl2)) { // FUNC 1 nivel maior ou igual que PF
                                debug_print("Identificador duplicado=[%s]\n",ident1);
                                TS_imprime(ts);
                                erro(ERRO_IDENT_DUPLICADO);
                            }
                            
                            break;
                    }
                    break;
                case CAT_PROC:
                    nl1 = s->proc.nivel_lexico;
                    switch (cat2) {
                        case CAT_VS:
                            nl2 = s2->vs.nivel_lexico;
                            
                            if (nl1-1==nl2) { // PROC 1 nivel maior que VS
                                debug_print("Identificador duplicado=[%s]\n",ident1);
                                TS_imprime(ts);
                                erro(ERRO_IDENT_DUPLICADO);
                            }
                            
                            break;
                        case CAT_PF:
                            nl2 = s2->pf.nivel_lexico;
                            
                            if (nl1-1==nl2) { // PROC 1 nivel maior que PF
                                debug_print("Identificador duplicado=[%s]\n",ident1);
                                TS_imprime(ts);
                                erro(ERRO_IDENT_DUPLICADO);
                            }
                            
                            break;
                        case CAT_PROC:
                            nl2 = s2->proc.nivel_lexico;
                            
                            if ((nl1-1==nl2) || (nl1==nl2)) { // PROC 1 nivel maior ou igual que PROC
                                debug_print("Identificador duplicado=[%s]\n",ident1);
                                TS_imprime(ts);
                                erro(ERRO_IDENT_DUPLICADO);
                            }
                            
                            break;
                        case CAT_FUNC:
                            nl2 = s2->func.nivel_lexico;
                            
                            if (nl1-1==nl2) { // PROC 1 nivel maior que FUNC
                                debug_print("Identificador duplicado=[%s]\n",ident1);
                                TS_imprime(ts);
                                erro(ERRO_IDENT_DUPLICADO);
                            }
                            
                            break;
                    }
                    break;
                case CAT_FUNC:
                    nl1 = s->func.nivel_lexico;
                    switch (cat2) {
                        case CAT_VS:
                            nl2 = s2->vs.nivel_lexico;
                            
                            if ((nl1-1==nl2) || (nl1==nl2)) { // FUNC 1 nivel maior ou igual que VS
                                debug_print("Identificador duplicado=[%s]\n",ident1);
                                TS_imprime(ts);
                                erro(ERRO_IDENT_DUPLICADO);
                            }
                            
                            break;
                        case CAT_PF:
                            nl2 = s2->pf.nivel_lexico;
                            
                            if ((nl1-1==nl2) || (nl1==nl2)) { // FUNC 1 nivel maior ou igual que VS
                                debug_print("Identificador duplicado=[%s]\n",ident1);
                                TS_imprime(ts);
                                erro(ERRO_IDENT_DUPLICADO);
                            }
                            
                            break;
                        case CAT_PROC:
                            nl2 = s2->proc.nivel_lexico;
                            
                            if (nl1+1==nl2) { // PROC 1 nivel maior que FUNC
                                debug_print("Identificador duplicado=[%s]\n",ident1);
                                TS_imprime(ts);
                                erro(ERRO_IDENT_DUPLICADO);
                            }
                            
                            break;
                        case CAT_FUNC:
                            nl2 = s2->func.nivel_lexico;
                            
                            if ((nl1-1==nl2) || (nl1==nl2)) { // FUNC 1 nivel maior ou igual que VS
                                debug_print("Identificador duplicado=[%s]\n",ident1);
                                TS_imprime(ts);
                                erro(ERRO_IDENT_DUPLICADO);
                            }
                            
                            break;
                    }
                    break;
            }
        }
    }
    
     empilha((void *) s, ts);
}

void TS_atualiza_params(int num_params, pilha ts){
    no_pilha n;
    int i=0;
    tipo_simbolo *s;
    param *p;
    char *s_str;
    tipo_simbolo *subrot = conteudo_pilha(topo(pilha_decl_subrot));
    if (subrot){
        s_str = TS_simbolo2str(subrot);
        debug_print("Atualizando %d parametros do simbolo [%s].\n", num_params, s_str);
        free (s_str);
    }
    else {
        erro(ERRO_FUNC_NDECL);
    }
    n = topo(ts);
    while (n){
        s = (tipo_simbolo *) conteudo_pilha(n);
        if (s->base.categoria == CAT_PF){
            
            p = (param *) malloc(sizeof(param));
            s->pf.deslocamento = -4-i;
            p->tipo = s->pf.tipo;
            p->passagem = s->pf.passagem;
            if (subrot->base.categoria == CAT_PROC){
                empilha( (void *) p, subrot->proc.params );
                subrot->proc.n_params++;
            }
            else if (subrot->base.categoria == CAT_FUNC){
                empilha( (void *) p, subrot->func.params );
                subrot->func.n_params++;
            }
            s_str = TS_simbolo2str(s);
            debug_print("Simbolo [%s] enfileirado.\n",s_str);
            free (s_str);
            i++;
        }
        else {
            break;
        }
        n = proximo_no_pilha(n);
    }
    if (subrot->base.categoria == CAT_FUNC) {
        subrot->func.deslocamento = -4-i;
        debug_print("Deslocamento da funcao atualizado para %d.\n", subrot->func.deslocamento);
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

tipo_simbolo *TS_busca_procedimento(char *ident, pilha ts){
    if (!ts)
        return NULL;
        
    no_pilha n = topo(ts);
    tipo_simbolo *subrot;
    
    while (n){ // acha simb da subrot
        subrot = (tipo_simbolo *) conteudo_pilha(n);
        if ((strncmp(ident, subrot->base.identificador, strlen(ident))==0) && (subrot->base.categoria == CAT_PROC)) {
            return subrot;
        }
        n = proximo_no_pilha(n);
    }
    return NULL;
}

tipo_simbolo *TS_busca_funcao(char *ident, pilha ts){
    if (!ts)
        return NULL;
        
    no_pilha n = topo(ts);
    tipo_simbolo *subrot;
    
    while (n){ // acha simb da subrot
        subrot = (tipo_simbolo *) conteudo_pilha(n);
        if ((strncmp(ident, subrot->base.identificador, strlen(ident))==0) && (subrot->base.categoria == CAT_FUNC)) {
            return subrot;
        }
        n = proximo_no_pilha(n);
    }
    return NULL;
}

void TS_remove_rtpr(tipo_simbolo *simb_rtpr, pilha ts){
    no_pilha n;
    int removidos=0;
    int i=0;
    int num_params;
    int niv_lex;
    tipo_simbolo *s;
    char *s_str;
    categorias cat = simb_rtpr->base.categoria;
    
    if (cat==CAT_PROC) {
        debug_print("Retornando de Proc. %s, nivel lexico %d.\n", simb_rtpr->proc.identificador, simb_rtpr->proc.nivel_lexico);
        num_params = simb_rtpr->proc.n_params;
        niv_lex = simb_rtpr->proc.nivel_lexico;
    }
    else {
        debug_print("Retornando de Func. %s, nivel lexico %d.\n", simb_rtpr->func.identificador, simb_rtpr->func.nivel_lexico);
        num_params = simb_rtpr->func.n_params;
        niv_lex = simb_rtpr->func.nivel_lexico;
    }
    
    n = topo(ts);
    while (n){
        s = (tipo_simbolo *) conteudo_pilha(n);
        
        if (( (s->base.categoria == CAT_PF) && (s->pf.nivel_lexico==nivel_lexico) )
        || ((s->base.categoria == CAT_PROC) && (s->proc.nivel_lexico>nivel_lexico))
        || ((s->base.categoria == CAT_FUNC) && (s->func.nivel_lexico>nivel_lexico)))
        {
            n = proximo_no_pilha(n);
            tipo_simbolo *s_removido = remove_indice_pilha(i,ts);
            
            s_str = TS_simbolo2str(s_removido);
            debug_print("Simbolo [%s] removido da tabela de simbolos.\n",s_str);
            free (s_str);
            
            free (s_removido);
            removidos++;
        }
        else {
            n = proximo_no_pilha(n);
            i++;
            continue;
        }
        
    }
    debug_print("%d simbolos removidos.\n", removidos);
}

void *TS_clona_param(void *param){
    void *copia = malloc(sizeof(int));
    memcpy(copia, param, sizeof(int));
    return copia;
}

void *TS_clona_simbolo(void *s){
    tipo_simbolo *orig = (tipo_simbolo *) s;
    tipo_simbolo *copia = malloc(sizeof(tipo_simbolo));
    switch(orig->base.categoria){ 
        case CAT_VS:
            copia->vs = TS_constroi_simbolo_vs(
                strdup(orig->vs.identificador),
                orig->vs.nivel_lexico,
                orig->vs.deslocamento,
                orig->vs.tipo
            );
            break;
        case CAT_PROC:
            copia->proc = TS_constroi_simbolo_proc(
                strdup(orig->proc.identificador),
                orig->proc.nivel_lexico,
                strdup(orig->proc.rotulo),
                orig->proc.n_params,
                clona_pilha(orig->proc.params, TS_clona_param)
            );
            break;
        case CAT_PF:
            copia->pf = TS_constroi_simbolo_pf(
                strdup(orig->pf.identificador),
                orig->pf.nivel_lexico,
                orig->pf.deslocamento,
                orig->pf.tipo,
                orig->pf.passagem
            );
            break;
        case CAT_FUNC:
            copia->func = TS_constroi_simbolo_func(
                strdup(orig->func.identificador),
                orig->func.nivel_lexico,
                orig->func.deslocamento,
                orig->func.tipo,
                strdup(orig->func.rotulo),
                orig->func.n_params,
                clona_pilha(orig->func.params, TS_clona_param)
            );
            break;
        case CAT_ROT:
            copia->rot = TS_constroi_simbolo_rot(
                strdup(orig->rot.identificador),
                orig->rot.nivel_lexico,
                strdup(orig->rot.rotulo)
            );
            break;
    }
    return (void *)copia;
}
