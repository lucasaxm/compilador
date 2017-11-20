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
#define TAM_TEXTO 256


/* -------------------------------------------------------------------
 * variáveis globais
 * ------------------------------------------------------------------- */

extern char token[TAM_TOKEN];
extern char texto[TAM_TEXTO];
extern int nivel_lexico;
extern int desloc;
extern int nl;

typedef struct aux_atrib {
    tipo_simbolo *s;
    tipo_simbolo *tipo;
} taux_atrib;

typedef struct schsubrot {
    int params_chamados;
    tipo_simbolo *subrot;
} tchsubrot;

char token[TAM_TOKEN];
char texto[TAM_TEXTO];
char ident[TAM_TOKEN]; // var aux para armazenar token

int num_vars;   // numero de vars de subrot
int num_params_decl_subrot; // numero de params na decl/chamada de subrot
int nivel_lexico;   // nivel lexico atual
pilha tabela_simbolos;  // tabela de simbolos
pilha pilha_rotulos_subrot; // pilha de rotulos que desviam da decl. de subrotinas
pilha pilha_rotulos_cond; // pilha de rotulos que sao usados no comando condicional
pilha pilha_rotulos_repet; // pilha de rotulos que sao usados no comando repetitivo
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
pilha pilha_decl_subrot;    // pilha de decl de subrotinas, topo=subrotina mais interna
pilha pilha_cham_subrot;    // pilha de chamadas de subrotinas, topo=ultima subrot chamada
int flag_var; // flag que indica se expressao eh uma variavel

int n_digitos(int n);
void erro(erros e);