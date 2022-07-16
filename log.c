#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdnoreturn.h>

void
__stderr_log(const char *type, const char *file
        , const int line, const char *fmt
        , va_list arg)
{
    fprintf(stderr, "%s: %s:%d: ", type, file, line);
    vfprintf(stderr, fmt, arg);
    fprintf(stderr, "\n");
}

noreturn void
_panic(const char *file, const int line
        , const char *fmt, ...)
{
    va_list a;
    va_start(a, fmt);
    __stderr_log("panic", file, line, fmt, a);
    va_end(a);
    exit(EXIT_FAILURE);
}

void 
_warn(const char *file, const int line
        , const char *fmt, ...)
{
    va_list a;
    va_start(a, fmt);
    __stderr_log("warn", file, line, fmt, a);
    va_end(a);
}

void 
_info(const char *file, const int line
        , const char *fmt, ...)
{
    va_list a;
    va_start(a, fmt);
    __stderr_log("info", file, line, fmt, a);
    va_end(a);
}

