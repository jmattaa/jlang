#include "include/jlang.h"

void Jlang_Compile(char *src)
{
    Lexer *lexer = Lexer_Init(src);
    Parser *parser = Parser_Init(lexer);

    AST *root = Parser_Parse(parser);

    printf("%d\n", root->children->size);

    Token *tok = 0;
    while ((tok = Lexer_NextToken(lexer))->type != TOKEN_EOF)
    {
        printf("TOK: <val: '%s', type %d>\n", tok->value, tok->type);
    }
}

