#include "include/asm_frontend.h"

char *ASMFrontend_Compound(AST *ast)
{
    char *val = calloc(1, sizeof(char));
    for (int i = 0; i < ast->children->size; i++)
    {
        AST *child_ast = (AST *)ast->children->items[i];
        char *next_val = ASMFrontend(child_ast);
        val = realloc(val, (strlen(next_val) + 1) * sizeof(char));
        strcat(val, next_val);
    }

    return val;
}

char *ASMFrontend_Assignment(AST *ast) {
    char *test = "mov eax, 123";
    char *val = (char *)calloc(strlen(test) + 1, sizeof(char));
    strcpy(val, test);
    
    return val;
}

char *ASMFrontent_Variable(AST *ast) {}

char *ASMFrontend_Int(AST *ast) {}

char *ASMFrontend(AST *ast)
{
    char *val = calloc(1, sizeof(char));
    char *next_val = 0;

    switch (ast->type)
    {
    case AST_COMPOUND:
        next_val = ASMFrontend_Compound(ast);
        break;
    case AST_ASSIGNMENT:
        next_val = ASMFrontend_Assignment(ast);
        break;
    case AST_VARIABLE:
        next_val = ASMFrontent_Variable(ast);
        break;
    case AST_INT:
        next_val = ASMFrontend_Int(ast);
        break;
    default:
        printf("jlang [ASM Frontend]: no frontend for ast type %d\n",
               ast->type);
        exit(1);
    }

    val = realloc(val, (strlen(next_val) + 1) * sizeof(char));
    strcat(val, next_val);

    return val;
}
