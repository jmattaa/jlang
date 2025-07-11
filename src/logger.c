#include "logger.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define _X(lvl, col, fd, sign, err)                                            \
    void jlang_log##lvl sign                                                   \
    {                                                                          \
        fprintf(fd, col "jlang [" #lvl "]:\x1b[0m ");                          \
        va_list args;                                                          \
        va_start(args, fmt);                                                   \
        vfprintf(fd, fmt, args);                                               \
        va_end(args);                                                          \
        err                                                                    \
    }
JLANG_LOGGER_LOGLVLS_ITER(_X)
#undef _X
