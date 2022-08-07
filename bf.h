#pragma once
#ifndef S_BF_H
#define S_BF_H

#include <stddef.h>
#include "stack.h"

#define MSIZE 30000

struct bf {
    char *iptr;
    Stack *loop;
    unsigned char mem[MSIZE];
    unsigned char *dptr;
    struct string *inst;
    struct string *out;
};

struct bf *bf_init(struct string *inst);

void bf_interpretone(struct bf *bf);

void bf_interpretall(struct bf *bf);

void bf_next_instruction(struct bf *bf);

void bf_prev_instruction(struct bf *bf);

void bf_check_bound(struct bf *bf);

void bf_next_cell(struct bf *bf);

void bf_prev_cell(struct bf *bf);

void bf_print(struct bf *bf);

void bf_scan(struct bf *bf);

unsigned char bf_getmem(struct bf *bf);

void bf_setmem(struct bf *bf, unsigned char n);

unsigned char bf_memat(struct bf *bf, size_t idx);

size_t bf_data_idx(struct bf *bf);

size_t bf_inst_idx(struct bf *bf);

#endif /* S_BF_H */
