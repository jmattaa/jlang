#ifndef _JLANG_LOGGER_H
#define _JLANG_LOGGER_H

#define JLANG_LOGGER_LOGLVLS_ITER(_X)                                          \
    _X(Info, "\x1b[34m", stdout, (const char *fmt, ...), {})                   \
    _X(Warn, "\x1b[33m", stderr, (const char *fmt, ...), {})                   \
    _X(Error, "\x1b[31m", stderr, (const char *fmt, ...), {})                  \
    _X(Fatal, "\x1b[31m", stderr, (int errcode, const char *fmt, ...),         \
       exit(errcode);)

#define _X(lvl, col, fd, sign, ...) void jlang_log##lvl sign;
JLANG_LOGGER_LOGLVLS_ITER(_X)
#undef _X

#endif
