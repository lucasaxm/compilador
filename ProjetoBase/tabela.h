#include "pilha.h"

#define TAM_ROT 3

typedef enum categorias {
  var_simples, procedimento, param_formal, funcao, rotulo
} categorias;

typedef enum tipos_passagem {
  valor, referencia
} tipos_passagem;

typedef enum tipos {
  tint, tboolean, tunknown
} tipos;

typedef struct param {
  tipos tipo;
  tipos_passagem passagem;
} param;


typedef struct simbolo_base {
    categorias categoria;
    char *identificador;
} simbolo_base;

typedef struct simbolo_vs {
  categorias categoria;
  char *identificador;
  int nivel_lexico;
  int deslocamento;
  tipos tipo;
} simbolo_vs;

typedef struct simbolo_proc {
  categorias categoria;
  char *identificador;
  int nivel_lexico;
  char rotulo[4];
  int n_params;
  param *params;
} simbolo_proc;

typedef struct simbolo_pf {
  categorias categoria;
  char *identificador;
  int nivel_lexico;
  int deslocamento;
  tipos tipo;
  tipos_passagem passagem;
} simbolo_pf;

typedef struct simbolo_func {
  categorias categoria;
  char *identificador;
  int nivel_lexico;
  int deslocamento;
  tipos tipo;
  char rotulo[4];
  int n_params;
  param *params;
} simbolo_func;

typedef struct simbolo_rot {
    categorias categoria;
    char *identificador;
    char rotulo[4];
    int nivel_lexico;
} simbolo_rot;

typedef union simbolo {
    simbolo_base base;
    simbolo_vs vs;
    simbolo_proc proc;
    simbolo_pf pf;
    simbolo_func func;
    simbolo_rot rot;
} tipo_simbolo;

simbolo_vs TS_constroi_simbolo_vs(char *identificador, int nivel_lexico, int deslocamento, tipos tipo);

simbolo_proc TS_constroi_simbolo_proc(char *identificador, categorias categoria, int nivel_lexico, char *rotulo, int n_params, param *params);

simbolo_pf TS_constroi_simbolo_pf(char *identificador, int nivel_lexico, int deslocamento, tipos tipo, tipos_passagem passagem);

simbolo_func TS_constroi_simbolo_func(char *identificador, categorias categoria, int nivel_lexico, int deslocamento, tipos tipo, char *rotulo, int n_params, param *params);

simbolo_rot TS_constroi_simbolo_rot(char *identificador, int nivel_lexico, char *rotulo);

void TS_imprime(pilha ts);

void TS_simbolo2str(tipo_simbolo *s, char *str);

void TS_atualiza_tipos();

tipo_simbolo *TS_busca(char *identificador, pilha ts);
