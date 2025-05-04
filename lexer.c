#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "token.h"

static char *input;
static int pos = 0;
static int line = 1;
static int column = 1;

void set_input(const char *code) {
    input = (char *)code;
    pos = 0;
    line = 1;
    column = 1;
}

static char peek(void) {
    return input[pos];
}

static char advance(void) {
    char c = input[pos++];
    if (c == '\n') {
        line++;
        column = 1;
    } else {
        column++;
    }
    return c;
}

static void skip_whitespace(void) {
    while (isspace(peek())) {
        advance();
    }
}

static int is_id_start(char c) {
    return isalpha(c) || c == '_' || c == '@';
}

static int is_id_char(char c) {
    return isalnum(c) || c == '_' || c == '@';
}

Token get_next_token(void) {
    skip_whitespace();

    Token token;
    token.lexeme[0] = '\0';
    token.line = line;
    token.column = column;

    if (peek() == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }

    char c = peek();

    // Palabras reservadas y tipos
    if (is_id_start(c)) {
        int i = 0;
        while (is_id_char(peek()) && i < MAX_ID_LEN - 1) {
            token.lexeme[i++] = advance();
        }
        token.lexeme[i] = '\0';

        if (strcmp(token.lexeme, "int") == 0) {
            token.type = TOKEN_INT;
        } else if (strcmp(token.lexeme, "float") == 0) {
            token.type = TOKEN_FLOAT;
        } else if (strcmp(token.lexeme, "if") == 0) {
            token.type = TOKEN_IF;
        } else if (strcmp(token.lexeme, "else") == 0) {
            token.type = TOKEN_ELSE;
        } else {
            token.type = TOKEN_ID;
        }
        return token;
    }

    // Números
    if (isdigit(c)) {
        int i = 0;
        while (isdigit(peek()) && i < MAX_ID_LEN - 1) {
            token.lexeme[i++] = advance();
        }
        token.lexeme[i] = '\0';
        token.type = TOKEN_NUMBER;
        return token;
    }

    // Operadores y símbolos
    switch (c) {
        case '+':
            token.lexeme[0] = advance();
            token.lexeme[1] = '\0';
            token.type = TOKEN_PLUS;
            return token;
        case '-':
            token.lexeme[0] = advance();
            token.lexeme[1] = '\0';
            token.type = TOKEN_MINUS;
            return token;
        case '=':
            token.lexeme[0] = advance();
            if (peek() == '=') {
                token.lexeme[1] = advance();
                token.lexeme[2] = '\0';
                token.type = TOKEN_EQ;
            } else {
                token.lexeme[1] = '\0';
                token.type = TOKEN_EQUAL;
            }
            return token;
        case ';':
            token.lexeme[0] = advance();
            token.lexeme[1] = '\0';
            token.type = TOKEN_SEMICOLON;
            return token;
        case '(':
            token.lexeme[0] = advance();
            token.lexeme[1] = '\0';
            token.type = TOKEN_LPAREN;
            return token;
        case ')':
            token.lexeme[0] = advance();
            token.lexeme[1] = '\0';
            token.type = TOKEN_RPAREN;
            return token;
        case '{':
            token.lexeme[0] = advance();
            token.lexeme[1] = '\0';
            token.type = TOKEN_LBRACE;
            return token;
        case '}':
            token.lexeme[0] = advance();
            token.lexeme[1] = '\0';
            token.type = TOKEN_RBRACE;
            return token;
        default:
            token.lexeme[0] = advance();
            token.lexeme[1] = '\0';
            token.type = TOKEN_ERROR;
            return token;
    }
}