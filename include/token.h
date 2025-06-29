#ifndef _JLANG_TOKEN_H
#define _JLANG_TOKEN_H

#include "lexutils.h"

#define JLANG_TOKEN_TYPES_ITER(_X)                                             \
    _X(TOKEN_ID)                                                               \
    _X(TOKEN_EQUALS)                                                           \
    _X(TOKEN_LPAREN)                                                           \
    _X(TOKEN_RPAREN)                                                           \
    _X(TOKEN_NUMBER)                                                           \
    _X(TOKEN_COLON)                                                            \
    _X(TOKEN_SEMICOLON)                                                        \
    _X(TOKEN_EOF)

typedef enum
{
#define X(name) name,
    JLANG_TOKEN_TYPES_ITER(X)
#undef X
} jlang_tokenType;

typedef struct
{
    jlang_tokenType t;
    const char *val;
    pos_t pos;
} jlang_token;

jlang_token *jlang_tokenInit(jlang_tokenType t, const char *val, pos_t pos);
void jlang_tokenFree(jlang_token *token);

#endif
