#include <stdio.h>
#include "include/jlang.h"
#include "include/IO.h"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("jlang usage: %s <file>\n", argv[0]);
        return 1;
    }

    char *src = IO_ReadFile(argv[1]);
    Jlang_Compile(src);

    free(src);

    return 0;
}

