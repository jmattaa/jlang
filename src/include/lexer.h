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
} Lexer;

// create the JLexer obj
Lexer *Lexer_Init(char *src);

// advance the lexer
void Lexer_Advance(Lexer *lexer);
// skip all whitespace
void Lexer_SkipWhitespace(Lexer *lexer);

// parse id
Token *Lexer_ParseId(Lexer *lexer);
// parse number
Token *Lexer_ParseNum(Lexer *lexer);

// return the current char and token and advance
Token *Lexer_AdvanceCurrent(Lexer *lexer, int type);

// get the next token
Token *Lexer_NextToken(Lexer *lexer);

#endif
