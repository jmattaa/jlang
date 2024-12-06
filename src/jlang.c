#include "jlang.h"

void Jlang_Compile(char* src, char* outfile)
{
    Lexer* lexer = Lexer_Init(src);
    Parser* parser = Parser_Init(lexer);

    AST* root = Parser_Parse(parser);

    char* as = ASMFrontend_Root(root, Dynlist_Init(sizeof(AST*)));

    IO_WriteFile(strcat(outfile, ".S"), as);

    // prevent all of the memory leaks cuz we got warnings
    // and warnings ain't good
    Parser_FreeParser(parser);
    free(as);
}
