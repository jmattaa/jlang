#include "include/jlang.h"

void Jlang_Compile(char *src)
{
    Lexer *lexer = Lexer_Init(src);
    Parser *parser = Parser_Init(lexer);

    AST *root = Parser_Parse(parser);

    const char *section_text = ".section .text\n"
                               ".globl _start\n"
                               "_start:\n"
                               "call main\n"
                               "mov %eax, %ebx\n"
                               "mov $1, %eax\n"
                               "int $0x80\n";

    char *as = calloc(strlen(section_text) + 1, sizeof(char));
    strcpy(as, section_text);

    char *asmVal = ASMFrontend(root);
    as = realloc(as, (strlen(as) + strlen(section_text) + 1) * sizeof(char));
    strcat(as, asmVal);

    IO_WriteFile("out.S", as);

    // prevent all of the memory leaks cuz we got warnings
    // and warnings ain't good
    Parser_FreeParser(parser);
    free(asmVal);
}
