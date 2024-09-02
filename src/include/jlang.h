#ifndef JLANG_H
#define JLANG_H

#include "IO.h"
#include "asm_frontend.h"
#include "lexer.h"
#include "parser.h"

// compile the src
void Jlang_Compile(char* src, char* outfile);

#endif
