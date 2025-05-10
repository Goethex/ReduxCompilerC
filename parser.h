#ifndef PARSER_H
#define PARSER_H

#include "token.h"

#define MAX_SYMBOLS 100
#define MAX_SCOPES 50

typedef enum { TYPE_INT, TYPE_FLOAT } VarType;

typedef struct {
    char name[MAX_ID_LEN];
    VarType type;
    int line; // Nuevo campo para la línea de declaración
} Symbol;

typedef struct {
    Symbol symbols[MAX_SYMBOLS];
    int symbol_count;
} Scope;

typedef struct {
    Scope scopes[MAX_SCOPES];
    int scope_count;
} SymbolTable;

typedef enum { NODE_DECL, NODE_EXPR, NODE_TERM, NODE_IF, NODE_BLOCK } NodeType;

typedef struct Node {
    NodeType node_type;
    Token token;
    VarType var_type;
    struct Node *left, *right, *else_branch;
    struct Node *next;
} Node;

Node *parse_syntactic(Token *tokens, int *token_index);
void analyze_semantic(Node *ast, SymbolTable *symbol_table);
void free_ast(Node *node);

#endif