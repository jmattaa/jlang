#include "include/jlang.h"

void Jlang_Compile(char *src)
{
    Lexer *lexer = Lexer_Init(src);
    Parser *parser = Parser_Init(lexer);

    AST *root = Parser_Parse(parser);

    printf("%d\n", root->children->size);
}
