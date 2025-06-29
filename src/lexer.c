#include "lexer.h"
#include "lexutils.h"
#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

jlang_lexer lexer;

// ------------------------ helper function decl ------------------------------
static void lexer_advance();
static void lexer_skip_ws();
static jlang_token *lexer_advance_with_token(jlang_tokenType t);
// ----------------------------------------------------------------------------

void jlang_lexerInit(const char *src)
{
    lexer.src = src;
    lexer.src_len = strlen(src);
    lexer.pos = (pos_t){1, 1, 0};
    lexer.c = lexer.src[lexer.pos.srcidx];
}

jlang_token *jlang_lexerNext()
{
    while (lexer.c != '\0')
    {
        lexer_skip_ws();
        // parse id and num
        switch (lexer.c)
        {
        case '=':
            return lexer_advance_with_token(TOKEN_EQUALS);
        case '(':
            return lexer_advance_with_token(TOKEN_LPAREN);
        case ')':
            return lexer_advance_with_token(TOKEN_RPAREN);
        case ':':
            return lexer_advance_with_token(TOKEN_COLON);
        case ';':
            return lexer_advance_with_token(TOKEN_SEMICOLON);
        case '\0':
            break;
        default:
            printf("jlang [ERROR] unknown char: '%c' at line %zu col %zu\n",
                   lexer.c, lexer.pos.line, lexer.pos.col); // TODO: logging
            exit(1);
        }
    }

    return jlang_tokenInit(TOKEN_EOF, "", lexer.pos);
}

// -------------------------- helper function impl ----------------------------
static void lexer_advance()
{
    if (lexer.pos.srcidx >= lexer.src_len || lexer.c == '\0')
        return;

    if (lexer.c == '\n')
    {
        lexer.pos.line++;
        lexer.pos.col = 0;
    }
    else
        lexer.pos.col++;

    lexer.pos.srcidx++;
    lexer.c = lexer.src[lexer.pos.srcidx];
}

static void lexer_skip_ws()
{
    while (lexer.c == ' ' || lexer.c == '\t' || lexer.c == '\n')
        lexer_advance();
}

static jlang_token *lexer_advance_with_token(jlang_tokenType t)
{
    const char val[2] = {lexer.c, '\0'};
    lexer_advance();
    return jlang_tokenInit(t, val, lexer.pos);
}
// ---------------------------------------------------------------------------
