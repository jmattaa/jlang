#include "include/asm_frontend.h"

AST *var_lookup(dynlist *varlist, const char *name)
{
    for (int i = 0; i < (int)varlist->size; i++)
    {
        AST *child_ast = (AST *)varlist->items[i];

        if (!child_ast->name)
            continue;

        if (strcmp(child_ast->name, name) == 0)
            return child_ast;
    }

    return 0;
}

char *ASMFrontend_Compound(AST *ast, dynlist *varlist)
{
    char *val = calloc(1, sizeof(char));
    for (int i = 0; i < ast->children->size; i++)
    {
        AST *child_ast = (AST *)ast->children->items[i];
        char *next_val = ASMFrontend(child_ast, varlist);

        val = realloc(val, (strlen(next_val) + strlen(val) + 1) * sizeof(char));
        strcat(val, next_val);

        free(next_val);
    }

    return val;
}

char *ASMFrontend_Assignment(AST *ast, dynlist *varlist)
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

        // get func args in list
        for (unsigned int i = 0; i < ast->value->children->size; i++)
        {
            AST *func_arg = (AST *)ast->value->children->items[i];

            AST *arg_var = AST_Init(AST_VARIABLE);
            arg_var->name = func_arg->name;
            arg_var->stack_index = (4 * ast->value->children->size) - (i * 4);

            Dynlist_Append(varlist, arg_var);

            // the amount of arg we have
            // this we can use for local vars
            varlist->current_offset = i;
            free(arg_var);
        }

        char *funcbody = ASMFrontend(ast->value->value, varlist);
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

        char *ret_val = ASMFrontend(ast->value, varlist);
        sprintf(val, template, ret_val);

        free(ret_val);
        return val;
    }

    // defining a normal variable not function
    char *template = "# defining '%s'\n"
                     "movl %s, %%eax\n"
                     "pushl %%eax\n";
    char *var_val = ASMFrontend(ast->value, varlist);
    char *val = calloc(strlen(template) + 256, sizeof(char));

    sprintf(val, template, ast->name, var_val);

    // save variable in varlist
    AST *var = AST_Init(AST_VARIABLE);
    var->name = ast->name;
    var->stack_index =
        (int)(-1 * (((int)varlist->size * 4) - (4 * varlist->current_offset)));

    Dynlist_Append(varlist, var);

    free(var_val);
    free(var);
    return val;
}

char *ASMFrontent_Variable(AST *ast, dynlist *varlist)
{
    AST *var = var_lookup(varlist, ast->name);

    if (!var)
    {
        printf("jlang: Variable '%s' not defined\n", ast->name);
        exit(1);
    }

    char *template = "%d(%%ebp)";

    char *val = calloc(strlen(template) + 8, sizeof(char));
    sprintf(val, template, var->stack_index);

    return val;
}

char *ASMFrontend_Int(AST *ast, dynlist *varlist)
{
    char *template = "$%d";

    char *val = calloc(strlen(template) + 256, sizeof(char));
    sprintf(val, template, ast->int_value);

    return val;
}

char *ASMFrontend(AST *ast, dynlist *varlist)
{
    char *val = calloc(1, sizeof(char));
    char *next_val = 0;

    switch (ast->type)
    {
    case AST_COMPOUND:
        next_val = ASMFrontend_Compound(ast, varlist);
        break;
    case AST_ASSIGNMENT:
        next_val = ASMFrontend_Assignment(ast, varlist);
        break;
    case AST_VARIABLE:
        next_val = ASMFrontent_Variable(ast, varlist);
        break;
    case AST_INT:
        next_val = ASMFrontend_Int(ast, varlist);
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

char *ASMFrontend_Root(AST *ast, dynlist *varlist)
{
    const char *section_text = ".section .text\n"
                               ".globl _start\n"
                               "_start:\n"
                               "pushl 0(%esp)\n" // give argc to main
                               "pushl 4(%esp)\n" // give argv to main
                               "call main\n"
                               "addl $4, %esp\n" // reset ptr
                               "movl %eax, %ebx\n"
                               "movl $1, %eax\n"
                               "int $0x80\n";

    char *as = calloc(strlen(section_text) + 1, sizeof(char));
    strcpy(as, section_text);

    char *asmVal = ASMFrontend(ast, varlist);
    as = realloc(as, (strlen(as) + strlen(asmVal) + 1) * sizeof(char));
    strcat(as, asmVal);

    free(asmVal);
    Dynlist_Freelist(varlist);
    return as;
}
