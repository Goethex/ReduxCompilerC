#ifndef TOKEN_H
#define TOKEN_H

#define MAX_ID_LEN 32
#define MAX_CODE_LEN 1024

typedef enum {
    TOKEN_INT, TOKEN_FLOAT, TOKEN_ID, TOKEN_NUMBER, TOKEN_PLUS,
    TOKEN_MINUS, TOKEN_EQUAL, TOKEN_SEMICOLON, TOKEN_EOF, TOKEN_ERROR,
    TOKEN_IF, TOKEN_LPAREN, TOKEN_RPAREN, TOKEN_EQ, TOKEN_ELSE,
    TOKEN_LBRACE, TOKEN_RBRACE // Nuevos tokens
} TokenType;

typedef struct {
    TokenType type;
    char lexeme[MAX_ID_LEN];
    float value; // Para números
    int line;
    int column; // Posición del carácter en la línea
} Token;

#endif