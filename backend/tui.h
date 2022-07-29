#ifdef HAVE_NCURSES
#pragma once

#include <stdbool.h>
#include "../log.h"
#include "../str.h"
#include "../xmalloc.h"
#include "../bf.h"

struct tui {
    struct bf *bf;
    unsigned offset, ncell;
    unsigned w, h;
    bool run;
};

struct tui *tui_init(struct bf *bf);

int tui_run(struct tui *ui);

#endif
