#include <malloc.h>
#include "fila.h"

//---------------------------------------------------------------------------
// nó de fila encadeada cujo conteúdo é um void *

struct no_fila {

  void *conteudo;
  no_fila proximo;
  // no_fila anterior;
};
//---------------------------------------------------------------------------
// fila encadeada

struct fila {
  
  unsigned int tamanho;
  no_fila inicio;
  no_fila final;
};
//---------------------------------------------------------------------------
// devolve o número de nós da fila f

unsigned int tamanho_fila(fila f) { return f->tamanho; }

//---------------------------------------------------------------------------
// devolve o inicio da fila f,
//      ou NULL, se l é vazia

no_fila inicio(fila f) { return f->inicio; }

//---------------------------------------------------------------------------
// devolve o inicio da fila f,
//      ou NULL, se l é vazia

no_fila final(fila f) { return f->final; }

//---------------------------------------------------------------------------
// devolve o conteúdo do nó n
//      ou NULL se n = NULL 

void *conteudo_fila(no_fila n) { return n->conteudo; }

//---------------------------------------------------------------------------
// devolve o sucessor do nó n,
//      ou NULL, se n for o último nó da fila
no_fila proximo_no_fila(no_fila n) { return n->proximo; }

//---------------------------------------------------------------------------
// devolve o sucessor do nó n,
//      ou NULL, se n for o último nó da fila
// no_fila anterior_no(no_fila n) { return n->anterior; }

//---------------------------------------------------------------------------
// cria uma fila vazia e a devolve
//
// devolve NULL em caso de falha

fila constroi_fila(void) {

  fila f = malloc(sizeof(struct fila));

  if ( ! f ) 
    return NULL;

  f->inicio = NULL;
  f->final = f->inicio;
  f->tamanho = 0;

  return f;
}
//---------------------------------------------------------------------------
// desaloca a fila f e todos os seus nós
// 
// se destroi != NULL invoca
//
//     destroi(conteudo_fila(n)) 
//
// para cada nó n da fila. 
//
// devolve 1 em caso de sucesso,
//      ou 0 em caso de falha

int destroi_fila(fila f, int destroi(void *)) { 
  
  no_fila n;
  int ok=1;

  while ( (n = inicio(f)) ) {
    
    f->inicio = proximo_no_fila(n);

    if ( destroi )
      ok &= destroi(conteudo_fila(n));

    free(n);
  }

  free(f);

  return ok;
}
//---------------------------------------------------------------------------
// insere um novo nó na fila f cujo conteúdo é p
//
// devolve o no_fila recém-criado 
//      ou NULL em caso de falha

no_fila enfileira(void *conteudo, fila f) { 

  no_fila novo = malloc(sizeof(struct no_fila));

  if ( ! novo ) 
    return NULL;
  
  if (tamanho_fila(f)==0)
    f->inicio = novo;
  else
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
  no_fila ex_inicio;
  void *cont;
  
  if (f->tamanho<=0)
    return NULL;
    
  ex_inicio = inicio(f);
  f->inicio = proximo_no_fila(ex_inicio);
  f->tamanho--;
  
  cont = conteudo_fila(ex_inicio);
  free(ex_inicio);
  
  return cont;
}

void imprime_fila(fila f, char *conteudo2str(void *)) {
  no_fila n;
  void *c;
  n = inicio(f);
  char *str_conteudo;
  
  while ( n ) {
    c = (void *) conteudo_fila(n);
    if (conteudo2str)
      str_conteudo = conteudo2str(c);
    else
      str_conteudo = (char *) c;
    printf("%s\n",str_conteudo);
    n = proximo_no_fila(n);
  }
}
