#ifndef _JLANG_LEXER_H
#define _JLANG_LEXER_H

#define JLANG_TOKEN_TYPES_ITER(_X)                                             \
    _X(TOKEN_ID)                                                               \
    _X(TOKEN_EQUALS)                                                           \
    _X(TOKEN_LPAREN)                                                           \
    _X(TOKEN_RPAREN)                                                           \
    _X(TOKEN_NUMBER)                                                           \
    _X(TOKEN_SEMICOLON)

typedef enum
{
#define X(name) name,
    JLANG_TOKEN_TYPES_ITER(X)
#undef X
} jlang_tokenType;

typedef struct
{
    jlang_tokenType type;
    char *value;
} jlang_token;

#endif
