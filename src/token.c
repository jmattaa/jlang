#include "token.h"
#include "logger.h"
#include <stdio.h>
#include <stdlib.h>

#define X(name) #name,
static const char *token_names[] = {JLANG_TOKEN_TYPES_ITER(X)};
#undef X

jlang_token *jlang_tokenInit(jlang_tokenType t, char *val, pos_t pos)
{
    jlang_token *token = malloc(sizeof(jlang_token));
    if (token == NULL)
        jlang_logFatal(1, "out of memory, malloc failed\n");

    token->t = t;
    token->val = val;
    token->pos = pos;
    return token;
}

void jlang_tokenPrint(jlang_token *token)
{
    jlang_logInfo("token { type: %s, value: \"%s\", line: %zu, col: %zu }\n",
                  token_names[token->t], token->val, token->pos.line,
                  token->pos.col);
}

void jlang_tokenFree(jlang_token *token)
{
    if (token->val)
        free(token->val);
    free(token);
}
