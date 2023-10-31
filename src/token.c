#include "include/token.h"

JToken *JToken_Init(char *value, int type)
{
    JToken *t = calloc(1, sizeof(JToken));

    t->value = value;
    t->type = type;

    return t;
}

