#include "io.h"
#include <stdio.h>
#include <stdlib.h>

char *jlang_readFile(const char *path)
{
    FILE *f = fopen(path, "r");

    if (f == NULL)
    {
        fprintf(stderr, "jlang [ERROR] could not open file: %s\n", path);
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    size_t len = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *file_content = malloc(len + 1);
    if (file_content == NULL)
    {
        fprintf(stderr, "jlang [ERROR] out of memory\n");
        return NULL;
    }

    fread(file_content, len, 1, f);
    file_content[len] = '\0';

    fclose(f);
    return file_content;
}
