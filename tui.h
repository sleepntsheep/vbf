#pragma once

#include "bf.h"

struct tui {
    struct bf *bf;
    unsigned offset, ncell;
    unsigned w, h;
};

struct tui *tui_init(struct bf *bf);

int tui_run(struct tui *ui);



