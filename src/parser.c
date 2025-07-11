#include "parser.h"
#include "ast.h"
#include "lexer.h"
#include "logger.h"
#include "token.h"
#include <stdio.h>
#include <stdlib.h>

// ------------------------ helper function decl ------------------------------
static jlang_ast *parse_compound();
static jlang_ast *parse_expr(jlang_token *tok);
// ----------------------------------------------------------------------------

// the program is a compound
jlang_ast *jlang_parse() { return parse_compound(); }

void jlang_freeAst(jlang_ast *ast)
{
    if (ast && ast->t == AST_COMPOUND)
    {
        for (size_t i = 0; i < ast->compound.nchildren; i++)
            jlang_freeAst(ast->compound.children[i]);
        if (ast->compound.children)
            free(ast->compound.children);
    }
    if (ast)
        free(ast);
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
        jlang_logInfo("id: %s\n", tok->val);
        return NULL; // parse identifier
    case TOKEN_LPAREN:
        jlang_logInfo("lparen\n");
        return NULL; // parse list
    case TOKEN_NUMBER:
        jlang_logInfo("number: %s\n", tok->val);
        return NULL; // parse number
    default:
        jlang_logInfo("something else\n");
        return NULL;
        // jlang_logFatal(1, "jlang [ERROR] unexpected token");
    }
}
// ----------------------------------------------------------------------------
