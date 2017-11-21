#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compilador.h" // compilador.h includes tabela.h

simbolo_vs *TS_constroi_simbolo_vs(char *identificador, int nivel_lexico, int deslocamento, tipo_simbolo *tipo){
    simbolo_vs *vs = malloc (sizeof(simbolo_vs));
    
    vs->identificador = (char *) malloc(sizeof(char)*(strlen(identificador)+1));
    strncpy(vs->identificador, identificador, strlen(identificador)+1);
    
    vs->categoria=CAT_VS;
    
    vs->nivel_lexico = nivel_lexico;
    
    vs->deslocamento = deslocamento;
    
    vs->tipo = tipo;
    
    tipo_simbolo s;
    s.vs = vs;
    char *s_str = TS_simbolo2str(&s);
    debug_print("%s\n", s_str);
    free (s_str);
    
    return vs;
}

simbolo_type *TS_constroi_simbolo_type(char *identificador, int nivel_lexico, tipos_primitivos tipo){
    simbolo_type *type = malloc (sizeof(simbolo_type));
    
    type->identificador = (char *) malloc(sizeof(char)*(strlen(identificador)+1));
    strncpy(type->identificador, identificador, strlen(identificador)+1);
    
    type->categoria=CAT_TYPE;
    
    type->nivel_lexico = nivel_lexico;
    
    type->tipo = tipo;
    
    tipo_simbolo s;
    s.type = type;
    char *s_str = TS_simbolo2str(&s);
    debug_print("%s\n", s_str);
    free (s_str);
    
    return type;
}

simbolo_proc *TS_constroi_simbolo_proc(char *identificador, int nivel_lexico, char *rotulo, int n_params, pilha params){
    simbolo_proc *proc = malloc (sizeof(simbolo_proc));
    
    proc->identificador = (char *) malloc(sizeof(char)*(strlen(identificador)+1));
    strncpy(proc->identificador, identificador, strlen(identificador)+1);
    
    proc->categoria=CAT_PROC;
    
    proc->nivel_lexico = nivel_lexico;
    
    strncpy(proc->rotulo, rotulo, TAM_ROT+1);
    
    if (params){
        proc->n_params = n_params;
        proc->params = params;
    }
    else {
        proc->n_params = 0;
        proc->params = constroi_pilha();
    }
    
    
    tipo_simbolo s;
    s.proc = proc;
    char *s_str = TS_simbolo2str(&s);
    debug_print("%s\n", s_str);
    free (s_str);
    
    return proc;
}

simbolo_pf *TS_constroi_simbolo_pf(char *identificador, int nivel_lexico, int deslocamento, tipo_simbolo *tipo, tipos_passagem passagem){
    simbolo_pf *pf = malloc (sizeof(simbolo_pf));
    
    pf->identificador = (char *) malloc(sizeof(char)*(strlen(identificador)+1));
    strncpy(pf->identificador, identificador, strlen(identificador)+1);
    
    pf->categoria=CAT_PF;
    
    pf->nivel_lexico = nivel_lexico;
    
    pf->deslocamento = deslocamento;
    
    pf->tipo = tipo;
    
    pf->passagem = passagem;
    
    
    tipo_simbolo s;
    s.pf = pf;
    char *s_str = TS_simbolo2str(&s);
    debug_print("%s\n", s_str);
    free (s_str);
    
    return pf;
}

simbolo_func *TS_constroi_simbolo_func(char *identificador, int nivel_lexico, int deslocamento, tipo_simbolo *tipo, char *rotulo, int n_params, pilha params){
    simbolo_func *func = malloc (sizeof(simbolo_func));
    
    func->identificador = (char *) malloc(sizeof(char)*(strlen(identificador)+1));
    strncpy(func->identificador, identificador, strlen(identificador)+1);
    
    func->categoria=CAT_FUNC;
    
    func->nivel_lexico = nivel_lexico;
    
    func->deslocamento = deslocamento;
    
    func->tipo = tipo;
    
    strncpy(func->rotulo, rotulo, TAM_ROT);
    
    if (params){
        func->n_params = n_params;
        func->params = params;
    }
    else {
        func->n_params = 0;
        func->params = constroi_pilha();
    }
    
    
    tipo_simbolo s;
    s.func = func;
    char *s_str = TS_simbolo2str(&s);
    debug_print("%s\n", s_str);
    free (s_str);
    
    return func;
}

simbolo_rot *TS_constroi_simbolo_rot(char *identificador, int nivel_lexico, char *rotulo){
    simbolo_rot *rot = malloc (sizeof(simbolo_rot));
    
    rot->identificador = (char *) malloc(sizeof(char)*(strlen(identificador)+1));
    strncpy(rot->identificador, identificador, strlen(identificador)+1);
    
    rot->categoria=CAT_ROT;
    
    strncpy(rot->rotulo, rotulo, TAM_ROT+1);
    
    rot->nivel_lexico = nivel_lexico;
    
    
    tipo_simbolo s;
    s.rot = rot;
    char *s_str = TS_simbolo2str(&s);
    debug_print("%s\n", s_str);
    free (s_str);
    
    return rot;
}

void TS_imprime(pilha ts){
    imprime_pilha(ts,TS_simbolo2str);
}

char *TS_params2str(pilha params){
    
    
    no_pilha n = topo(params);
    int param_str_len = 0;
    int params_str_len = 0;
    param *p;
    while (n) {
        p = (param *) conteudo_pilha(n);
        param_str_len=0;
        
        param_str_len+=strlen("{");
        param_str_len+=strlen(p->tipo->type->identificador);
        param_str_len+=strlen(", ");
        param_str_len+=strlen(p->passagem==PASS_VAL ? "PASS_VAL" : "PASS_REF");
        param_str_len+=strlen("}");
        
        params_str_len+=param_str_len;
        n = proximo_no_pilha(n);
    }
    
    char param_str[params_str_len+1];
    char *params_str = malloc (sizeof(char)*(params_str_len+1));
    
    params_str[0]='\0';
    
    n = topo(params);
    while (n) {
        p = (param *) conteudo_pilha(n);
        snprintf(param_str, params_str_len+1, "{%s, %s}", p->tipo->type->identificador, p->passagem==PASS_VAL ? "PASS_VAL" : "PASS_REF");
        strncat(params_str, param_str, params_str_len+1);
        n = proximo_no_pilha(n);
    }
    return params_str;
}

char *TS_simbolo2str(void *simbolo_void){
    int str_len=0;
    tipo_simbolo *s = (tipo_simbolo *) simbolo_void;
    char *str;
    char *tipo;
    switch(s->base->categoria){
        case CAT_TYPE: {
            int tipo_len = 0;
            switch (s->type->tipo) {
                case TIPO_BOOL:
                    tipo_len = strlen("TIPO_BOOL");
                    tipo = malloc(sizeof(tipo_len+1));
                    strncpy(tipo, "TIPO_BOOL", tipo_len+1);
                    break;
                case TIPO_INT:
                    tipo_len = strlen("TIPO_INT");
                    tipo = malloc(sizeof(tipo_len+1));
                    strncpy(tipo, "TIPO_INT", tipo_len+1);
                    break;
                case TIPO_UNKNOWN:
                    tipo_len = strlen("TIPO_UNKNOWN");
                    tipo = malloc(sizeof(tipo_len+1));
                    strncpy(tipo, "TIPO_UNKNOWN", tipo_len+1);
                    break;
            }
            str_len+=strlen(s->type->identificador);
            str_len+=strlen(" / TYPE(");
            str_len+=n_digitos(s->type->categoria);
            str_len+=strlen(") / ");
            str_len+=n_digitos(s->type->nivel_lexico);
            str_len+=strlen(" / ");
            str_len+=tipo_len;
            str = malloc(sizeof(char)*(str_len+1));
            snprintf(str, str_len+1, "%s / TYPE(%d) / %d / %s", s->type->identificador, s->type->categoria, s->type->nivel_lexico, tipo);
            free (tipo);
            break;
        }
        case CAT_VS:
            str_len+=strlen(s->vs->identificador);
            str_len+=strlen(" / VS(");
            str_len+=n_digitos(s->vs->categoria);
            str_len+=strlen(") / ");
            str_len+=n_digitos(s->vs->nivel_lexico);
            str_len+=strlen(" / ");
            str_len+=n_digitos(s->vs->deslocamento);
            str_len+=strlen(" / ");
            if(s->vs->tipo)
                str_len+=strlen(s->vs->tipo->type->identificador);
            else
                str_len+=strlen("(null)");
            str = malloc(sizeof(char)*(str_len+1));
            snprintf(str, str_len+1, "%s / VS(%d) / %d / %d / %s", s->vs->identificador, s->vs->categoria, s->vs->nivel_lexico, s->vs->deslocamento, s->vs->tipo ? s->vs->tipo->type->identificador : "(null)");
            break;
        case CAT_PROC: {
            char *params_str = TS_params2str(s->proc->params);
            str_len+=strlen(s->proc->identificador);
            str_len+=strlen(" / PROC(");
            str_len+=n_digitos(s->proc->categoria);
            str_len+=strlen(") / ");
            str_len+=n_digitos(s->proc->nivel_lexico);
            str_len+=strlen(" / ");
            str_len+=strlen(s->proc->rotulo);
            str_len+=strlen(" / ");
            str_len+=n_digitos(s->proc->n_params);
            str_len+=strlen("{ ");
            str_len+=strlen(params_str);
            str_len+=strlen(" }");
            str = malloc(sizeof(char)*(str_len+1));
            snprintf(str, str_len+1, "%s / PROC(%d) / %d / %s / %d{ %s }", s->proc->identificador, s->proc->categoria, s->proc->nivel_lexico, s->proc->rotulo, s->proc->n_params, params_str);
            free(params_str);
            break;
        }
        case CAT_PF:
            str_len+=strlen(s->vs->identificador);
            str_len+=strlen(" / PF(");
            str_len+=n_digitos(s->pf->categoria);
            str_len+=strlen(") / ");
            str_len+=n_digitos(s->pf->nivel_lexico);
            str_len+=strlen(" / ");
            str_len+=n_digitos(s->pf->deslocamento);
            str_len+=strlen(" / ");
            if(s->pf->tipo)
                str_len+=strlen(s->pf->tipo->type->identificador);
            else
                str_len+=strlen("(null)");
            str_len+=strlen(" / ");
            str_len+=strlen(s->pf->passagem==PASS_VAL ? "PASS_VAL" : "PASS_REF");
            str = malloc(sizeof(char)*(str_len+1));
            snprintf(str, str_len+1, "%s / PF(%d) / %d / %d / %s / %s", s->pf->identificador, s->pf->categoria, s->pf->nivel_lexico, s->pf->deslocamento, s->pf->tipo ? s->pf->tipo->type->identificador : "(null)", s->pf->passagem==PASS_VAL ? "PASS_VAL" : "PASS_REF");
            break;
        case CAT_FUNC: {
            char *params_str = TS_params2str(s->func->params);
            str_len+=strlen(s->func->identificador);
            str_len+=strlen(" / FUNC(");
            str_len+=n_digitos(s->func->categoria);
            str_len+=strlen(") / ");
            str_len+=n_digitos(s->func->nivel_lexico);
            str_len+=strlen(" / ");
            str_len+=n_digitos(s->func->deslocamento);
            str_len+=strlen(" / ");
            if(s->vs->tipo)
                str_len+=strlen(s->vs->tipo->type->identificador);
            else
                str_len+=strlen("(null)");
            str_len+=strlen(" / ");
            str_len+=strlen(s->func->rotulo);
            str_len+=strlen(" / ");
            str_len+=n_digitos(s->func->n_params);
            str_len+=strlen("{ ");
            str_len+=strlen(params_str);
            str_len+=strlen(" }");
            str = malloc(sizeof(char)*(str_len+1));
            snprintf(str, str_len+1, "%s / FUNC(%d) / %d / %d / %s / %s / %d{ %s }", s->func->identificador, s->func->categoria, s->func->nivel_lexico, s->func->deslocamento, s->func->tipo ? s->func->tipo->type->identificador : "(null)", s->func->rotulo, s->func->n_params, params_str);
            free (params_str);
            break;
        }
        case CAT_ROT:
            str_len+=strlen(s->rot->identificador);
            str_len+=strlen(" / ROT(");
            str_len+=n_digitos(s->rot->categoria);
            str_len+=strlen(") / ");
            str_len+=n_digitos(s->rot->nivel_lexico);
            str_len+=strlen(" / ");
            str_len+=strlen(s->rot->rotulo);
            str = malloc(sizeof(char)*(str_len+1));
            sprintf(str, "%s / ROT(%d) / %d / %s", s->rot->identificador, s->rot->categoria, s->rot->nivel_lexico, s->rot->rotulo);
            break;
    }
    return str;
}

void TS_atualiza_tipos(tipo_simbolo *tipo, categorias cat, pilha ts){
    no_pilha n;
    tipo_simbolo *s;
    int i;
    int tipo_old_len;
    char *tipo_old;
    int tipo_novo_len;
    char *tipo_novo;
    
    if (conta_tipo==0 && cat!=CAT_FUNC){
        debug_print("%s\n", "Nenhum simbolo para ser atualizado. Por que fui chamado?");
        return;
    }
    
    if( (!tipo) || (tipo->base->categoria != CAT_TYPE) || (tipo->type->tipo == TIPO_UNKNOWN) )
        erro(ERRO_TIPONAOEXISTE);
    
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
                if (s->vs->tipo == NULL){
                    s->vs->tipo = tipo;
                    debug_print ("Tipo da vs com ident. [%s] atualizado para [%s]\n", s->vs->identificador, s->vs->tipo->type->identificador);
                }
                else{
                    debug_print ("Tipo da vs com ident. [%s] ja estava definido (tipo=[%s]).\n", s->pf->identificador, s->pf->tipo->type->identificador);
                    erro(ERRO_TINCOMPATIVEL);
                }
            }
            break;
        case CAT_PF:
            for (i=0; (i < conta_tipo) && n; i++, n=proximo_no_pilha(n)) {
                s = (tipo_simbolo *) conteudo_pilha(n);
                if (s->pf->tipo == NULL){
                    s->pf->tipo = tipo;
                    debug_print ("Tipo do pf com ident. [%s] atualizado para [%s]\n", s->pf->identificador, s->pf->tipo->type->identificador);
                }
                else{
                    debug_print ("Tipo do pf com ident. [%s] ja estava definido (tipo=[%s]).\n", s->pf->identificador, s->pf->tipo->type->identificador);
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
            if (s->func->tipo == NULL){
                s->func->tipo = tipo;
                debug_print ("Tipo da func com ident. [%s] atualizado para [%s]\n", s->func->identificador, s->func->tipo->type->identificador);
            }
            else{
                debug_print ("Tipo da func com ident. [%s] ja estava definido (tipo=[%s]).\n", s->pf->identificador, s->pf->tipo->type->identificador);
                erro(ERRO_TINCOMPATIVEL);
            }
            break;
        case CAT_TYPE:
            for (i=0; (i < conta_tipo) && n; i++, n=proximo_no_pilha(n)) {
                s = (tipo_simbolo *) conteudo_pilha(n);
                tipo_old_len = 0;
                switch (s->type->tipo) {
                    case TIPO_BOOL:
                        tipo_old_len = strlen("TIPO_BOOL");
                        tipo_old = malloc(sizeof(tipo_old_len+1));
                        strncpy(tipo_old, "TIPO_BOOL", tipo_old_len+1);
                        break;
                    case TIPO_INT:
                        tipo_old_len = strlen("TIPO_INT");
                        tipo_old = malloc(sizeof(tipo_old_len+1));
                        strncpy(tipo_old, "TIPO_INT", tipo_old_len+1);
                        break;
                    case TIPO_UNKNOWN:
                        tipo_old_len = strlen("TIPO_UNKNOWN");
                        tipo_old = malloc(sizeof(tipo_old_len+1));
                        strncpy(tipo_old, "TIPO_UNKNOWN", tipo_old_len+1);
                        break;
                }
                tipo_novo_len = 0;
                switch (s->type->tipo) {
                    case TIPO_BOOL:
                        tipo_novo_len = strlen("TIPO_BOOL");
                        tipo_novo = malloc(sizeof(tipo_novo_len+1));
                        strncpy(tipo_novo, "TIPO_BOOL", tipo_novo_len+1);
                        break;
                    case TIPO_INT:
                        tipo_novo_len = strlen("TIPO_INT");
                        tipo_novo = malloc(sizeof(tipo_novo_len+1));
                        strncpy(tipo_novo, "TIPO_INT", tipo_novo_len+1);
                        break;
                    case TIPO_UNKNOWN:
                        tipo_novo_len = strlen("TIPO_UNKNOWN");
                        tipo_novo = malloc(sizeof(tipo_novo_len+1));
                        strncpy(tipo_novo, "TIPO_UNKNOWN", tipo_novo_len+1);
                        break;
                }
                if (s->type->tipo == TIPO_UNKNOWN){
                    s->type->tipo = tipo->type->tipo;
                    debug_print ("Tipo primitivo do type com ident. [%s] atualizado para [%s]\n", s->type->identificador, tipo_novo);
                }
                else{
                    debug_print ("Tipo primitivo do type com ident. [%s] ja estava definido como [%s].\n", s->pf->identificador, tipo_old);
                    erro(ERRO_TINCOMPATIVEL);
                }
            }
            break;
        default:
            erro(ERRO_DESCONHECIDO);
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
        if ( strcmp(identificador, s->base->identificador) == 0 ){
            simb_str = TS_simbolo2str(s);
            debug_print("Simbolo encontrado: %s.\n", simb_str);
            free (simb_str);
            return s;
        }
        n = proximo_no_pilha(n);
    }
    
    return NULL;
}

tipo_simbolo *TS_busca_tipo(char *ident, pilha ts){
    if (!ts)
        return NULL;
        
    no_pilha n = topo(ts);
    tipo_simbolo *tipo;
    
    while (n){ // acha simb do tipo
        tipo = (tipo_simbolo *) conteudo_pilha(n);
        if ((strncmp(ident, tipo->base->identificador, strlen(ident))==0) && (tipo->base->categoria == CAT_TYPE)) {
            return tipo;
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
        if ( (s->base->categoria == CAT_VS) && (s->vs->nivel_lexico==nivel_lexico) ){
            
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
        if ( (s->base->categoria == CAT_VS) && (s->vs->nivel_lexico==nivel_lexico) ){
            
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
        
    char *ident1 = s->base->identificador;
    categorias cat1 = s->base->categoria;
    int nl1;
    
    tipo_simbolo *s2;
    char *ident2;
    categorias cat2;
    int nl2;
    
    no_pilha n = topo(ts);
    while (n) {
        s2 = (tipo_simbolo *) conteudo_pilha(n);
        n = proximo_no_pilha(n);
        ident2 = s2->base->identificador;
        cat2 = s2->base->categoria;
        if (strcmp(ident1, ident2)==0) {
            switch (cat1) {
                case CAT_VS:
                    nl1 = s->vs->nivel_lexico;
                    switch (cat2) {
                        case CAT_VS:
                            nl2 = s2->vs->nivel_lexico;
                            
                            if(nl1==nl2) { // VS mesmo nivel
                                debug_print("Identificador duplicado=[%s]\n",ident1);
                                if (DEBUG)
                                    TS_imprime(ts);
                                erro(ERRO_IDENT_DUPLICADO);
                            }
                            
                            break;
                        case CAT_PF:
                            nl2 = s2->pf->nivel_lexico;
                            
                            if(nl1==nl2) { // PF mesmo nivel
                                debug_print("Identificador duplicado=[%s]\n",ident1);
                                if (DEBUG)
                                    TS_imprime(ts);
                                erro(ERRO_IDENT_DUPLICADO);
                            }
                            
                            break;
                        case CAT_PROC:
                            nl2 = s2->proc->nivel_lexico;
                            
                            if (nl1+1==nl2) { // PROC 1 nivel maior que VS
                                debug_print("Identificador duplicado=[%s]\n",ident1);
                                if (DEBUG)
                                    TS_imprime(ts);
                                erro(ERRO_IDENT_DUPLICADO);
                            }
                                
                            break;
                        case CAT_FUNC:
                            nl2 = s2->func->nivel_lexico;
                            
                            if ((nl1+1==nl2) || (nl1==nl2)) { // FUNC 1 nivel maior ou igual que VS
                                debug_print("Identificador duplicado=[%s]\n",ident1);
                                if (DEBUG)
                                    TS_imprime(ts);
                                erro(ERRO_IDENT_DUPLICADO);
                            }
                            break;
                        case CAT_TYPE:
                            debug_print("Identificador duplicado=[%s]\n",ident1);
                            if (DEBUG)
                                TS_imprime(ts);
                            erro(ERRO_IDENT_DUPLICADO);
                            break;
                        case CAT_ROT:
                            break;
                    }
                    break;
                case CAT_PF: // Igual VS
                    nl1 = s->pf->nivel_lexico;
                    switch (cat2) {
                        case CAT_VS:
                            nl2 = s2->vs->nivel_lexico;
                            
                            if(nl1==nl2) { // VS mesmo nivel
                                debug_print("Identificador duplicado=[%s]\n",ident1);
                                if (DEBUG)
                                    TS_imprime(ts);
                                erro(ERRO_IDENT_DUPLICADO);
                            }
                            
                            break;
                        case CAT_PF:
                            nl2 = s2->pf->nivel_lexico;
                            
                            if(nl1==nl2) { // PF mesmo nivel (quer dizer que sao da mesma subrot)
                                debug_print("Identificador duplicado=[%s]\n",ident1);
                                if (DEBUG)
                                    TS_imprime(ts);
                                erro(ERRO_IDENT_DUPLICADO);
                            }
                            
                            break;
                        case CAT_PROC:
                            nl2 = s2->proc->nivel_lexico;
                            
                            if (nl1+1==nl2) { // PROC 1 nivel maior que PF
                                debug_print("Identificador duplicado=[%s]\n",ident1);
                                if (DEBUG)
                                    TS_imprime(ts);
                                erro(ERRO_IDENT_DUPLICADO);
                            }
                            
                            break;
                        case CAT_FUNC:
                            nl2 = s2->func->nivel_lexico;
                            
                            if ((nl1+1==nl2) || (nl1==nl2)) { // FUNC 1 nivel maior ou igual que PF
                                debug_print("Identificador duplicado=[%s]\n",ident1);
                                if (DEBUG)
                                    TS_imprime(ts);
                                erro(ERRO_IDENT_DUPLICADO);
                            }
                            
                            break;
                        case CAT_TYPE:
                            debug_print("Identificador duplicado=[%s]\n",ident1);
                            if (DEBUG)
                                TS_imprime(ts);
                            erro(ERRO_IDENT_DUPLICADO);
                            break;
                        case CAT_ROT:
                            break;
                    }
                    break;
                case CAT_PROC:
                    nl1 = s->proc->nivel_lexico;
                    switch (cat2) {
                        case CAT_VS:
                            nl2 = s2->vs->nivel_lexico;
                            
                            if (nl1-1==nl2) { // PROC 1 nivel maior que VS
                                debug_print("Identificador duplicado=[%s]\n",ident1);
                                if (DEBUG)
                                    TS_imprime(ts);
                                erro(ERRO_IDENT_DUPLICADO);
                            }
                            
                            break;
                        case CAT_PF:
                            nl2 = s2->pf->nivel_lexico;
                            
                            if (nl1-1==nl2) { // PROC 1 nivel maior que PF
                                debug_print("Identificador duplicado=[%s]\n",ident1);
                                if (DEBUG)
                                    TS_imprime(ts);
                                erro(ERRO_IDENT_DUPLICADO);
                            }
                            
                            break;
                        case CAT_PROC:
                            nl2 = s2->proc->nivel_lexico;
                            
                            if ((nl1-1==nl2) || (nl1==nl2)) { // PROC 1 nivel maior ou igual que PROC
                                debug_print("Identificador duplicado=[%s]\n",ident1);
                                if (DEBUG)
                                    TS_imprime(ts);
                                erro(ERRO_IDENT_DUPLICADO);
                            }
                            
                            break;
                        case CAT_FUNC:
                            nl2 = s2->func->nivel_lexico;
                            
                            if (nl1-1==nl2) { // PROC 1 nivel maior que FUNC
                                debug_print("Identificador duplicado=[%s]\n",ident1);
                                if (DEBUG)
                                    TS_imprime(ts);
                                erro(ERRO_IDENT_DUPLICADO);
                            }
                            
                            break;
                        case CAT_TYPE:
                            debug_print("Identificador duplicado=[%s]\n",ident1);
                            if (DEBUG)
                                TS_imprime(ts);
                            erro(ERRO_IDENT_DUPLICADO);
                            break;
                        case CAT_ROT:
                            break;
                    }
                    break;
                case CAT_FUNC:
                    nl1 = s->func->nivel_lexico;
                    switch (cat2) {
                        case CAT_VS:
                            nl2 = s2->vs->nivel_lexico;
                            
                            if ((nl1-1==nl2) || (nl1==nl2)) { // FUNC 1 nivel maior ou igual que VS
                                debug_print("Identificador duplicado=[%s]\n",ident1);
                                if (DEBUG)
                                    TS_imprime(ts);
                                erro(ERRO_IDENT_DUPLICADO);
                            }
                            
                            break;
                        case CAT_PF:
                            nl2 = s2->pf->nivel_lexico;
                            
                            if ((nl1-1==nl2) || (nl1==nl2)) { // FUNC 1 nivel maior ou igual que VS
                                debug_print("Identificador duplicado=[%s]\n",ident1);
                                if (DEBUG)
                                    TS_imprime(ts);
                                erro(ERRO_IDENT_DUPLICADO);
                            }
                            
                            break;
                        case CAT_PROC:
                            nl2 = s2->proc->nivel_lexico;
                            
                            if (nl1+1==nl2) { // PROC 1 nivel maior que FUNC
                                debug_print("Identificador duplicado=[%s]\n",ident1);
                                if (DEBUG)
                                    TS_imprime(ts);
                                erro(ERRO_IDENT_DUPLICADO);
                            }
                            
                            break;
                        case CAT_FUNC:
                            nl2 = s2->func->nivel_lexico;
                            
                            if ((nl1-1==nl2) || (nl1==nl2)) { // FUNC 1 nivel maior ou igual que VS
                                debug_print("Identificador duplicado=[%s]\n",ident1);
                                if (DEBUG)
                                    TS_imprime(ts);
                                erro(ERRO_IDENT_DUPLICADO);
                            }
                            
                            break;
                        case CAT_TYPE:
                            debug_print("Identificador duplicado=[%s]\n",ident1);
                            if (DEBUG)
                                TS_imprime(ts);
                            erro(ERRO_IDENT_DUPLICADO);
                            break;
                        case CAT_ROT:
                            break;
                    }
                    break;
                case CAT_ROT:
                    nl1 = s->rot->nivel_lexico;
                    if ((s2->rot->categoria==CAT_ROT) && (s2->rot->nivel_lexico == nl1)){
                        debug_print("Identificador duplicado=[%s]\n",ident1);
                        if (DEBUG)
                            TS_imprime(ts);
                        erro(ERRO_IDENT_DUPLICADO);
                    }
                    break;
                case CAT_TYPE:
                    debug_print("Identificador duplicado=[%s]\n",ident1);
                    if (DEBUG)
                        TS_imprime(ts);
                    erro(ERRO_IDENT_DUPLICADO);
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
        if (s->base->categoria == CAT_PF){
            
            p = (param *) malloc(sizeof(param));
            s->pf->deslocamento = -4-i;
            p->tipo = s->pf->tipo;
            p->passagem = s->pf->passagem;
            if (subrot->base->categoria == CAT_PROC){
                empilha( (void *) p, subrot->proc->params );
                subrot->proc->n_params++;
            }
            else if (subrot->base->categoria == CAT_FUNC){
                empilha( (void *) p, subrot->func->params );
                subrot->func->n_params++;
            }
            s_str = TS_simbolo2str(s);
            debug_print("Param [%s] empilhado.\n",s_str);
            free (s_str);
            i++;
        }
        else {
            break;
        }
        n = proximo_no_pilha(n);
    }
    if (subrot->base->categoria == CAT_FUNC) {
        subrot->func->deslocamento = -4-i;
        debug_print("Deslocamento da funcao atualizado para %d.\n", subrot->func->deslocamento);
    }
    debug_print("%d parametros atualizados.\n", i);
    if (DEBUG)
        TS_imprime(ts);
}

tipo_simbolo *TS_busca_subrotina(pilha ts){
    if (!ts)
        return NULL;
        
    no_pilha n = topo(ts);
    tipo_simbolo *subrot;
    
    while (n){ // acha simb da subrot
        subrot = (tipo_simbolo *) conteudo_pilha(n);
        if (subrot->base->categoria == CAT_PROC || subrot->base->categoria == CAT_FUNC){
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
        if ((strncmp(ident, subrot->base->identificador, strlen(ident))==0) && (subrot->base->categoria == CAT_PROC)) {
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
        if ((strncmp(ident, subrot->base->identificador, strlen(ident))==0) && (subrot->base->categoria == CAT_FUNC)) {
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
    tipo_simbolo *s;
    char *s_str;
    categorias cat = simb_rtpr->base->categoria;
    
    if (cat==CAT_PROC) {
        debug_print("Retornando de Proc. %s, nivel lexico %d.\n", simb_rtpr->proc->identificador, simb_rtpr->proc->nivel_lexico);
    }
    else {
        debug_print("Retornando de Func. %s, nivel lexico %d.\n", simb_rtpr->func->identificador, simb_rtpr->func->nivel_lexico);
    }
    
    n = topo(ts);
    while (n){
        s = (tipo_simbolo *) conteudo_pilha(n);
        
        if (( (s->base->categoria == CAT_PF) && (s->pf->nivel_lexico==nivel_lexico) )
        || ((s->base->categoria == CAT_PROC) && (s->proc->nivel_lexico>nivel_lexico))
        || ((s->base->categoria == CAT_FUNC) && (s->func->nivel_lexico>nivel_lexico)))
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