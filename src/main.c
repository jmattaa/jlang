#include "io.h"
#include "lexer.h"
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

    jlang_lexerInit(src);
    jlang_token *token;
    while((token = jlang_lexerNext())->t != TOKEN_EOF)
    {
        jlang_tokenPrint(token);
        jlang_tokenFree(token);
    }

    free(src);
    return 0;
}
