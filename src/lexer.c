#include "include/lexer.h"
#include "include/token.h"

JLexer *JLexer_Init(char *src)
{
    JLexer *lexer = calloc(1, sizeof(JLexer));

    lexer->src = src;
    lexer->src_size = strlen(src);
    lexer->i = 0;
    lexer->c = src[lexer->i];

    return lexer;
}

void JLexer_Advance(JLexer *lexer)
{
    if (lexer->i < lexer->src_size && lexer->c != '\0') 
    {
        lexer->i += 1;
        lexer->c = lexer->src[lexer->i];
    }
}

void JLexer_SkipWhitespace(JLexer *lexer)
{
    while (lexer->c == '\r' || lexer->c == '\n' || 
            lexer->c == ' ' || lexer->c == '\t')
        JLexer_Advance(lexer);
}

JToken *JLexer_ParseId(JLexer *lexer)
{
    char *val = calloc(1, sizeof(char));

    while (isalnum(lexer->c))
    {
        val = realloc(val, (strlen(val) + 2) * sizeof(char));
        strcat(val, (char[]){lexer->c, 0});

        JLexer_Advance(lexer);
    }
    return JToken_Init(val, TOKEN_ID);
}

JToken *JLexer_ParseNum(JLexer *lexer)
{
    char *val = calloc(1, sizeof(char));

    while (isdigit(lexer->c))
    {
        val = realloc(val, (strlen(val) + 2) * sizeof(char));
        strcat(val, (char[]){lexer->c, 0});
        JLexer_Advance(lexer);
    }
    return JToken_Init(val, TOKEN_INT);
}

JToken *JLexer_AdvanceCurrent(JLexer *lexer, int type)
{
    char *val = calloc(2, sizeof(char));
    val[0] = lexer->c;
    val[1] = '\0';

    JToken *tok = JToken_Init(val, type);
    JLexer_Advance(lexer);

    return tok;
}

JToken *JLexer_NextToken(JLexer *lexer)
{
    while (lexer->c != '\0') 
    {
        JLexer_SkipWhitespace(lexer);
        
        if (isalpha(lexer->c))
            return JLexer_ParseId(lexer);

        if (isdigit(lexer->c))
            return JLexer_ParseNum(lexer);

        switch (lexer->c)
        {
            case ':': return JLexer_AdvanceCurrent(lexer, TOKEN_COLON);
            case '(': return JLexer_AdvanceCurrent(lexer, TOKEN_LPAREN);
            case ')': return JLexer_AdvanceCurrent(lexer, TOKEN_RPAREN);
            case '{': return JLexer_AdvanceCurrent(lexer, TOKEN_LBRACE);
            case '}': return JLexer_AdvanceCurrent(lexer, TOKEN_RBRACE);
            case '=': return JLexer_AdvanceCurrent(lexer, TOKEN_EQUALS);
            case ';': return JLexer_AdvanceCurrent(lexer, TOKEN_SEMI);
            case '\0': break;
            default: 
                printf("JLang [Lexer]: Unexpected character '%c'\n", lexer->c);
                exit(1);
        }
    }

    return JToken_Init(0, TOKEN_EOF);
}

