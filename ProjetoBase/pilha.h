#ifndef _PILHA_H
#define _PILHA_H

//-----------------------------------------------------------------------------
// (apontador para) pilha encadeada

typedef struct pilha *pilha;

//-----------------------------------------------------------------------------
// (apontador para) nó da pilha encadeada cujo conteúdo é um void *

typedef struct no *no;

//------------------------------------------------------------------------------
// devolve o número de nós da pilha p

unsigned int tamanho_pilha(pilha p);

//------------------------------------------------------------------------------
// devolve o primeiro nó da pilha p,
//      ou NULL, se l é vazia

no topo(pilha p);

//------------------------------------------------------------------------------
// devolve o sucessor do nó n,
//      ou NULL, se n for o último nó da pilha

no proximo_no(no n);

//------------------------------------------------------------------------------
// devolve o conteúdo do nó n
//      ou NULL se n = NULL 

void *conteudo(no n);
//------------------------------------------------------------------------------
// insere um novo nó na pilha p cujo conteúdo é p
//
// devolve o no recém-criado 
//      ou NULL em caso de falha

no empilha(void *conteudo, pilha p);
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

#endif
