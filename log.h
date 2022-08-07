#pragma once
#ifndef S_LOG_H
#define S_LOG_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define dowhile0(x) \
        do { x } \
        while ( 0 )
#define err \
        fprintf(stderr, " %s\n", strerror(errno))
#define __FL__ __FILE__, __LINE__
#define panic(...)     dowhile0( \
                           __stderr_log("panic", __FL__, __VA_ARGS__); \
                           exit(1); \
                       );
#define warn(...)      __stderr_log("warn", __FL__, __VA_ARGS__)
#define info(...)      __stderr_log("info", __FL__, __VA_ARGS__)

#define panicerr(...)   dowhile0( \
                            __stderr_log("panic", __FL__, __VA_ARGS__); \
                            err; \
                            exit(1); \
                        );
#define warnerr(...)    do { \
                            __stderr_log("warn", __FL__, __VA_ARGS__); \
                            err; \
                        } while (0);
#define infoerr(...)    dowhile0( \
                            __stderr_log("info", __FL__, __VA_ARGS__); \
                            err; \
                        );

void __stderr_log(const char *type, const char *file, const int line, const char *fmt, ...);

#endif /* S_LOG_H */
