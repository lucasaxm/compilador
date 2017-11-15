#ifndef _PILHA_H
#define _PILHA_H

//-----------------------------------------------------------------------------
// (apontador para) pilha encadeada

typedef struct pilha *pilha;

//-----------------------------------------------------------------------------
// (apontador para) nó da pilha encadeada cujo conteúdo é um void *

typedef struct no_pilha *no_pilha;

//------------------------------------------------------------------------------
// devolve o número de nós da pilha p

unsigned int tamanho_pilha(pilha p);

//------------------------------------------------------------------------------
// devolve o primeiro nó da pilha p,
//      ou NULL, se l é vazia

no_pilha topo(pilha p);

//------------------------------------------------------------------------------
// devolve o sucessor do nó n,
//      ou NULL, se n for o último nó da pilha

no_pilha proximo_no_pilha(no_pilha n);

//------------------------------------------------------------------------------
// devolve o conteúdo do nó n
//      ou NULL se n = NULL 

void *conteudo_pilha(no_pilha n);
//------------------------------------------------------------------------------
// insere um novo nó na pilha p cujo conteúdo é p
//
// devolve o no_pilha recém-criado 
//      ou NULL em caso de falha

no_pilha empilha(void *conteudo, pilha p);

//---------------------------------------------------------------------------
// retorna e remove o conteudo do topo da pilha
//
// devolve o topo da pilha
//      ou NULL em caso de pilha vazia
void *desempilha(pilha p);
    
//------------------------------------------------------------------------------
// cria uma pilha vazia e a devolve
//
// devolve NULL em caso de falha

pilha constroi_pilha(void);
//------------------------------------------------------------------------------
// desaloca a pilha p e todos os seus nós
// 
// se destroi != NULL invoca
//
//     destroi(conteudo(n)) 
//
// para cada nó n da pilha. 
//
// devolve 1 em caso de sucesso,
//      ou 0 em caso de falha

int destroi_pilha(pilha p, int destroi(void *));

void imprime_pilha(pilha p, char *conteudo2str(void *));


#endif
