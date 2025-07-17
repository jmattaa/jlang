#ifndef _JLANG_AST_H
#define _JLANG_AST_H

#include <stddef.h>
#define JLANG_AST_TYPES_ITER(_X)                                               \
    _X(AST_COMPOUND)                                                           \
    _X(AST_FUNCTION_DECL)                                                      \
    _X(AST_ASSIGNMENT)                                                         \
    _X(AST_LITERAL)

typedef enum
{
#define X(name) name,
    JLANG_AST_TYPES_ITER(X)
#undef X
} jlang_astType;

typedef struct ast
{
    jlang_astType t;

    union
    {
        struct
        {
            struct ast **children;
            size_t nchildren;
        } compound;

        struct
        {
            char *name;
            char *ret_type;
            struct ast *body;
        } function_decl;

        struct
        {
            char *name;
            char *type;
            struct ast *value;
        } assignment;

        struct
        {
            char *val;
        } literal;
    };
} jlang_ast;

#endif
