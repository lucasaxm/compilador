#ifndef _fila_H
#define _fila_H

//-----------------------------------------------------------------------------
// (apontador para) fila encadeada

typedef struct fila *fila;

//-----------------------------------------------------------------------------
// (apontador para) nó da fila encadeada cujo conteúdo é um void *

typedef struct no *no;

//------------------------------------------------------------------------------
// devolve o número de nós da fila p

unsigned int tamanho_fila(fila f);

//------------------------------------------------------------------------------
// devolve o primeiro nó da fila p,
//      ou NULL, se l é vazia

no inicio(fila f);

no final(fila f);

//------------------------------------------------------------------------------
// devolve o sucessor do nó n,
//      ou NULL, se n for o último nó da fila

no proximo_no(no n);

//------------------------------------------------------------------------------
// devolve o conteúdo do nó n
//      ou NULL se n = NULL 

void *conteudo(no n);
//------------------------------------------------------------------------------
// insere um novo nó na fila p cujo conteúdo é p
//
// devolve o no recém-criado 
//      ou NULL em caso de falha

no enfileira(void *conteudo, fila f);

//---------------------------------------------------------------------------
// retorna e remove o conteudo do topo da fila
//
// devolve o topo da fila
//      ou NULL em caso de fila vazia
void *desenfileira(fila f);
    
//------------------------------------------------------------------------------
// cria uma fila vazia e a devolve
//
// devolve NULL em caso de falha

fila constroi_fila(void);
//------------------------------------------------------------------------------
// desaloca a fila p e todos os seus nós
// 
// se destroi != NULL invoca
//
//     destroi(conteudo(n)) 
//
// para cada nó n da fila. 
//
// devolve 1 em caso de sucesso,
//      ou 0 em caso de falha

int destroi_fila(fila f, int destroi(void *));

void imprime(fila f, void conteudo2str(void *, char *));



#endif
