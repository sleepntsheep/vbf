#pragma once

#define LENGTH(a) (sizeof(a) / sizeof(*(a)))
#ifndef itoa
char *itoa(int i);
#endif
