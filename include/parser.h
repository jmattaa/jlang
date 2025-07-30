#ifndef _JLANG_PARSER_H
#define _JLANG_PARSER_H

#include "ast.h"
#include "lexer.h"

jlang_ast *jlang_parse();
void jlang_freeAst(jlang_ast *ast);

#ifdef JL_DEBUG
void jlang_debugprintast_impl(jlang_ast *ast, int indent);
#define jlang_debugprintast(ast) jlang_debugprintast_impl(ast, 0)
#endif

#endif
