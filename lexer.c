#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

static char *code;
static int pos = 0;
static int line = 1;
static int column = 1;

void set_input(char *input) {
    code = input;
    pos = 0;
    line = 1;
    column = 1;
}

Token get_next_token() {
    Token token = {TOKEN_ERROR, "", 0.0, line, column};
    while (code[pos] != '\0') {
        if (isspace(code[pos])) {
            if (code[pos] == '\n') {
                line++;
                column = 1;
            } else {
                column++;
            }
            pos++;
            continue;
        }
        token.line = line;
        token.column = column;

        if (isalpha(code[pos])) {
            int i = 0;
            while (isalnum(code[pos]) && i < MAX_ID_LEN - 1) {
                token.lexeme[i++] = code[pos++];
                column++;
            }
            token.lexeme[i] = '\0';
            if (strcmp(token.lexeme, "int") == 0) token.type = TOKEN_INT;
            else if (strcmp(token.lexeme, "float") == 0) token.type = TOKEN_FLOAT;
            else if (strcmp(token.lexeme, "if") == 0) token.type = TOKEN_IF;
            else if (strcmp(token.lexeme, "else") == 0) token.type = TOKEN_ELSE;
            else token.type = TOKEN_ID;
            return token;
        }
        if (isdigit(code[pos]) || code[pos] == '.') {
            int i = 0;
            while ((isdigit(code[pos]) || code[pos] == '.') && i < MAX_ID_LEN - 1) {
                token.lexeme[i++] = code[pos++];
                column++;
            }
            token.lexeme[i] = '\0';
            token.type = TOKEN_NUMBER;
            sscanf(token.lexeme, "%f", &token.value);
            return token;
        }
        switch (code[pos]) {
            case '+': token.type = TOKEN_PLUS; token.lexeme[0] = '+'; token.lexeme[1] = '\0'; pos++; column++; break;
            case '-': token.type = TOKEN_MINUS; token.lexeme[0] = '-'; token.lexeme[1] = '\0'; pos++; column++; break;
            case '=':
                if (code[pos + 1] == '=') {
                    token.type = TOKEN_EQ;
                    strcpy(token.lexeme, "==");
                    pos += 2;
                    column += 2;
                } else {
                    token.type = TOKEN_EQUAL;
                    token.lexeme[0] = '=';
                    token.lexeme[1] = '\0';
                    pos++;
                    column++;
                }
                break;
            case ';': token.type = TOKEN_SEMICOLON; token.lexeme[0] = ';'; token.lexeme[1] = '\0'; pos++; column++; break;
            case '(': token.type = TOKEN_LPAREN; token.lexeme[0] = '('; token.lexeme[1] = '\0'; pos++; column++; break;
            case ')': token.type = TOKEN_RPAREN; token.lexeme[0] = ')'; token.lexeme[1] = '\0'; pos++; column++; break;
            case '{': token.type = TOKEN_LBRACE; token.lexeme[0] = '{'; token.lexeme[1] = '\0'; pos++; column++; break;
            case '}': token.type = TOKEN_RBRACE; token.lexeme[0] = '}'; token.lexeme[1] = '\0'; pos++; column++; break;
            default:
                token.lexeme[0] = code[pos++];
                token.lexeme[1] = '\0';
                column++;
                return token; // TOKEN_ERROR
        }
        return token;
    }
    token.type = TOKEN_EOF;
    token.line = line;
    token.column = column;
    return token;
}