#pragma once

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>

struct gui {
    SDL_Window *win;
    SDL_Window *rend;
    struct bf *bf;
    unsigned offset, ncell;
    unsigned w, h;
    bool run;
};

struct gui *gui_init(struct bf *bf);

