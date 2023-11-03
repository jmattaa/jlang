#ifndef JLANG_TOKEN_H
#define JLANG_TOKEN_H

#include <stdio.h>
#include <stdlib.h>

typedef struct 
{
    char *value;
    enum
    {
        TOKEN_ID,
        TOKEN_INT,
        TOKEN_COLON,
        TOKEN_COMMA,
        TOKEN_LPAREN,
        TOKEN_RPAREN,
        TOKEN_LBRACE,
        TOKEN_RBRACE,
        TOKEN_EQUALS,
        TOKEN_SEMI,
        TOKEN_EOF,
    } type;
} Token;

// create the token obj
Token *Token_Init(char *value, int type);

char *Tok_to_str(Token* tok);

void Token_FreeToken(Token *token);

#endif
