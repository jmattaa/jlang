#include "parser.h"
#include "ast.h"
#include "lexer.h"
#include "logger.h"
#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ------------------------ helper function decl ------------------------------
static jlang_ast *parse_compound(jlang_token **tok);
static jlang_ast *parse_expr(jlang_token **tok);
static jlang_token *parser_eaa(jlang_token *tok,
                               jlang_tokenType t); // expect and advance
static jlang_ast *parse_id(jlang_token **tok);
// ----------------------------------------------------------------------------

// the program is a compound
jlang_ast *jlang_parse()
{
    jlang_token *tok = jlang_lexerNext();
    return parse_compound(&tok);
}

void jlang_freeAst(jlang_ast *ast)
{
    if (ast)
    {
        switch (ast->t)
        {
        case AST_COMPOUND:
            for (size_t i = 0; i < ast->compound.nchildren; i++)
                jlang_freeAst(ast->compound.children[i]);
            if (ast->compound.children)
                free(ast->compound.children);
            break;
        case AST_FUNCTION_DECL:
            if (ast->function_decl.name)
                free(ast->function_decl.name);
            if (ast->function_decl.ret_type)
                free(ast->function_decl.ret_type);
            if (ast->function_decl.body)
                jlang_freeAst(ast->function_decl.body);
            break;
        case AST_ASSIGNMENT:
            if (ast->assignment.name)
                free(ast->assignment.name);
            if (ast->assignment.type)
                free(ast->assignment.type);
            if (ast->assignment.value)
                jlang_freeAst(ast->assignment.value);
            break;
        case AST_LITERAL_STR:
            if (ast->literalstr.val)
                free(ast->literalstr.val);
            break;
        case AST_LITERAL_INT:
            break;
        }
        free(ast);
    }
}

// ---------------------------- helper function impl --------------------------
static jlang_ast *parse_compound(jlang_token **tok)
{
    jlang_ast *cmpd = malloc(sizeof(jlang_ast));
    if (cmpd == NULL)
        jlang_logFatal(1, "out of memory, malloc failed\n");

    cmpd->t = AST_COMPOUND;
    cmpd->compound.children = NULL;
    cmpd->compound.nchildren = 0;

    while ((*tok)->t != TOKEN_EOF)
    {
        cmpd->compound.nchildren++;
        jlang_ast **new_children =
            realloc(cmpd->compound.children,
                    sizeof(jlang_ast *) * cmpd->compound.nchildren);
        if (!new_children)
            jlang_logFatal(1, "out of memory, realloc failed\n");

        cmpd->compound.children = new_children;
        cmpd->compound.children[cmpd->compound.nchildren - 1] = parse_expr(tok);
    }

    return cmpd;
}

static jlang_ast *parse_expr(jlang_token **tok)
{
    jlang_ast *ast = NULL;
    switch ((*tok)->t)
    {
    case TOKEN_ID:
        ast = parse_id(tok);
        *tok = parser_eaa(*tok, TOKEN_SEMICOLON);
        break;
    case TOKEN_NUMBER:
        ast = malloc(sizeof(jlang_ast));
        if (!ast)
            jlang_logFatal(1, "out of memory\n");

        ast->t = AST_LITERAL_INT;
        ast->literalint.val = atoi((*tok)->val);

        *tok = parser_eaa(*tok, TOKEN_NUMBER);
        break;
    default:
        jlang_logFatal(1, "Unexpected token %s at %zu:%zu\n",
                       jlang_tokenNames[(*tok)->t], (*tok)->pos.line,
                       (*tok)->pos.col);
    }
    return ast;
}

static jlang_token *parser_eaa(jlang_token *tok, jlang_tokenType t)
{
    if (tok->t != t)
        jlang_logFatal(1, "jlang [ERROR] expected %s, got %s at %zu:%zu\n",
                       jlang_tokenNames[t], jlang_tokenNames[tok->t],
                       tok->pos.line, tok->pos.col);
    jlang_tokenFree(tok);
    return jlang_lexerNext();
}

static jlang_ast *parse_id(jlang_token **tok)
{
    char *idname = strdup((*tok)->val);

    *tok = parser_eaa(*tok, TOKEN_ID);
    // var or func definition
    if ((*tok)->t == TOKEN_COLON)
    {
        *tok = parser_eaa(*tok, TOKEN_COLON);
        // variable declaration
        if ((*tok)->t == TOKEN_ID)
        {
            char *vartype = strdup((*tok)->val);
            *tok = parser_eaa(*tok, TOKEN_ID);
            *tok = parser_eaa(*tok, TOKEN_EQUALS);

            jlang_ast *ast = malloc(sizeof(jlang_ast));
            if (ast == NULL)
                jlang_logFatal(1, "out of memory, malloc failed\n");

            ast->t = AST_ASSIGNMENT;
            ast->assignment.name = idname;
            ast->assignment.type = vartype;
            ast->assignment.value = parse_expr(tok);

            return ast;
        }

        // function definition
        *tok = parser_eaa(*tok, TOKEN_LPAREN);
        if ((*tok)->t == TOKEN_RPAREN) // no args
        {
            *tok = parser_eaa(*tok, TOKEN_RPAREN);
            char *rettype = strdup((*tok)->val);
            *tok = parser_eaa(*tok, TOKEN_ID);

            jlang_ast *ast = malloc(sizeof(jlang_ast));
            if (ast == NULL)
                jlang_logFatal(1, "out of memory, malloc failed\n");

            ast->t = AST_FUNCTION_DECL;
            ast->function_decl.name = idname;
            ast->function_decl.ret_type = rettype;

            *tok = parser_eaa(*tok, TOKEN_EQUALS);
            ast->function_decl.body = parse_compound(tok);

            return ast;
        }
    }

    // variable access or function call
    // TODO
    return NULL;
}
// ----------------------------------------------------------------------------
