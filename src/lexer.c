#include "lexer.h"
#include "lexutils.h"
#include "logger.h"
#include "token.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

jlang_lexer LEXER;

// ------------------------ helper function decl ------------------------------
static void lexer_advance();
static void lexer_skip_ws();
static jlang_token *lexer_advance_with_token(jlang_tokenType t);
static jlang_token *lexer_parse_id();
static jlang_token *lexer_parse_num();
// ----------------------------------------------------------------------------

void jlang_lexerInit(const char *src)
{
    LEXER.src = src;
    LEXER.src_len = strlen(src);
    LEXER.pos = (pos_t){1, 1, 0};
    LEXER.c = LEXER.src[LEXER.pos.srcidx];
}

jlang_token *jlang_lexerNext()
{
    while (LEXER.c != '\0')
    {
        lexer_skip_ws();

        if (isalpha(LEXER.c)) // first char in id must be alphabetic
            return lexer_parse_id();
        if (isdigit(LEXER.c))
            return lexer_parse_num();

        switch (LEXER.c)
        {
        case '#': // comment skip util \n
        {
            while (LEXER.c != '\n')
                lexer_advance();
            break;
        }
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
            jlang_logFatal(1, "unkown char: '%c' at line %zu col %zu\n",
                           LEXER.c, LEXER.pos.line, LEXER.pos.col);
            break;
        }
    }

    return jlang_tokenInit(TOKEN_EOF, "", LEXER.pos);
}

// -------------------------- helper function impl ----------------------------
static void lexer_advance()
{
    if (LEXER.pos.srcidx >= LEXER.src_len || LEXER.c == '\0')
        return;

    if (LEXER.c == '\n')
    {
        LEXER.pos.line++;
        LEXER.pos.col = 1;
    }
    else
        LEXER.pos.col++;

    LEXER.pos.srcidx++;
    LEXER.c = LEXER.src[LEXER.pos.srcidx];
}

static void lexer_skip_ws()
{
    while (LEXER.c == ' ' || LEXER.c == '\t' || LEXER.c == '\n')
        lexer_advance();
}

static jlang_token *lexer_advance_with_token(jlang_tokenType t)
{
    char *val = calloc(2, sizeof(char));
    if (val == NULL)
        jlang_logFatal(1, "out of memory, calloc failed\n");

    val[0] = LEXER.c;
    val[1] = '\0';

    lexer_advance();
    return jlang_tokenInit(t, val, LEXER.pos);
}

static jlang_token *lexer_parse_id()
{
    size_t startidx = LEXER.pos.srcidx;
    while (isalnum(LEXER.c))
        lexer_advance();
    size_t len = LEXER.pos.srcidx - startidx;

    char *val = calloc(len + 1, sizeof(char));
    if (val == NULL)
        jlang_logFatal(1, "out of memory, calloc failed\n");

    memcpy(val, LEXER.src + startidx, len);
    val[len] = '\0';

    return jlang_tokenInit(TOKEN_ID, val, LEXER.pos);
}

static jlang_token *lexer_parse_num()
{
    size_t startidx = LEXER.pos.srcidx;
    while (isdigit(LEXER.c))
        lexer_advance();
    size_t len = LEXER.pos.srcidx - startidx;

    char *val = calloc(len + 1, sizeof(char));
    if (val == NULL)
        jlang_logFatal(1, "out of memory, calloc failed\n");

    memcpy(val, LEXER.src + startidx, len);
    val[len] = '\0';

    return jlang_tokenInit(TOKEN_NUMBER, val, LEXER.pos);
}
// ---------------------------------------------------------------------------
