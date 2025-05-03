#include <stdio.h>
#include <string.h>
#include "token.h"
#include "lexer.h"
#include "parser.h"

#define MAX_TOKENS 256

int main() {
    char code[MAX_CODE_LEN];
    printf("Ingrese el codigo fuente (termine con una linea vacia):\n");
    int code_pos = 0;
    while (1) {
        char line[256];
        if (!fgets(line, sizeof(line), stdin)) break;
        if (line[0] == '\n') break;
        strncpy(code + code_pos, line, MAX_CODE_LEN - code_pos);
        code_pos += strlen(line);
        if (code_pos >= MAX_CODE_LEN - 1) break;
    }
    code[code_pos] = '\0';

    set_input(code);

    Token tokens[MAX_TOKENS];
    int token_count = 0;
    printf("\n=== Analisis Lexico ===\n");
    while (1) {
        Token token = get_next_token();
        tokens[token_count++] = token;
        printf("Token: %-10s Lexema: %-10s Linea: %d\n",
               token.type == TOKEN_INT ? "INT" :
               token.type == TOKEN_FLOAT ? "FLOAT" :
               token.type == TOKEN_ID ? "ID" :
               token.type == TOKEN_NUMBER ? "NUMBER" :
               token.type == TOKEN_PLUS ? "PLUS" :
               token.type == TOKEN_MINUS ? "MINUS" :
               token.type == TOKEN_EQUAL ? "EQUAL" :
               token.type == TOKEN_SEMICOLON ? "SEMICOLON" :
               token.type == TOKEN_IF ? "IF" :
               token.type == TOKEN_LPAREN ? "LPAREN" :
               token.type == TOKEN_RPAREN ? "RPAREN" :
               token.type == TOKEN_EQ ? "EQ" :
               token.type == TOKEN_ELSE ? "ELSE" :
               token.type == TOKEN_LBRACE ? "LBRACE" :
               token.type == TOKEN_RBRACE ? "RBRACE" :
               token.type == TOKEN_EOF ? "EOF" : "ERROR",
               token.lexeme, token.line);
        if (token.type == TOKEN_EOF || token.type == TOKEN_ERROR || token_count >= MAX_TOKENS) break;
    }

    if (tokens[token_count-1].type == TOKEN_ERROR) {
        printf("Error lexico detectado.\n");
        return 1;
    }

    printf("\n=== Analisis Sintactico ===\n");
    int token_index = 0;
    Node *ast = parse_syntactic(tokens, &token_index);
    if (ast) {
        printf("Analisis sintactico completado con exito.\n");
    } else {
        printf("No se genero AST (codigo vacio).\n");
        return 1;
    }

    printf("\n=== Analisis Semantico ===\n");
    SymbolTable symbol_table;
    analyze_semantic(ast, &symbol_table);

    printf("\nTabla de simbolos:\n");
    for (int i = 0; i < symbol_table.scope_count; i++) {
        printf("Ambito %d (%s):\n", i, i == 0 ? "global" : "local");
        Scope *scope = &symbol_table.scopes[i];
        if (scope->symbol_count == 0) {
            printf("  (Ninguna variable declarada)\n");
        } else {
            for (int j = 0; j < scope->symbol_count; j++) {
                printf("  Variable: %-10s Tipo: %s\n", scope->symbols[j].name,
                       scope->symbols[j].type == TYPE_INT ? "int" : "float");
            }
        }
    }
    if (symbol_table.scope_count == 0) {
        printf("(Ningun ambito creado)\n");
    }
    printf("Analisis semantico completado con exito.\n");

    free_ast(ast);
    return 0;
}