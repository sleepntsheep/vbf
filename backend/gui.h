#pragma once

#include <stdbool.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>

#define black 0,0,0,255
#define white 255,255,255,255
#define green 119,221,119,255

#define gpadx 20
#define gpady 20
#define gcw 100
#define fontsize 40

struct gui {
    SDL_Window *win;
    SDL_Renderer *rend;
    TTF_Font *font;
    struct bf *bf;
    unsigned offset, ncell;
    unsigned w, h;
    bool auto_run, running;
};

struct gui *gui_init(struct bf *bf);
void gui_cleanup(struct gui *ui);
int gui_run(struct gui *ui);
int gui_update(struct gui *ui);
int gui_input(struct gui *ui);
int gui_left(struct gui *ui);
int gui_right(struct gui *ui);
int gui_render(struct gui *ui);
