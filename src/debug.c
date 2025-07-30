#ifdef JL_DEBUG
#include "ast.h"
#include <stdio.h>

static void print_indent(int indent)
{
    for (int i = 0; i < indent; i++)
    {
        putchar(' ');
    }
}
void jlang_debugprintast_impl(jlang_ast *ast, int indent)
{
    if (!ast)
        return;

    switch (ast->t)
    {
    case AST_COMPOUND:
        print_indent(indent);
        printf("compound {\n");
        for (size_t i = 0; i < ast->compound.nchildren; i++)
        {
            jlang_debugprintast_impl(ast->compound.children[i], indent + 2);
        }
        print_indent(indent);
        printf("}\n");
        break;

    case AST_FUNCTION_DECL:
        print_indent(indent);
        printf("func decl {\n");
        print_indent(indent + 2);
        printf("name: %s\n", ast->function_decl.name);
        print_indent(indent + 2);
        printf("ret_type: %s\n", ast->function_decl.ret_type);
        print_indent(indent + 2);
        printf("body:\n");
        jlang_debugprintast_impl(ast->function_decl.body, indent + 4);
        print_indent(indent);
        printf("}\n");
        break;

    case AST_ASSIGNMENT:
        print_indent(indent);
        printf("assignment {\n");
        print_indent(indent + 2);
        printf("name: %s\n", ast->assignment.name);
        print_indent(indent + 2);
        printf("type: %s\n", ast->assignment.type);
        print_indent(indent + 2);
        printf("val:\n");
        jlang_debugprintast_impl(ast->assignment.value, indent + 4);
        print_indent(indent);
        printf("}\n");
        break;

    case AST_LITERAL_STR:
        print_indent(indent);
        printf("literal: \"%s\"\n", ast->literalstr.val);
        break;

    case AST_LITERAL_INT:
        print_indent(indent);
        printf("literal: %d\n", ast->literalint.val);
        break;
    }
}
#endif
