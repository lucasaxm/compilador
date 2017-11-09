/* -------------------------------------------------------------------
 *            Arquivo: compilaodr.h
 * -------------------------------------------------------------------
 *              Autor: Bruno Muller Junior
 *               Data: 08/2007
 *      Atualizado em: [15/03/2012, 08h:22m]
 *
 * -------------------------------------------------------------------
 *
 * Tipos, protótipos e vaiáveis globais do compilador
 *
 * ------------------------------------------------------------------- */

#include "tabela.h"

#define DEBUG 1

#define debug_print(fmt, ...) \
        do { \
          if (DEBUG) \
            fprintf(stderr, "DEBUG | %s:%d | %s | " fmt, __FILE__, __LINE__, __func__, __VA_ARGS__); \
        } while (0)

#define TAM_TOKEN 16


/* -------------------------------------------------------------------
 * variáveis globais
 * ------------------------------------------------------------------- */

extern char token[TAM_TOKEN];
extern int nivel_lexico;
extern int desloc;
extern int nl;

typedef struct aux_atrib {
    tipo_simbolo *s;
    tipos tipo;
} taux_atrib;

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

char token[TAM_TOKEN];

int num_vars;
int nivel_lexico;
pilha tabela_simbolos;
tipo_simbolo *s;
taux_atrib aux_atrib;
int d_rot;
char s_rot[TAM_ROT+1];
int has_label;
char *parametros[3];