#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "bf.h"
#include "log.h"

bf *bf_init(char *program) {
    bf *n =  calloc(1, sizeof(bf));
    n->program = program;
    memset(n->mem, 0, sizeof n->mem);
    n->dptr = n->mem;
    n->iptr = n->program;
    n->program_len = strlen(program);
    n->outlen = 0;
    n->outcap = 1024;
    n->out = malloc(n->outcap);
    n->out[0] = '\0';
    return n;
}

void bf_interpretone(bf *bf) {
    switch (*bf->iptr) {
        case '+':
            (*bf->dptr)++;
            break;
        case '-':
            (*bf->dptr)--;
            break;
        case '>':
            bf_next_cell(bf);
            break;
        case '<':
            bf_prev_cell(bf);
            break;
        case '[':
            if (bf_getmem(bf) != 0) {
                bf->loop = stack_push(bf->loop, bf->iptr);
            }
            else {
                char *t = strchr(bf->iptr, ']');
                if (t != NULL) {
                    bf->iptr = t;
                }
                else {
                    panic("[: no matching ]");
                }
            }
            break;
        case ']':
            if (bf_getmem(bf) != 0) {
                if (bf->loop->ptr == NULL)
                    panic("]: no matching [");
                bf->iptr = bf->loop->ptr;
            }
            else {
                if (bf->loop == NULL)
                    panic("]: no matching [");
                bf->loop = stack_pop(bf->loop);
            }
            break;
        case ',':
            bf_scan(bf);
            break;
        case '.':
            bf_print(bf);
            break;
        default:
            break;
    }
    bf->iptr++;
}

void bf_print(bf *bf) {
    if (!(bf->outlen < bf->outcap - 2))
        bf->out = realloc(bf->out, bf->outcap *= 2);
    snprintf(bf->out+(bf->outlen++), 2,
            "%c", bf_getmem(bf));
    fputc(bf_getmem(bf), stdout);
}

void bf_scan(bf *bf) {
    bf_setmem(bf, fgetc(stdin));
}

void bf_interpretall(bf *bf) {
    while (*bf->iptr != '\0')
        bf_interpretone(bf);
}

void bf_next_instruction(bf *bf) {
    bf->iptr++;
    bf_check_bound(bf);
}

void bf_prev_instruction(bf *bf) {
    bf->iptr--;
    bf_check_bound(bf);
}

void bf_next_cell(bf *bf) {
    bf->dptr++;
    bf_check_bound(bf);
}

void bf_prev_cell(bf *bf) {
    bf->dptr--;
    bf_check_bound(bf);
}

void bf_check_bound(bf *bf) {
    if (bf->iptr < bf->program || bf->iptr > bf->program + bf->program_len) {
        panic("instruction index out of bound (%d)", bf->iptr - bf->program_len);
    }
    if (bf->dptr < bf->mem || bf->dptr > bf->mem + MSIZE) {
        panic("data index out of bound (%d)", bf->dptr - bf->mem);
    }
}

unsigned char bf_getmem(bf *bf) {
    return *bf->dptr;
}

void bf_setmem(bf *bf, unsigned char n) {
    *bf->dptr = n;
}

unsigned char bf_memat(bf *bf, size_t idx) {
    if (idx < 0 || idx >= MSIZE) {
        warn("tried to access data out of bound (%d)", idx);
        return 0;
    }
    return bf->mem[idx];
}

