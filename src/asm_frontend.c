#include "include/asm_frontend.h"

char *ASMFrontend_Compound(AST *ast)
{
    char *val = calloc(1, sizeof(char));
    for (int i = 0; i < ast->children->size; i++)
    {
        AST *child_ast = (AST *)ast->children->items[i];
        char *next_val = ASMFrontend(child_ast);

        val = realloc(val, (strlen(next_val) + strlen(val) + 1) * sizeof(char));
        strcat(val, next_val);

        free(next_val);
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
                         "%s:\n"
                         "pushl %%ebp\n" // store ebp
                         "movl %%esp, %%ebp\n"
                         "%s\n";

        char *funcbody = ASMFrontend(ast->value->value);
        char *val = calloc(
            (strlen(template) + (strlen(ast->name) * 2) + strlen(funcbody) + 1),
            sizeof(char));

        // function body is pretty much a compound in value
        sprintf(val, template, ast->name, ast->name, funcbody);

        free(funcbody);
        return val;
    }
    else if (strcmp(ast->name, "return") == 0)
    {
        char *template = "movl %s, %%eax\n"    // return val in eax
                         "movl %%ebp, %%esp\n" // reset args
                         "popl %%ebp\n"        // reset stack
                         "ret\n";
        char *val = calloc(strlen(template) + 256, sizeof(char));

        char *ret_val = ASMFrontend(ast->value);
        sprintf(val, template, ret_val);

        free(ret_val);
        return val;
    }

    // defining a normal variable not function
    char *template = "movl %s, %%eax\n"
                     "push %%eax\n";
    char *var_val = ASMFrontend(ast->value);
    char *val = calloc(strlen(template) + 256, sizeof(char));

    sprintf(val, template, var_val);
    free(var_val);
    return val;
}

char *ASMFrontent_Variable(AST *ast)
{
    char *template = "%d(%%esp)";

    char *val = calloc(strlen(template) + 8, sizeof(char));
    sprintf(val, template, 8); // TODO: CALC VAR OFFSET FROM NAME ast->name

    return val;
}

char *ASMFrontend_Int(AST *ast)
{
    char *template = "$%d";

    char *val = calloc(strlen(template) + 256, sizeof(char));
    sprintf(val, template, ast->int_value);

    return val;
}

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

    free(next_val);
    return val;
}

char *ASMFrontend_Root(AST *ast)
{
    const char *section_text = ".section .text\n"
                               ".globl _start\n"
                               "_start:\n"
                               "pushl 0(%esp)\n" // give args to main
                               "call main\n"
                               "addl $4, %esp\n" // reset ptr
                               "movl %eax, %ebx\n"
                               "movl $1, %eax\n"
                               "int $0x80\n";

    char *as = calloc(strlen(section_text) + 1, sizeof(char));
    strcpy(as, section_text);

    char *asmVal = ASMFrontend(ast);
    as = realloc(as, (strlen(as) + strlen(asmVal) + 1) * sizeof(char));
    strcat(as, asmVal);

    free(asmVal);
    return as;
}
