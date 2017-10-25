#include <malloc.h>
#include "pilha.h"

//---------------------------------------------------------------------------
// nó de pilha encadeada cujo conteúdo é um void *

struct no {

  void *conteudo;
  no proximo;
};
//---------------------------------------------------------------------------
// pilha encadeada

struct pilha {
  
  unsigned int tamanho;
  no topo;
};
//---------------------------------------------------------------------------
// devolve o número de nós da pilha p

unsigned int tamanho_pilha(pilha p) { return p->tamanho; }

//---------------------------------------------------------------------------
// devolve o topo da pilha p,
//      ou NULL, se l é vazia

no topo(pilha p) { return p->topo; }

//---------------------------------------------------------------------------
// devolve o conteúdo do nó n
//      ou NULL se n = NULL 

void *conteudo(no n) { return n->conteudo; }

//---------------------------------------------------------------------------
// devolve o sucessor do nó n,
//      ou NULL, se n for o último nó da pilha

no proximo_no(no n) { return n->proximo; }

//---------------------------------------------------------------------------
// cria uma pilha vazia e a devolve
//
// devolve NULL em caso de falha

pilha constroi_pilha(void) {

  pilha p = malloc(sizeof(struct pilha));

  if ( ! p ) 
    return NULL;

  p->topo = NULL;
  p->tamanho = 0;

  return p;
}
//---------------------------------------------------------------------------
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

int destroi_pilha(pilha p, int destroi(void *)) { 
  
  no n;
  int ok=1;

  while ( (n = topo(p)) ) {
    
    p->topo = proximo_no(n);

    if ( destroi )
      ok &= destroi(conteudo(n));

    free(n);
  }

  free(p);

  return ok;
}
//---------------------------------------------------------------------------
// insere um novo nó na pilha p cujo conteúdo é p
//
// devolve o no recém-criado 
//      ou NULL em caso de falha

no empilha(void *conteudo, pilha p) { 

  no novo = malloc(sizeof(struct no));

  if ( ! novo ) 
    return NULL;

  novo->conteudo = conteudo;
  novo->proximo = topo(p);
  ++p->tamanho;
  
  return p->topo = novo;
}
