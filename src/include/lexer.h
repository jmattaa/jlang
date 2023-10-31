#ifndef JLANG_LEXER_H
#define JLANG_LEXER_H

#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "token.h"

typedef struct 
{
    char *src;
    size_t src_size;
    char c;
    unsigned int i;
} JLexer;

// create the JLexer obj
JLexer *JLexer_Init(char *src);

// advance the lexer
void JLexer_Advance(JLexer *lexer);
// skip all whitespace
void JLexer_SkipWhitespace(JLexer *lexer);

// parse id
JToken *JLexer_ParseId(JLexer *lexer);
// parse number
JToken *JLexer_ParseNum(JLexer *lexer);

// return the current char and token and advance
JToken *JLexer_AdvanceCurrent(JLexer *lexer, int type);

// get the next token
JToken *JLexer_NextToken(JLexer *lexer);

#endif
