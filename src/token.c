#include "include/token.h"

Token *Token_Init(char *value, int type)
{
    Token *t = calloc(1, sizeof(Token));

    t->value = value;
    t->type = type;

    return t;
}

