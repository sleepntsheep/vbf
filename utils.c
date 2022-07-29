#include "utils.h"
#include "xmalloc.h"
#include <stdio.h>

char *itoa(int i) {
  char *res = xmalloc(50);
  res[snprintf(res, 50, "%d", i)] = '\0';
  return res;
}

