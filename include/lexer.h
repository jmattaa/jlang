#ifndef _JLANG_LEXER_H
#define _JLANG_LEXER_H

#include "lexutils.h"
#include "token.h"
#include <stddef.h>

typedef struct
{
    const char *src;
    size_t src_len;

    pos_t pos;
    char c;
} jlang_lexer;

extern jlang_lexer LEXER;

void jlang_lexerInit(const char *src);
jlang_token *jlang_lexerNext();

#endif
