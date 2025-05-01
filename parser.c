#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

// Prototipos de funciones
static Node *parse_statement(Token *tokens, int *token_index);
static Node *parse_block(Token *tokens, int *token_index);

static void error(const char *msg, int line, int column, const char *type) {
    printf("%s Error en linea %d, columna %d: %s\n", type, line, column, msg);
    exit(1);
}

static Token current_token(Token *tokens, int *token_index) {
    return tokens[*token_index];
}

static void advance(int *token_index) {
    (*token_index)++;
}

static VarType get_type(Token token) {
    if (token.type == TOKEN_INT) return TYPE_INT;
    if (token.type == TOKEN_FLOAT) return TYPE_FLOAT;
    error("Tipo esperado", token.line, token.column, "Sintactico");
    return TYPE_INT; // No alcanzado
}

static Node *new_node(NodeType node_type, Token token) {
    Node *node = (Node *)malloc(sizeof(Node));
    node->node_type = node_type;
    node->token = token;
    node->var_type = TYPE_INT; // Valor por defecto
    node->left = node->right = node->else_branch = node->next = NULL;
    return node;
}

static Node *parse_term(Token *tokens, int *token_index) {
    Token token = current_token(tokens, token_index);
    if (token.type == TOKEN_ID || token.type == TOKEN_NUMBER) {
        advance(token_index);
        return new_node(NODE_TERM, token);
    }
    error("Identificador o numero esperado", token.line, token.column, "Sintactico");
    return NULL;
}

static Node *parse_expr(Token *tokens, int *token_index) {
    Node *left = parse_term(tokens, token_index);
    while (current_token(tokens, token_index).type == TOKEN_PLUS ||
           current_token(tokens, token_index).type == TOKEN_MINUS) {
        Token op = current_token(tokens, token_index);
        advance(token_index);
        Node *right = parse_term(tokens, token_index);
        Node *node = new_node(NODE_EXPR, op);
        node->left = left;
        node->right = right;
        left = node;
    }
    return left;
}

static Node *parse_declaration(Token *tokens, int *token_index) {
    Token type_token = current_token(tokens, token_index);
    VarType var_type = get_type(type_token);
    advance(token_index);

    Token id_token = current_token(tokens, token_index);
    if (id_token.type != TOKEN_ID) error("Identificador esperado", id_token.line, id_token.column, "Sintactico");
    advance(token_index);

    Token current = current_token(tokens, token_index);
    if (current.type != TOKEN_EQUAL) {
        if (current.type == TOKEN_EQ) {
            error("Operador '==' no permitido en declaracion; se esperaba '='", current.line, current.column, "Sintactico");
        } else {
            error("= esperado", current.line, current.column, "Sintactico");
        }
    }
    advance(token_index);

    Node *expr = parse_expr(tokens, token_index);

    if (current_token(tokens, token_index).type != TOKEN_SEMICOLON) error("; esperado", current_token(tokens, token_index).line, current_token(tokens, token_index).column, "Sintactico");
    advance(token_index);

    Node *node = new_node(NODE_DECL, id_token);
    node->var_type = var_type;
    node->left = expr;
    return node;
}

static Node *parse_if_statement(Token *tokens, int *token_index) {
    Token if_token = current_token(tokens, token_index);
    if (if_token.type != TOKEN_IF) error("'if' esperado", if_token.line, if_token.column, "Sintactico");
    advance(token_index);

    if (current_token(tokens, token_index).type != TOKEN_LPAREN) error("( esperado", current_token(tokens, token_index).line, current_token(tokens, token_index).column, "Sintactico");
    advance(token_index);

    // Parsear la primera expresión
    Node *left_expr = parse_expr(tokens, token_index);

    if (current_token(tokens, token_index).type != TOKEN_EQ) error("== esperado", current_token(tokens, token_index).line, current_token(tokens, token_index).column, "Sintactico");
    Token eq_token = current_token(tokens, token_index);
    advance(token_index);

    // Parsear la segunda expresión
    Node *right_expr = parse_expr(tokens, token_index);

    if (current_token(tokens, token_index).type != TOKEN_RPAREN) error(") esperado", current_token(tokens, token_index).line, current_token(tokens, token_index).column, "Sintactico");
    advance(token_index);

    // Parsear el cuerpo del if como un bloque
    Node *body = parse_block(tokens, token_index);

    Node *node = new_node(NODE_IF, if_token);
    Node *condition = new_node(NODE_EXPR, eq_token);
    condition->left = left_expr;
    condition->right = right_expr;
    node->left = condition;
    node->right = body;

    // Verificar si hay una cláusula else
    if (current_token(tokens, token_index).type == TOKEN_ELSE) {
        advance(token_index);
        node->else_branch = parse_block(tokens, token_index);
    }

    return node;
}

static Node *parse_block(Token *tokens, int *token_index) {
    Token token = current_token(tokens, token_index);
    if (token.type != TOKEN_LBRACE) error("{ esperado", token.line, token.column, "Sintactico");
    advance(token_index);

    Node *block = new_node(NODE_BLOCK, token);
    Node *current = NULL;

    // Parsear sentencias hasta encontrar }
    while (current_token(tokens, token_index).type != TOKEN_RBRACE) {
        Token curr_token = current_token(tokens, token_index);
        if (curr_token.type != TOKEN_INT && curr_token.type != TOKEN_FLOAT && curr_token.type != TOKEN_IF) {
            error("} esperado", curr_token.line, curr_token.column, "Sintactico");
        }
        Node *stmt = parse_statement(tokens, token_index);
        if (!block->right) {
            block->right = stmt;
            current = stmt;
        } else {
            current->next = stmt;
            current = stmt;
        }
    }

    if (current_token(tokens, token_index).type != TOKEN_RBRACE) error("} esperado", current_token(tokens, token_index).line, current_token(tokens, token_index).column, "Sintactico");
    advance(token_index);

    return block;
}

static Node *parse_statement(Token *tokens, int *token_index) {
    Token token = current_token(tokens, token_index);
    if (token.type == TOKEN_INT || token.type == TOKEN_FLOAT) {
        return parse_declaration(tokens, token_index);
    } else if (token.type == TOKEN_IF) {
        return parse_if_statement(tokens, token_index);
    }
    error("Declaracion o sentencia 'if' esperada", token.line, token.column, "Sintactico");
    return NULL;
}

Node *parse_syntactic(Token *tokens, int *token_index) {
    Node *root = NULL;
    Node *current = NULL;

    while (current_token(tokens, token_index).type != TOKEN_EOF) {
        Node *stmt = parse_statement(tokens, token_index);
        if (!root) {
            root = stmt;
            current = root;
        } else {
            current->next = stmt;
            current = stmt;
        }
    }
    return root;
}

static void add_symbol(Symbol *symbol_table, int *symbol_count, char *name, VarType type, int line, int column) {
    for (int i = 0; i < *symbol_count; i++) {
        if (strcmp(symbol_table[i].name, name) == 0) {
            error("Variable ya declarada", line, column, "Semantico");
        }
    }
    strcpy(symbol_table[*symbol_count].name, name);
    symbol_table[*symbol_count].type = type;
    (*symbol_count)++;
}

static VarType lookup_symbol(Symbol *symbol_table, int symbol_count, char *name, int line, int column) {
    for (int i = 0; i < symbol_count; i++) {
        if (strcmp(symbol_table[i].name, name) == 0) {
            return symbol_table[i].type;
        }
    }
    error("Variable no declarada", line, column, "Semantico");
    return TYPE_INT; // No alcanzado
}

static void analyze_expr(Node *expr, Symbol *symbol_table, int *symbol_count) {
    if (!expr) return;

    if (expr->node_type == NODE_TERM && expr->token.type == TOKEN_ID) {
        lookup_symbol(symbol_table, *symbol_count, expr->token.lexeme, expr->token.line, expr->token.column);
    } else if (expr->node_type == NODE_EXPR) {
        if (expr->left) analyze_expr(expr->left, symbol_table, symbol_count);
        if (expr->right) analyze_expr(expr->right, symbol_table, symbol_count);
    }
}

static void analyze_node(Node *node, Symbol *symbol_table, int *symbol_count) {
    if (!node) return;
    
    if (node->node_type == NODE_DECL) {
        analyze_expr(node->left, symbol_table, symbol_count);
        add_symbol(symbol_table, symbol_count, node->token.lexeme, node->var_type, node->token.line, node->token.column);
    } else if (node->node_type == NODE_IF) {
        // Analizar ambas expresiones en la condición
        if (node->left && node->left->node_type == NODE_EXPR) {
            analyze_expr(node->left->left, symbol_table, symbol_count);
            analyze_expr(node->left->right, symbol_table, symbol_count);
        }
        // Analizar el cuerpo del if
        if (node->right && node->right->node_type == NODE_BLOCK) {
            Node *stmt = node->right->right;
            while (stmt) {
                analyze_node(stmt, symbol_table, symbol_count);
                stmt = stmt->next;
            }
        }
        // Analizar el cuerpo del else
        if (node->else_branch && node->else_branch->node_type == NODE_BLOCK) {
            Node *stmt = node->else_branch->right;
            while (stmt) {
                analyze_node(stmt, symbol_table, symbol_count);
                stmt = stmt->next;
            }
        }
    } else if (node->node_type == NODE_BLOCK) {
        // Analizar todas las sentencias del bloque
        Node *stmt = node->right;
        while (stmt) {
            analyze_node(stmt, symbol_table, symbol_count);
            stmt = stmt->next;
        }
    }
}

void analyze_semantic(Node *ast, Symbol *symbol_table, int *symbol_count) {
    Node *current = ast;
    while (current) {
        analyze_node(current, symbol_table, symbol_count);
        current = current->next;
    }
}

void free_ast(Node *node) {
    if (!node) return;
    free_ast(node->left);
    free_ast(node->right);
    free_ast(node->else_branch);
    free_ast(node->next);
    free(node);
}