#ifndef LEXER_H
#define LEXER_H

#include "token.h"

void set_input(const char *input);
Token get_next_token(void);

#endif