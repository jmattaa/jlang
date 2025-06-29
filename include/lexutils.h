#ifndef _JLANG_LEXUTILS_H
#define _JLANG_LEXUTILS_H

#include <stddef.h>

typedef struct
{
    size_t line, col;
    size_t srcidx;
} pos_t;

#endif
