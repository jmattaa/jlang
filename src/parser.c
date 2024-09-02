#include "include/parser.h"

Parser* Parser_Init(Lexer* lexer)
{
    Parser* parser = calloc(1, sizeof(Parser));

    parser->lexer = lexer;
    parser->token = Lexer_NextToken(lexer);

    return parser;
}

Token* Parser_Eat(Parser* parser, int type)
{
    if (parser->token->type != type) {
        printf("jlang [Parser]: Unexpected token '%s' expected %s\n",
            Tok_to_str(parser->token), Tok_to_str(Token_Init("", type)));
        exit(1);
    }

    Token_FreeToken(parser->token); // this line fixed 390 bytes of mem leak

    parser->token = Lexer_NextToken(parser->lexer);
    return parser->token;
}

AST* Parser_Parse(Parser* parser) { return Parser_ParseCompound(parser); }

AST* Parser_ParseId(Parser* parser)
{
    char* val = calloc(strlen(parser->token->value) + 1, sizeof(char));
    strcpy(val, parser->token->value);

    // expect name
    Parser_Eat(parser, TOKEN_ID);
    if (parser->token->type == TOKEN_COLON) {
        Parser_Eat(parser, TOKEN_COLON);
        // get the type of the variable
        char* type = parser->token->value;

        // expect the type
        Parser_Eat(parser, TOKEN_ID);

        // defining something
        if (parser->token->type == TOKEN_EQUALS) {
            // expect the equals
            Parser_Eat(parser, TOKEN_EQUALS);

            AST* ast = AST_Init(AST_ASSIGNMENT);

            ast->name = val;
            ast->variable_type = type;

            ast->value = Parser_ParseExpr(parser);

            // end all definitions with semicolon
            Parser_Eat(parser, TOKEN_SEMI);

            return ast;
        }
    }

    // calling or using variable
    AST* ast = AST_Init(AST_VARIABLE);
    ast->name = val;

    if (parser->token->type == TOKEN_LPAREN) {
        ast->type = AST_FUNCTION_CALL;
        // get the args
        ast->value = Parser_ParseExpr(parser);

        // we need semi after functin call
        Parser_Eat(parser, TOKEN_SEMI);
    }

    return ast;
}

AST* Parser_ParseBlock(Parser* parser)
{
    Parser_Eat(parser, TOKEN_LBRACE);

    AST* ast = AST_Init(AST_COMPOUND);

    while (parser->token->type != TOKEN_RBRACE) {
        Dynlist_Append(ast->children, Parser_ParseExpr(parser));
    }

    Parser_Eat(parser, TOKEN_RBRACE);
    return ast;
}

AST* Parser_ParseList(Parser* parser)
{
    Parser_Eat(parser, TOKEN_LPAREN);

    AST* ast = AST_Init(AST_COMPOUND);

    if (parser->token->type == TOKEN_RPAREN) {
        Parser_Eat(parser, TOKEN_RPAREN);

        // function definition
        if (parser->token->type == TOKEN_LBRACE) {
            // make ast know were a function not a variable
            ast->type = AST_FUNCTION_BODY;

            // parse body
            ast->value = Parser_ParseBlock(parser);

            return ast;
        }

        return NULL; // if we call with no args we no need for args
    }

    Dynlist_Append(ast->children, Parser_ParseExpr(parser));
    while (parser->token->type == TOKEN_COMMA) {
        Parser_Eat(parser, TOKEN_COMMA);
        Dynlist_Append(ast->children, Parser_ParseExpr(parser));
    }

    Parser_Eat(parser, TOKEN_RPAREN);

    // function definition
    if (parser->token->type == TOKEN_LBRACE) {
        // make ast know were a function not a variable
        ast->type = AST_FUNCTION_BODY;

        // parse body
        ast->value = Parser_ParseBlock(parser);
    }

    return ast;
}

AST* Parser_ParseInt(Parser* parser)
{
    int int_value = atoi(parser->token->value);
    Parser_Eat(parser, TOKEN_INT);

    AST* ast = AST_Init(AST_INT);

    ast->int_value = int_value;

    return ast;
}

AST* Parser_ParseExpr(Parser* parser)
{
    switch (parser->token->type) {
    case TOKEN_ID:
        return Parser_ParseId(parser);
    case TOKEN_LPAREN:
        return Parser_ParseList(parser); // for example function arguments
    case TOKEN_INT:
        return Parser_ParseInt(parser);
    default:
        printf("jlang [Parser]: Unexpected token: '%s'\n",
            Tok_to_str(parser->token));
        exit(1);
    }
}

AST* Parser_ParseCompound(Parser* parser)
{
    AST* compound = AST_Init(AST_COMPOUND);

    while (parser->token->type != TOKEN_EOF) {
        Dynlist_Append(compound->children, Parser_ParseExpr(parser));
    }

    return compound;
}

void Parser_FreeParser(Parser* parser)
{
    if (parser->lexer)
        Lexer_FreeLexer(parser->lexer);
    if (parser->token)
        Token_FreeToken(parser->token);

    free(parser);
}
