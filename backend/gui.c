#include "gui.h"
#include "../utils.h"
#include "../xmalloc.h"
#include "../bf.h"
#include "../log.h"
#include "font.h"
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_rwops.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_ttf.h>

bool sdl_inited = false;

struct gui *
gui_init(struct bf *bf)
{
    struct gui *ui = xmalloc(sizeof(struct gui));
    ui->bf = bf;
    ui->offset = 0;
    ui->auto_run = false;
    ui->running = true;
    ui->w = 800; ui->h = 600;
    if (!sdl_inited)
    {
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
            warnerr("SDL_Init");
        if (TTF_Init() != 0)
            warnerr("TTF_Init");
        sdl_inited = true;
    }
    if ((ui->win = SDL_CreateWindow("Brainfuck Visualizer", 
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            ui->w, ui->h, 0)) == NULL)
        panicerr("Failed creating SDL window");
    if ((ui->rend = SDL_CreateRenderer(ui->win, -1, 0)) == NULL)
        panicerr("Failed creating SDL renderer");
    SDL_RWops *io = SDL_RWFromConstMem(font_ttf, font_ttf_size);
    if ((ui->font = TTF_OpenFontRW(io, 1, fontsize)) == NULL) 
        /* 1 as second parameter freesrc tell TTF_OpenFontRW to free (*io), so we don't have to do that */
        warnerr("Failed opening font from font_ttf[] size %u", fontsize);
    return ui;
}

void
gui_cleanup(struct gui *ui)
{
    TTF_CloseFont(ui->font);
    SDL_DestroyRenderer(ui->rend);
    SDL_DestroyWindow(ui->win);
    /*free(ui);*/
    SDL_Quit();
}

int
gui_right(struct gui *ui)
{
    if (ui->offset < MSIZE - ui->ncell)
        ui->offset++;
    else
        ui->offset = 0;
    return 0;
}

int
gui_left(struct gui *ui)
{
    if (ui->offset > 0)
        ui->offset--;
    else
        ui->offset = MSIZE - ui->ncell;
    return 0;
}

int
gui_update(struct gui *ui)
{
    if (ui->auto_run)
        bf_interpretone(ui->bf);
    return 0;
}

int
gui_input(struct gui *ui)
{
    SDL_Event ev;
    SDL_StartTextInput();
    while (SDL_PollEvent(&ev))
    {
        switch (ev.type)
        {
        case SDL_QUIT:
            ui->running = false;
            gui_cleanup(ui);
            return 0;
        case SDL_TEXTINPUT:
            switch (ev.text.text[0])
            {
            case 'i':
                bf_interpretone(ui->bf);
                break;
            case 'I':
                bf_interpretall(ui->bf);
                break;
            case 'a':
                gui_left(ui);
                break;
            case 'd':
                gui_right(ui);
                break;
            default:
                break;
            }
            break;
        case SDL_KEYDOWN:
            switch (ev.key.keysym.sym)
            {
            case SDLK_LEFT:
                gui_left(ui);
                break;
            case SDLK_RIGHT:
                gui_right(ui);
                break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            break;
        case SDL_WINDOWEVENT:
            if (ev.window.event == SDL_WINDOWEVENT_RESIZED) {
                ui->w = ev.window.data1;
                ui->h = ev.window.data2;
            }
            break;
        }
    }
    return 0;
}

int
gui_drawtext(struct gui *ui, const char *s, int x, int y, SDL_Color clr)
{
    SDL_Surface *surface;
    SDL_Texture *texture;
    surface = TTF_RenderText_Blended(ui->font, s, clr);
    texture = SDL_CreateTextureFromSurface(ui->rend, surface);
    SDL_Rect message_rect = { x, y, surface->w, surface->h };
    SDL_FreeSurface(surface);
    SDL_RenderCopy(ui->rend, texture, NULL, &message_rect);
    SDL_DestroyTexture(texture);
    return 0;
}

int
gui_render(struct gui *ui)
{
    SDL_SetRenderDrawColor(ui->rend, black);
    SDL_RenderClear(ui->rend);
    ui->ncell = (ui->w - 2*gpadx) / gcw;
    for (int i = 0; i < ui->ncell; i++)
    {
        size_t l = i + ui->offset;
        bool iscurrent = i+ ui->offset == bf_data_idx(ui->bf);
        int lpad = gpadx + i * gcw;
        int upad = gpady;
        SDL_SetRenderDrawColor(ui->rend, white);
        SDL_Color textclr = (SDL_Color){white};
        if (iscurrent) {
            SDL_SetRenderDrawColor(ui->rend, green);
            textclr = (SDL_Color){green};
        }
        gui_drawtext(ui, itoa(bf_memat(ui->bf, l)),
                lpad+5, upad+5, textclr);
        gui_drawtext(ui, itoa(l),
                lpad+5, upad+5+gcw, textclr);
        SDL_Rect r = {lpad, upad, gcw, gcw};
        SDL_RenderDrawRect(ui->rend, &r);
    }
    
    SDL_RenderPresent(ui->rend);
    return 0;
}

int
gui_run(struct gui *ui)
{
    for (;;)
    {
        gui_update(ui);
        gui_input(ui);
        if (!ui->running)
            return 0;
        gui_render(ui);
        SDL_Delay(1000/60);
    }
}

