#pragma once
#ifndef S_XMALLOC_H
#define S_XMALLOC_H

#include <stddef.h>

void *check_alloc(void *ptr);

void *xmalloc(size_t size);
void *xcalloc(size_t nmemb, size_t size);
void *xrealloc(void *ptr, size_t size);

#endif /* S_XMALLOC_H */
