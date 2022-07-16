#include <stdnoreturn.h>

#define panic(...) _panic(__FILE__, __LINE__, __VA_ARGS__)
#define warn(...)  _warn(__FILE__, __LINE__, __VA_ARGS__)
#define info(...)  _info(__FILE__, __LINE__, __VA_ARGS__)

noreturn void _panic(const char *file, const int line, const char *fmt, ...);
void _warn(const char *file, const int line, const char *fmt, ...);
void _info(const char *file, const int line, const char *fmt, ...);
