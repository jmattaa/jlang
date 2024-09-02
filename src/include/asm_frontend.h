#ifndef JLANG_ASM_FRONTEND_H
#define JLANG_ASM_FRONTEND_H

#include "AST.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* ASMFrontend(AST* ast, dynlist* list);
char* ASMFrontend_Root(AST* ast, dynlist* list);

#endif
