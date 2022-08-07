#pragma once
#ifndef S_STR_H
#define S_STR_H

#include <stddef.h>

struct string {
    char *b; // buffer
    size_t c, l; // capacity, length
};

struct string *str_init();

struct string *str_from_charp(char *p);

void str_push(struct string *s, char *n);

void str_npush(struct string *s, char *n, size_t len);

#endif /* S_STR_H */
