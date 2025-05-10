#ifndef TOKEN_H
#define TOKEN_H

#define MAX_ID_LEN 32
#define MAX_CODE_LEN 1024

typedef enum {
    TOKEN_INT,
    TOKEN_FLOAT,
    TOKEN_ID,
    TOKEN_NUMBER,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_EQUAL,
    TOKEN_SEMICOLON,
    TOKEN_IF,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_EQ,
    TOKEN_ELSE,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_GT,    // Mayor que (>)
    TOKEN_LT,    // Menor que (<)
    TOKEN_GE,    // Mayor o igual (>=)
    TOKEN_LE,    // Menor o igual (<=)
    TOKEN_EOF,
    TOKEN_ERROR
} TokenType;

typedef struct {
    TokenType type;
    char lexeme[MAX_ID_LEN];
    int line;
    int column;
} Token;

void set_input(const char *code);
Token get_next_token(void);

#endif