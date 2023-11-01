#include "include/token.h"
#include <string.h>

Token *Token_Init(char *value, int type)
{
    Token *t = calloc(1, sizeof(Token));

    t->value = value;
    t->type = type;

    return t;
}

const char *token_type_to_str(int type)
{
    switch (type)
    {
    case TOKEN_ID:
        return "TOKEN_ID";
    case TOKEN_EQUALS:
        return "TOKEN_EQUALS";
    case TOKEN_LPAREN:
        return "TOKEN_LPAREN";
    case TOKEN_RPAREN:
        return "TOKEN_RPAREN";
    case TOKEN_LBRACE:
        return "TOKEN_LBRACE";
    case TOKEN_RBRACE:
        return "TOKEN_RBRACE";
    case TOKEN_COLON:
        return "TOKEN_COLON";
    case TOKEN_COMMA:
        return "TOKEN_COMMA";
    case TOKEN_INT:
        return "TOKEN_INT";
    case TOKEN_SEMI:
        return "TOKEN_SEMI";
    case TOKEN_EOF:
        return "TOKEN_EOF";
    }

    return "not_stringable";
}

char *Tok_to_str(Token *token)
{
    const char *type_str = token_type_to_str(token->type);
    const char *template = "<type=`%s`, int_type=`%d`, value=`%s`>";

    char *str = calloc(strlen(type_str) + strlen(template) + 8, sizeof(char));
    sprintf(str, template, type_str, token->type, token->value);

    return str;
}
