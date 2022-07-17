#pragma once
#include <stddef.h>

void *check_alloc(void *ptr);

void *xmalloc(size_t size);
void *xcalloc(size_t nmemb, size_t size);
void *xrealloc(void *ptr, size_t size);


