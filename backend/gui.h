#pragma once

#include <stdbool.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>

#define RGBA(x) (x).r,(x).g,(x).b,(x).a
#define black 0,0,0,255
#define white 255,255,255,255
#define green 119,221,119,255
#define gray 50,50,50,255

#define gwidth 800
#define gheight 600
#define gpadx 20
#define gpady 20
#define gcw 100
#define fontsize 36

struct gui {
    SDL_Window *win;
    SDL_Renderer *rend;
    TTF_Font *font;
    struct bf *bf;
    unsigned offset, ncell;
    unsigned w, h;
    bool auto_run, running;
};

struct color {
    uint8_t r, g, b, a;
};

struct gui_button {
    char *text;
    int x, y, w, h;
    struct color bg, fg;
};

struct gui *gui_init(struct bf *bf);
void gui_cleanup(struct gui *ui);
int gui_run(struct gui *ui);
int gui_update(struct gui *ui);
int gui_input(struct gui *ui);
int gui_left(struct gui *ui);
int gui_right(struct gui *ui);
int gui_render(struct gui *ui);
void gui_button_draw(struct gui *ui, struct gui_button b);
bool gui_button_isover(struct gui_button btn, int x, int y);

