#include "parser.h"
#include "ast.h"
#include "lexer.h"
#include "logger.h"
#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ------------------------ helper function decl ------------------------------
static jlang_ast *parse_compound();
static jlang_ast *parse_expr(jlang_token *tok);
static jlang_token *parser_eaa(jlang_token *tok,
                               jlang_tokenType t); // expect and advance
static jlang_ast *parse_id(jlang_token *tok);
// ----------------------------------------------------------------------------

// the program is a compound
jlang_ast *jlang_parse() { return parse_compound(); }

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
        case AST_LITERAL:
            if (ast->literal.val)
                free(ast->literal.val);
            break;
        }
        free(ast);
    }
}

// ---------------------------- helper function impl --------------------------
static jlang_ast *parse_compound()
{
    jlang_ast *cmpd = malloc(sizeof(jlang_ast));
    if (cmpd == NULL)
        jlang_logFatal(1, "out of memory, malloc failed\n");
    cmpd->t = AST_COMPOUND;
    cmpd->compound.children = NULL;
    cmpd->compound.nchildren = 0;

    jlang_token *tok;
    while ((tok = jlang_lexerNext())->t != TOKEN_EOF)
    {
        cmpd->compound.nchildren++;
        if (cmpd->compound.children == NULL)
        {
            cmpd->compound.children = malloc(sizeof(jlang_ast *));
            if (cmpd->compound.children == NULL)
                jlang_logFatal(1, "out of memory, malloc failed\n");
        }
        else
        {
            cmpd->compound.children =
                realloc(cmpd->compound.children,
                        sizeof(jlang_ast *) * cmpd->compound.nchildren);
            if (cmpd->compound.children == NULL)
                jlang_logFatal(1, "out of memory, realloc failed\n");
        }

        cmpd->compound.children[cmpd->compound.nchildren - 1] = parse_expr(tok);

        jlang_tokenFree(tok);
    }

    return cmpd;
}

static jlang_ast *parse_expr(jlang_token *tok)
{
    switch (tok->t)
    {
    case TOKEN_ID:
        return parse_id(tok);
    case TOKEN_LPAREN:
        jlang_logInfo("lparen\n");
        return NULL; // parse list
    case TOKEN_NUMBER:
        jlang_logInfo("number: %s\n", tok->val);
        return NULL; // parse number
    default:
        jlang_logInfo("something else");
        return NULL;
        // jlang_logFatal(1, "jlang [ERROR] unexpected token");
    }

    // every expression must end with ;
    jlang_tokenFree(parser_eaa(tok, TOKEN_SEMICOLON));
}

static jlang_token *parser_eaa(jlang_token *tok, jlang_tokenType t)
{
    if (tok->t != t)
        jlang_logFatal(1, "jlang [ERROR] expected %s, got %s\n",
                       jlang_tokenNames[t], jlang_tokenNames[tok->t]);
    jlang_tokenFree(tok);
    return jlang_lexerNext();
}

static jlang_ast *parse_id(jlang_token *tok)
{
    char *idname = strdup(tok->val);

    tok = parser_eaa(tok, TOKEN_ID);
    // var or func definition
    if (tok->t == TOKEN_COLON)
    {
        tok = parser_eaa(tok, TOKEN_COLON);
        char *vartype = strdup(tok->val);
        tok = parser_eaa(tok, TOKEN_ID);
        // variable declaration
        if (tok->t == TOKEN_EQUALS)
        {
            tok = parser_eaa(tok, TOKEN_EQUALS);
            jlang_ast *ast = malloc(sizeof(jlang_ast *));
            if (ast == NULL)
                jlang_logFatal(1, "out of memory, malloc failed\n");
            ast->t = AST_ASSIGNMENT;

            ast->assignment.name = idname;
            ast->assignment.type = vartype;
            ast->assignment.value = parse_expr(tok);

            return ast;
        }

        // function definition
        if (vartype != NULL && vartype[0] == '(')
        {
            jlang_ast *ast = malloc(sizeof(jlang_ast *));
            if (ast == NULL)
                jlang_logFatal(1, "out of memory, malloc failed\n");
            ast->t = AST_FUNCTION_DECL;
            ast->function_decl.name = idname;
            ast->function_decl.ret_type = vartype;
            ast->function_decl.body = parse_compound();
            return ast;
        }
    }
    // variable access or function call
    // TODO
    return NULL;
}
// ----------------------------------------------------------------------------
