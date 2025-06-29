#include "token.h"
#include <stdlib.h>

jlang_token *jlang_tokenInit(jlang_tokenType t, const char *val, pos_t pos)
{
    jlang_token *token = malloc(sizeof(jlang_token));
    token->t = t;
    token->val = val;
    token->pos = pos;
    return token;
}

void jlang_tokenFree(jlang_token *token)
{
    free(token);
}
