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

static int is_spanish_char(char c) {
    // Lista de caracteres españoles en UTF-8 (primer byte)
    // á (0xC3 0xA1), é (0xC3 0xA9), í (0xC3 0xAD), ó (0xC3 0xB3), ú (0xC3 0xBA)
    // ñ (0xC3 0xB1), ü (0xC3 0xBC)
    // Á (0xC3 0x81), É (0xC3 0x89), Í (0xC3 0x8D), Ó (0xC3 0x93), Ú (0xC3 0x9A)
    // Ñ (0xC3 0x91), Ü (0xC3 0x9C)
    if (c == (char)0xC3) {
        char next = input[pos + 1];
        return (next == (char)0xA1 || next == (char)0xA9 || next == (char)0xAD ||
                next == (char)0xB3 || next == (char)0xBA || next == (char)0xB1 ||
                next == (char)0xBC || next == (char)0x81 || next == (char)0x89 ||
                next == (char)0x8D || next == (char)0x93 || next == (char)0x9A ||
                next == (char)0x91 || next == (char)0x9C);
    }
    return 0;
}

static int is_id_start(char c) {
    return isalpha(c) || c == '_' || c == '@' || is_spanish_char(c) ||
           c == '!' || c == '#' || c == '$' || c == '%' || c == '&' ||
           c == '*' || c == '^' || c == '`' || c == '~' || c == '|' ||
           c == '\\' || c == '?' || c == ':' || c == '.' || c == '/';
}

static int is_id_char(char c) {
    return isalnum(c) || c == '_' || c == '@' || is_spanish_char(c) ||
           c == '!' || c == '#' || c == '$' || c == '%' || c == '&' ||
           c == '*' || c == '^' || c == '`' || c == '~' || c == '|' ||
           c == '\\' || c == '?' || c == ':' || c == '.' || c == '/';
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
            if (is_spanish_char(peek())) {
                // Avanzar dos bytes para caracteres UTF-8
                if (i < MAX_ID_LEN - 2) {
                    token.lexeme[i++] = advance();
                    token.lexeme[i++] = advance();
                } else {
                    break;
                }
            } else {
                token.lexeme[i++] = advance();
            }
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
        case '>':
            token.lexeme[0] = advance();
            if (peek() == '=') {
                token.lexeme[1] = advance();
                token.lexeme[2] = '\0';
                token.type = TOKEN_GE;
            } else {
                token.lexeme[1] = '\0';
                token.type = TOKEN_GT;
            }
            return token;
        case '<':
            token.lexeme[0] = advance();
            if (peek() == '=') {
                token.lexeme[1] = advance();
                token.lexeme[2] = '\0';
                token.type = TOKEN_LE;
            } else {
                token.lexeme[1] = '\0';
                token.type = TOKEN_LT;
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