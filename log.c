#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#define va(x) \
        va_list a; \
        va_start(a, fmt); \
        x; \
        va_end(a); 

void
__stderr_log(const char *type, const char *file
        , const int line, const char *fmt
        , ...)
{
    fprintf(stderr, "%s: %s:%d: ", type, file, line);
    va(vfprintf(stderr, fmt, a));
    fprintf(stderr, "\n");
}
