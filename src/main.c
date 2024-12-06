#include "IO.h"
#include "jlang.h"
#include <stdio.h>

int main(int argc, char** argv)
{
    if (argc < 2) {
        printf("jlang usage: %s <file>\n", argv[0]);
        return 1;
    }

    int compiled = 0;
    for (int i = 0; i < argc; i++) {
        if (argv[i][0] == '-') {
            switch (argv[i][1]) {
            case 'o': {
                char* src = IO_ReadFile(argv[1]);
                Jlang_Compile(src, argv[i + 1]);
                compiled = 1;
                break;
            }
            case 'h':
                printf("jlang usage: %s <file> -o <output>\n", argv[0]);
                compiled = 1; // we dont want to compile
                break;
            }
        }
    }

    if (!compiled) {
        printf("jlang: No output file provided\n");
        printf("jlang usage: %s <file> -o <output>\n", argv[0]);
    }

    return 0;
}
