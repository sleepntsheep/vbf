#include <stdlib.h>
#include <string.h>
#include "str.h"
#include "xmalloc.h"

struct string *str_init() {
    struct string *s = xmalloc(sizeof(struct string*));
    s->l = 0;
    s->c = 512;
    s->b = xcalloc(s->c, 1);
    return s;
}

struct string *str_from_charp(char *p) {
    struct string *str = str_init();
    str_push(str, p);
    return str;
}

void str_push(struct string *s, char *n) {
    int nlen = strlen(n);
    if (s->l + nlen >= s->c) {
        while (s->l + nlen >= s->c)
            s->c *= 2;
        s->b = xrealloc(s->b, s->c);
    }
    strcpy(s->b+s->l, n);
    s->l += nlen;
}

void str_npush(struct string *s, char *n, size_t len) {
    if (s->l + len >= s->c) {
        while (s->l + len >= s->c)
            s->c *= 2;
        s->b = xrealloc(s->b, s->c);
    }
    strncpy(s->b+s->l, n, len);
    s->l += len;
}

