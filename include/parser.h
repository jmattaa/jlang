#ifndef _JLANG_PARSER_H
#define _JLANG_PARSER_H

#include "ast.h"
#include "lexer.h"

jlang_ast *jlang_parse();
void jlang_freeAst(jlang_ast *ast);

#endif
