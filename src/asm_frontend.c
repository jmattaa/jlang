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

char *ASMFrontend_Assignment(AST *ast)
{
    if (ast->value->type == AST_FUNCTION_BODY)
    {
        // first 2 %s is function name
        // %s after is body in asm
        char *template = ".globl %s\n"
                         "%s:\n%s\n";

        char *funcbody = ASMFrontend(ast->value->value);
        char *val = calloc(
            (strlen(template) + (strlen(ast->name) * 2) + strlen(funcbody) + 1),
            sizeof(char));

        // function body is pretty much a compound in value
        sprintf(val, template, ast->name, ast->name, funcbody);

        return val;
    }
    else if (strcmp(ast->name, "return") == 0)
    {
        char *template = "mov $%d, %%eax\n"
                         "ret\n";
        char *val = calloc(strlen(template) + sizeof(int), sizeof(char));

        sprintf(val, template, ast->value->int_value);
        return val;
    }

    return "";
}

char *ASMFrontent_Variable(AST *ast) { return ""; }

char *ASMFrontend_Int(AST *ast) { return ""; }

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
