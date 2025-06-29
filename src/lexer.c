#include "lexer.h"
#include "lexutils.h"
#include "token.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

jlang_lexer lexer;

// ------------------------ helper function decl ------------------------------
static void lexer_advance();
static void lexer_skip_ws();
static jlang_token *lexer_advance_with_token(jlang_tokenType t);
static jlang_token *lexer_parse_id();
static jlang_token *lexer_parse_num();
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

        if (isalpha(lexer.c)) // first char in id must be alphabetic
            return lexer_parse_id();
        if (isdigit(lexer.c))
            return lexer_parse_num();

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
            fprintf(stderr,
                    "jlang [ERROR] unknown char: '%c' at line %zu col %zu\n",
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
        lexer.pos.col = 1;
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
    char *val = calloc(2, sizeof(char));
    if (val == NULL)
    {
        fprintf(stderr, "jlang [ERROR] out of memory\n");
        exit(1);
    }

    val[0] = lexer.c;
    val[1] = '\0';

    lexer_advance();
    return jlang_tokenInit(t, val, lexer.pos);
}

static jlang_token *lexer_parse_id()
{
    size_t startidx = lexer.pos.srcidx;
    while (isalnum(lexer.c))
        lexer_advance();
    size_t len = lexer.pos.srcidx - startidx;

    char *val = calloc(len + 1, sizeof(char));
    if (val == NULL)
    {
        fprintf(stderr, "jlang [ERROR] out of memory\n");
        exit(1);
    }

    memcpy(val, lexer.src + startidx, len);
    val[len] = '\0';

    return jlang_tokenInit(TOKEN_ID, val, lexer.pos);
}

static jlang_token *lexer_parse_num()
{
    size_t startidx = lexer.pos.srcidx;
    while (isdigit(lexer.c))
        lexer_advance();
    size_t len = lexer.pos.srcidx - startidx;

    char *val = calloc(len + 1, sizeof(char));
    if (val == NULL)
    {
        fprintf(stderr, "jlang [ERROR] out of memory\n");
        exit(1);
    }

    memcpy(val, lexer.src + startidx, len);
    val[len] = '\0';

    return jlang_tokenInit(TOKEN_NUMBER, val, lexer.pos);
}
// ---------------------------------------------------------------------------
