#pragma once

#include "bf.h"

struct tui {
    bf *bf;
    unsigned offset, ncell;
    unsigned w, h;
};

struct tui *tui_init(bf *bf);

int tui_run(struct tui *ui);



