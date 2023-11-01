#include "include/jlang.h"
#include "include/lexer.h"

void Jlang_Compile(char *src)
{
    Lexer *lexer = Lexer_Init(src);
    Token *tok = 0;

    while ((tok = Lexer_NextToken(lexer))->type != TOKEN_EOF)
    {
        printf("TOK: <val: '%s', type %d>\n", tok->value, tok->type);
    }
}

