ch_func:
    {
        num_params_subrot=0;
        tipo_simbolo *subrot = TS_busca_funcao(ident, tabela_simbolos);
        if (!subrot) {
            debug_print("Funcao com ident [%s] nao declarada.\n", ident);
            erro(ERRO_FUNC_NDECL);
        }
        empilha(subrot, pilha_cham_subrot);
        char *subrot_srt = TS_simbolo2str(subrot);
        debug_print("Chamando subrotina [%s]\n", subrot_srt);
        free (subrot_srt);
    }
    passa_params_func
    {
        tipo_simbolo *subrot = desempilha(pilha_cham_subrot);
        if(num_params_subrot < subrot->func.n_params)
            erro(ERRO_NPARAM);
        enfileira_param_string(subrot->func.rotulo);
        enfileira_param_int(nivel_lexico);
        geraCodigo(NULL, "CHPR");
        $$ = subrot->func.tipo;
    }
;