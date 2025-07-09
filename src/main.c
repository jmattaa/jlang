#include "ast.h"
#include "io.h"
#include "lexer.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "jlang [ERROR] no input file\n");
        return 1;
    }

    char *src = jlang_readFile(argv[1]);
    if (src == NULL)
        return 1;

    jlang_lexerInit(src); // this has to be before jlang_parse cuz it
                          // initializes the global LEXER
    jlang_ast *root = jlang_parse();
    jlang_freeAst(root);

    free(src);
    return 0;
}
