#ifndef JLANG_AST_H
#define JLANG_AST_H

#include <stdlib.h>
#include "dynlist.h"

typedef struct AST_STRUCT
{
    enum
    {
        AST_COMPOUND,
        AST_FUNCTION,
        AST_ASSIGNMENT,
        AST_DEF_TYPE,
        AST_VARIABLE,
        AST_STATEMENT,
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
} AST;

AST *AST_Init(int type);

#endif
