#include <malloc.h>
#include "fila.h"

//---------------------------------------------------------------------------
// nó de fila encadeada cujo conteúdo é um void *

struct no {

  void *conteudo;
  no proximo;
  // no anterior;
};
//---------------------------------------------------------------------------
// fila encadeada

struct fila {
  
  unsigned int tamanho;
  no inicio;
  no final;
};
//---------------------------------------------------------------------------
// devolve o número de nós da fila f

unsigned int tamanho_fila(fila f) { return f->tamanho; }

//---------------------------------------------------------------------------
// devolve o inicio da fila f,
//      ou NULL, se l é vazia

no inicio(fila f) { return f->inicio; }

//---------------------------------------------------------------------------
// devolve o inicio da fila f,
//      ou NULL, se l é vazia

no final(fila f) { return f->final; }

//---------------------------------------------------------------------------
// devolve o conteúdo do nó n
//      ou NULL se n = NULL 

void *conteudo(no n) { return n->conteudo; }

//---------------------------------------------------------------------------
// devolve o sucessor do nó n,
//      ou NULL, se n for o último nó da fila
no proximo_no(no n) { return n->proximo; }

//---------------------------------------------------------------------------
// devolve o sucessor do nó n,
//      ou NULL, se n for o último nó da fila
// no anterior_no(no n) { return n->anterior; }

//---------------------------------------------------------------------------
// cria uma fila vazia e a devolve
//
// devolve NULL em caso de falha

fila constroi_fila(void) {

  fila f = malloc(sizeof(struct fila));

  if ( ! f ) 
    return NULL;

  f->inicio = NULL;
  f->final = NULL;
  f->tamanho = 0;

  return f;
}
//---------------------------------------------------------------------------
// desaloca a fila f e todos os seus nós
// 
// se destroi != NULL invoca
//
//     destroi(conteudo(n)) 
//
// para cada nó n da fila. 
//
// devolve 1 em caso de sucesso,
//      ou 0 em caso de falha

int destroi_fila(fila f, int destroi(void *)) { 
  
  no n;
  int ok=1;

  while ( (n = inicio(f)) ) {
    
    f->inicio = proximo_no(n);

    if ( destroi )
      ok &= destroi(conteudo(n));

    free(n);
  }

  free(f);

  return ok;
}
//---------------------------------------------------------------------------
// insere um novo nó na fila f cujo conteúdo é p
//
// devolve o no recém-criado 
//      ou NULL em caso de falha

no enfileira(void *conteudo, fila f) { 

  no novo = malloc(sizeof(struct no));

  if ( ! novo ) 
    return NULL;

  f->final->proximo = novo;
  novo->conteudo = conteudo;
  novo->proximo = NULL;
  ++f->tamanho;
  
  return f->final = novo;
}

//---------------------------------------------------------------------------
// retorna e remove o conteudo do inicio da fila
//
// devolve o inicio da fila
//      ou NULL em caso de fila vazia
void *desenfileira(fila f) {
  no ex_inicio;
  void *cont;
  
  if (f->tamanho<=0)
    return NULL;
    
  ex_inicio = inicio(f);
  f->inicio = proximo_no(ex_inicio);
  f->tamanho--;
  
  cont = conteudo(ex_inicio);
  free(ex_inicio);
  
  return cont;
}

void imprime(fila f, void conteudo2str(void *, char *)) {
  no n;
  void *c;
  n = inicio(f);
  char *str_conteudo;
  
  while ( n ) {
    c = (void *) conteudo(n);
    conteudo2str(c, str_conteudo);
    printf("%s\n",str_conteudo);
    n = proximo_no(n);
  }
}
