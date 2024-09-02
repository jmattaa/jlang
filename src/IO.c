#include "include/IO.h"

char* IO_ReadFile(const char* filename)
{
    FILE* file = fopen(filename, "r");

    if (file == NULL) {
        printf("JLang [IO]: error opening %s\n", filename);
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* file_contents = (char*)malloc(file_size + 1);

    if (file_contents == NULL)
        perror("JLang [IO]: memory allocation error");

    fread(file_contents, 1, file_size, file);
    file_contents[file_size - 1] = '\0';

    fclose(file);

    return file_contents;
}

void IO_WriteFile(const char* filename, char* outBuffer)
{
    FILE* file = fopen(filename, "wb");

    if (file == NULL) {
        printf("JLang [IO]: error opening %s\n", filename);
        exit(1);
    }

    fprintf(file, "%s", outBuffer);

    fclose(file);
}
