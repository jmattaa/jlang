#ifndef JLANG_IO 
#define JLANG_IO

#include <stdlib.h>
#include <stdio.h>

// open and read the content of a file
char *IO_ReadFile(const char *filename);

// create and write to file 
void IO_WriteFile(const char *filename, char *outBuffer);

#endif

