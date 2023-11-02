#include "include/jlang.h"
#include "include/asm_frontend.h"

void Jlang_Compile(char *src)
{
    Lexer *lexer = Lexer_Init(src);
    Parser *parser = Parser_Init(lexer);

    AST *root = Parser_Parse(parser);

    char *as = ASMFrontend(root);

    printf("%s\n", as);
}
