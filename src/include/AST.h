#ifndef JLANG_AST_H
#define JLANG_AST_H

#include "dynlist.h"
#include <stdlib.h>

typedef struct
{
    dynlist *names;
    dynlist *offsets;
} variables_list;

typedef struct AST_STRUCT
{
    enum
    {
        AST_COMPOUND,
        AST_FUNCTION_BODY,
        AST_ASSIGNMENT,
        AST_VARIABLE,
        AST_FUNCTION_CALL,
        AST_INT,
        AST_NOOP,
    } type;

    dynlist *children; // only exist if its a AST_Compound

    char *name;
    struct AST_STRUCT *value;

    // assignment specific stuff
    char *variable_type;

    // int
    int int_value;

    // stack
    int stack_index;
} AST;

AST *AST_Init(int type);

#endif
