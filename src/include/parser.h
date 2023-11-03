#ifndef JLANG_PARSER_H
#define JLANG_PARSER_H

#include <stdlib.h>
#include "AST.h"
#include "lexer.h"
#include "token.h"

typedef struct
{
    Lexer *lexer;
    Token *token;
} Parser;

// create parser obj
Parser *Parser_Init(Lexer *lexer);

// eat token
Token *Parser_Eat(Parser *parser, int type);

// parse
AST *Parser_Parse(Parser *parser);

// parse expression
AST *Parser_ParseExpr(Parser *parser);

// parse compound
AST *Parser_ParseCompound(Parser *parser);

void Parser_FreeParser(Parser *parser);

#endif

