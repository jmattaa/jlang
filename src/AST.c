#include "include/AST.h"

AST* AST_Init(int type)
{
    AST* ast = calloc(1, sizeof(AST));

    ast->type = type;

    if (type == AST_COMPOUND)
        ast->children = Dynlist_Init(sizeof(AST*));

    return ast;
}
