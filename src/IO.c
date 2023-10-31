#include "include/IO.h"

char *IO_ReadFile(const char *filename)
{
    FILE *file = fopen(filename, "r");

    if (file == NULL) 
    {
        perror("JLang [IO]: error opening the file");
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *file_contents = (char *)malloc(file_size + 1);

    if (file_contents == NULL) 
    {
        perror("JLang [IO]: memory allocation error");
    }

    fread(file_contents, 1, file_size, file);
    file_contents[file_size - 1] = '\0'; 

    fclose(file);

    return file_contents;
}

