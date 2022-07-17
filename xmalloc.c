#include <stdlib.h>
#include "xmalloc.h"
#include "log.h"

void *check_alloc(void *ptr) {
    if (ptr == NULL)
        panic("allocation failed");
    return ptr;
}

void *xmalloc(size_t size) {
    return check_alloc(malloc(size));
}

void *xcalloc(size_t nmemb, size_t size) {
    return check_alloc(calloc(nmemb, size));
}

void *xrealloc(void *ptr, size_t size) {
    return check_alloc(realloc(ptr, size));
}



