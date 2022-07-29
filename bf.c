#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "str.h"
#include "bf.h"
#include "log.h"
#include "xmalloc.h"

struct bf *bf_init(struct string *inst) {
    struct bf *n =  xcalloc(1, sizeof(struct bf));
    n->inst = inst;
    memset(n->mem, 0, sizeof n->mem);
    n->dptr = n->mem;
    n->iptr = n->inst->b;
    n->inst = inst;
    n->out = str_init();
    return n;
}

void bf_interpretone(struct bf *bf) {
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

void bf_print(struct bf *bf) {
    char c = bf_getmem(bf);
    str_npush(bf->out, &c, 1);
    fputc(c, stdout);
    fflush(stdout);
}

void bf_scan(struct bf *bf) {
    bf_setmem(bf, fgetc(stdin));
}

void bf_interpretall(struct bf *bf) {
    while (*bf->iptr != '\0')
        bf_interpretone(bf);
}

void bf_next_instruction(struct bf *bf) {
    bf->iptr++;
    bf_check_bound(bf);
}

void bf_prev_instruction(struct bf *bf) {
    bf->iptr--;
    bf_check_bound(bf);
}

void bf_next_cell(struct bf *bf) {
    bf->dptr++;
    bf_check_bound(bf);
}

void bf_prev_cell(struct bf *bf) {
    bf->dptr--;
    bf_check_bound(bf);
}

void bf_check_bound(struct bf *bf) {
    size_t inst_index = bf_inst_idx(bf);
    if (inst_index < 0 || inst_index >= bf->inst->l)
        panic("instruction index out of bound (%d)", inst_index);
    size_t data_index = bf_data_idx(bf);
    if (data_index < 0 || data_index >= MSIZE)
        panic("data index out of bound (%d)", data_index);
}

unsigned char bf_getmem(struct bf *bf) {
    return *bf->dptr;
}

void bf_setmem(struct bf *bf, unsigned char n) {
    *bf->dptr = n;
}

unsigned char bf_memat(struct bf *bf, size_t idx) {
    if (idx < 0 || idx >= MSIZE) {
        warn("tried to access data out of bound (%d)", idx);
        return 0;
    }
    return bf->mem[idx];
}

size_t bf_data_idx(struct bf *bf) {
    return bf->dptr - bf->mem;
}

size_t bf_inst_idx(struct bf *bf) {
    return bf->iptr - bf->inst->b;
}

