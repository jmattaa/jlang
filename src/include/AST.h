#ifndef JLANG_AST_H
#define JLANG_AST_H

#include <stdlib.h>
#include "dynlist.h"

typedef struct
{
    enum
    {
        AST_COMPOUND,
        AST_FUNCTION_DEF,
        AST_DEF_TYPE,
        AST_VARIABLE,
        AST_STATEMENT,
        AST_NOOP,
    } type;
    
    dynlist *children; // only exist if its a AST_Compound
} AST;

AST *AST_Init(int type);

#endif
