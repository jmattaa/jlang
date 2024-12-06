#include "tokenizer/lexer.h"
#include "tokenizer/token.h"

Lexer* Lexer_Init(char* src)
{
    Lexer* lexer = calloc(1, sizeof(Lexer));

    lexer->src = src;
    lexer->src_size = strlen(src);
    lexer->i = 0;
    lexer->c = src[lexer->i];

    return lexer;
}

void Lexer_Advance(Lexer* lexer)
{
    if (lexer->i < lexer->src_size && lexer->c != '\0') {
        lexer->i += 1;
        lexer->c = lexer->src[lexer->i];
    }
}

void Lexer_SkipWhitespace(Lexer* lexer)
{
    while (lexer->c == '\r' || lexer->c == '\n' || lexer->c == ' ' || lexer->c == '\t')
        Lexer_Advance(lexer);
}

Token* Lexer_ParseId(Lexer* lexer)
{
    char* val = calloc(1, sizeof(char));

    while (isalnum(lexer->c)) {
        val = realloc(val, (strlen(val) + 2) * sizeof(char));
        strcat(val, (char[]) { lexer->c, 0 });

        Lexer_Advance(lexer);
    }
    return Token_Init(val, TOKEN_ID);
}

Token* Lexer_ParseNum(Lexer* lexer)
{
    char* val = calloc(1, sizeof(char));

    while (isdigit(lexer->c)) {
        val = realloc(val, (strlen(val) + 2) * sizeof(char));
        strcat(val, (char[]) { lexer->c, 0 });
        Lexer_Advance(lexer);
    }
    return Token_Init(val, TOKEN_INT);
}

Token* Lexer_AdvanceCurrent(Lexer* lexer, int type)
{
    char* val = calloc(2, sizeof(char));
    val[0] = lexer->c;
    val[1] = '\0';

    Token* tok = Token_Init(val, type);
    Lexer_Advance(lexer);

    return tok;
}

Token* Lexer_NextToken(Lexer* lexer)
{
    while (lexer->c != '\0') {
        Lexer_SkipWhitespace(lexer);

        if (isalpha(lexer->c))
            return Lexer_ParseId(lexer);

        if (isdigit(lexer->c))
            return Lexer_ParseNum(lexer);

        switch (lexer->c) {
        case ':':
            return Lexer_AdvanceCurrent(lexer, TOKEN_COLON);
        case ',':
            return Lexer_AdvanceCurrent(lexer, TOKEN_COMMA);
        case '(':
            return Lexer_AdvanceCurrent(lexer, TOKEN_LPAREN);
        case ')':
            return Lexer_AdvanceCurrent(lexer, TOKEN_RPAREN);
        case '{':
            return Lexer_AdvanceCurrent(lexer, TOKEN_LBRACE);
        case '}':
            return Lexer_AdvanceCurrent(lexer, TOKEN_RBRACE);
        case '=':
            return Lexer_AdvanceCurrent(lexer, TOKEN_EQUALS);
        case ';':
            return Lexer_AdvanceCurrent(lexer, TOKEN_SEMI);
        case '\0':
            break;
        default:
            printf("jlang [Lexer]: Unexpected character '%c'\n", lexer->c);
            exit(1);
        }
    }

    return Token_Init(0, TOKEN_EOF);
}

void Lexer_FreeLexer(Lexer* lexer)
{
    if (lexer->src)
        free(lexer->src);
    free(lexer);
}
