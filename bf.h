#pragma once
#include <stddef.h>
#include "stack.h"

#define MSIZE 30000

typedef struct {
    char *program;
    char *iptr;
    Stack *loop;
    unsigned char mem[MSIZE];
    size_t outcap, outlen;
    char *out;
    unsigned char *dptr;
    size_t program_len;
} bf;

bf *bf_init(char *program);

void bf_interpretone(bf *bf);

void bf_interpretall(bf *bf);

void bf_next_instruction(bf *bf);

void bf_prev_instruction(bf *bf);

void bf_check_bound(bf *bf);

void bf_next_cell(bf *bf);

void bf_prev_cell(bf *bf);

void bf_print(bf *bf);

void bf_scan(bf *bf);

unsigned char bf_getmem(bf *bf);

void bf_setmem(bf *bf, unsigned char n);

unsigned char bf_memat(bf *bf, size_t idx);

