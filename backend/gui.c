#include "gui.h"
#include "../xmalloc.h"
 
struct gui *
gui_init(struct bf *bf)
{
    struct gui *ui = xmalloc(sizeof(struct gui));
    //ui->win = SDL_CreateWindow()
    ui->bf = bf;
    ui->offset = 0;
    ui->run = false;
    return ui;
}


