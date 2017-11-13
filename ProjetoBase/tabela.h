#include "pilha.h"

#define TAM_ROT 3

typedef enum erros {
    ERRO_ATRIB,
    ERRO_TPARAM,
    ERRO_NPARAM,
    ERRO_IDENT_DECL,
    ERRO_VS_NDECL,
    ERRO_PROC_NDECL,
    ERRO_FUNC_NDECL,
    ERRO_ROT_NDECL,
    ERRO_TINCOMPATIVEL
} erros;

typedef enum categorias {
  CAT_VS, CAT_PROC, CAT_PF, CAT_FUNC, CAT_ROT
} categorias;

typedef enum tipos_passagem {
  PASS_VAL, PASS_REF
} tipos_passagem;

typedef enum tipos {
  TIPO_INT, TIPO_BOOL, TIPO_UNKNOWN
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

simbolo_proc TS_constroi_simbolo_proc(char *identificador, int nivel_lexico, char *rotulo, int n_params, param *params);

simbolo_pf TS_constroi_simbolo_pf(char *identificador, int nivel_lexico, int deslocamento, tipos tipo, tipos_passagem passagem);

simbolo_func TS_constroi_simbolo_func(char *identificador, int nivel_lexico, int deslocamento, tipos tipo, char *rotulo, int n_params, param *params);

simbolo_rot TS_constroi_simbolo_rot(char *identificador, int nivel_lexico, char *rotulo);

void TS_imprime(pilha ts);

void TS_simbolo2str(void *s, char *str);

int TS_atualiza_tipos(tipos tipo, categorias cat, pilha ts);

tipo_simbolo *TS_busca(char *identificador, pilha ts);

int TS_tamanho(pilha ts);

int TS_remove_vs(int nivel_lexico, pilha ts);