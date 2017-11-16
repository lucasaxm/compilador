#include <malloc.h>
#include "pilha.h"

//---------------------------------------------------------------------------
// nó de pilha encadeada cujo conteúdo é um void *

struct no_pilha {

  void *conteudo;
  no_pilha proximo;
};
//---------------------------------------------------------------------------
// pilha encadeada

struct pilha {
  
  unsigned int tamanho;
  no_pilha topo;
};
//---------------------------------------------------------------------------
// devolve o número de nós da pilha p

unsigned int tamanho_pilha(pilha p) { return p->tamanho; }

//---------------------------------------------------------------------------
// devolve o topo da pilha p,
//      ou NULL, se l é vazia

no_pilha topo(pilha p) { return p->topo; }

//---------------------------------------------------------------------------
// devolve o conteúdo do nó n
//      ou NULL se n = NULL 

void *conteudo_pilha(no_pilha n) { return n ? n->conteudo : NULL; }

//---------------------------------------------------------------------------
// devolve o sucessor do nó n,
//      ou NULL, se n for o último nó da pilha

no_pilha proximo_no_pilha(no_pilha n) { return n->proximo; }

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
//     destroi(conteudo_pilha(n)) 
//
// para cada nó n da pilha. 
//
// devolve 1 em caso de sucesso,
//      ou 0 em caso de falha

int destroi_pilha(pilha p, int destroi(void *)) { 
  
  no_pilha n;
  int ok=1;

  while ( (n = topo(p)) ) {
    
    p->topo = proximo_no_pilha(n);

    if ( destroi )
      ok &= destroi(conteudo_pilha(n));

    free(n);
  }

  free(p);

  return ok;
}
//---------------------------------------------------------------------------
// insere um novo nó na pilha p cujo conteúdo é p
//
// devolve o no_pilha recém-criado 
//      ou NULL em caso de falha

no_pilha empilha(void *conteudo, pilha p) { 

  no_pilha novo = malloc(sizeof(struct no_pilha));

  if ( ! novo ) 
    return NULL;

  novo->conteudo = conteudo;
  novo->proximo = topo(p);
  ++p->tamanho;
  
  return p->topo = novo;
}

//---------------------------------------------------------------------------
// retorna e remove o conteudo do topo da pilha
//
// devolve o topo da pilha
//      ou NULL em caso de pilha vazia
void *desempilha(pilha p) {
  no_pilha ex_topo;
  void *cont;
  
  if (p->tamanho<=0)
    return NULL;
    
  ex_topo = topo(p);
  p->topo = proximo_no_pilha(ex_topo);
  p->tamanho--;
  
  cont = conteudo_pilha(ex_topo);
  free(ex_topo);
  
  return cont;
}

void imprime_pilha(pilha p, char *conteudo2str(void *)) {
  no_pilha n;
  void *c;
  n = topo(p);
  char *str_conteudo;
  
  while ( n ) {
    c = (void *) conteudo_pilha(n);
    if (conteudo2str)
      str_conteudo = conteudo2str(c);
    else
      str_conteudo = (char *) c;
    printf("%s\n",str_conteudo);
    n = proximo_no_pilha(n);
  }
}

void *busca_indice_pilha(int indice, pilha p){
  int i;
  if ( (!p) || (tamanho_pilha(p)<=0) ) return NULL;
  
  no_pilha n = topo(p);
  for (i = 0; (i < indice) && n; i++) {
    n = proximo_no_pilha(n);
  }
  if (n)
    return conteudo_pilha(n);
  else
    return NULL;
}

void *remove_indice_pilha(int indice, pilha p){
  if (indice==0)
    return desempilha(p);
    
  int i;
  void *c;
  
  no_pilha n_anterior = topo(p);
  no_pilha n = proximo_no_pilha(n_anterior);
  
  for (i = 1; (i < indice) && n; i++) {
    n_anterior = n;
    n = proximo_no_pilha(n_anterior);
  }
  if (!n)
    return NULL;
    
  c = conteudo_pilha(n);
  n_anterior->proximo = proximo_no_pilha(n);
  free(n);
  
  return c;
}