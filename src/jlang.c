#include "include/jlang.h"
#include "include/lexer.h"

void Jlang_Compile(char *src)
{
    JLexer *lexer = JLexer_Init(src);
    JToken *tok = 0;

    while ((tok = JLexer_NextToken(lexer))->type != TOKEN_EOF)
    {
        printf("TOK: <val: '%s', type %d>\n", tok->value, tok->type);
    }
}

