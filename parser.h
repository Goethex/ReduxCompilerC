#ifndef PARSER_H
#define PARSER_H

#include "token.h"

typedef enum { TYPE_INT, TYPE_FLOAT } VarType;

typedef struct {
    char name[MAX_ID_LEN];
    VarType type;
} Symbol;

typedef enum { NODE_DECL, NODE_EXPR, NODE_TERM, NODE_IF, NODE_BLOCK } NodeType;

typedef struct Node {
    NodeType node_type;
    Token token;
    VarType var_type; // Para declaraciones
    struct Node *left, *right, *else_branch; // left: condición, right: cuerpo if/block, else_branch: cuerpo else/block
    struct Node *next; // Para lista de statements dentro de un bloque o programa
} Node;

Node *parse_syntactic(Token *tokens, int *token_index);
void analyze_semantic(Node *ast, Symbol *symbol_table, int *symbol_count);
void free_ast(Node *node);

#endif