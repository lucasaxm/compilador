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

char token[TAM_TOKEN];

int num_vars;   // numero de vars de subrot
int num_params_subrot; // numero de params na decl de subrot
int nivel_lexico;   // nivel lexico atual
pilha tabela_simbolos;  // tabela de simbolos
pilha pilha_rotulos_dsvs; // pilha de rotulos que desviam da decl. de subrotinas
tipo_simbolo *s;    // simbolo usado em diversos lugares no codigo
taux_atrib aux_atrib;   // estrutura que auxilia no parsing de atribuicao
tipos_passagem aux_passagem;    // auxiliar para adicionar tipo de passagem de params
categorias aux_categoria;    // auxiliar para tipo categoria (usado em atualizacao de tipo)
int d_rot;  // inteiro usado para gerar proximo rotulo
char s_rot[TAM_ROT+1];  // string que representa o proximo rotulo
char *main_rot; // rotulo que representa programa principal
int has_label;  // flag para indicar que proximo comando possui um label
int conta_tipo; // contador de simbolos com mesmo tipo que devem ser atualizados
fila parametros; // fila de parametros usada no geraCodigo