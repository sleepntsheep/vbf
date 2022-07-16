#include <stdlib.h>
#include <ncurses.h>
#include <stdbool.h>
#include "log.h"
#include "bf.h"
#include "tui.h"

struct tui *
tui_init(bf *bf)
{
    struct tui *ui = malloc(sizeof(struct tui*));
    ui->bf = bf;
    ui->offset = 0;
    return ui;
}

void
tui_left(struct tui *ui)
{
    if (ui->offset > 0)
        ui->offset--;
    else
        ui->offset = MSIZE - ui->ncell;
}

void
tui_right(struct tui *ui)
{
    if (ui->offset < MSIZE - ui->ncell)
        ui->offset++;
    else
        ui->offset = 0;
}

void
tui_draw_instruction(struct tui *ui)
{
    const int y = 8;
    int truncate = ui->w - 10;
    char *curr = ui->bf->iptr;
    char c = *curr;
    int idx = curr - ui->bf->program;
    int n;
    mvprintw(y, 0, "program: %n", &n);
    if (idx > truncate)
        mvprintw(y, n, "%s", curr - truncate);
    else 
        mvprintw(y, n, "%s", ui->bf->program);
    if (curr == NULL) return;
    attron(A_REVERSE);
    mvprintw(y, n+idx, "%c", *curr);
    attroff(A_REVERSE);
}

struct mousebutton {
    char *text;
    int x, y, w, h;
    int slen;
};

void
mousebutton_draw(struct mousebutton btn) {
    attron(A_REVERSE);
    for (int i = 0; i < btn.h; i++) {
        mvprintw(i+btn.y, btn.x, "%.*s", btn.w, "                                                                                                ");
        if (i == btn.h / 2)
            mvprintw(i+btn.y, btn.x, "%.*s", btn.w, btn.text);
    }
    attroff(A_REVERSE);
}

bool
mousebutton_isover(struct mousebutton btn, MEVENT m) {
    return m.x >= btn.x &&
           m.y >= btn.y &&
           m.x <= btn.x + btn.w &&
           m.y <= btn.y + btn.h;
}

void
rect(WINDOW *win, int y1, int x1, int y2, int x2)
{
    mvwhline(win, y1, x1, 0, x2-x1);
    mvwhline(win, y2, x1, 0, x2-x1);
    mvwvline(win, y1, x1, 0, y2-y1);
    mvwvline(win, y1, x2, 0, y2-y1);
    mvwaddch(win, y1, x1, ACS_ULCORNER);
    mvwaddch(win, y2, x1, ACS_LLCORNER);
    mvwaddch(win, y1, x2, ACS_URCORNER);
    mvwaddch(win, y2, x2, ACS_LRCORNER);
}

int
tui_run(struct tui *ui)
{
    int ch;
    mmask_t old;
    initscr();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    start_color();
    init_pair(2,  COLOR_GREEN, COLOR_BLACK);
    mousemask (ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, &old);
    clear();
    refresh();

    struct mousebutton nextbutton = (struct mousebutton) { .text = "next", .x = 0,  .y = 5, .w = 6, .h = 2 };
    struct mousebutton rightbutton= (struct mousebutton) { .text = ">",    .x = 11, .y = 5, .w = 2, .h = 2 };
    struct mousebutton leftbutton = (struct mousebutton) { .text = "<",    .x = 7,  .y = 5, .w = 2, .h = 2 };

    for (;;) {
        erase();
        getmaxyx(stdscr, ui->h, ui->w);

        ui->ncell = ui->w / 7;

        /* draw cells */
        for (int i = 0; i < ui->ncell; i++) {
            bool iscurrent = ui->bf->mem + i + ui->offset == ui->bf->dptr;
            if (iscurrent)
                attron(COLOR_PAIR(2));
            rect(stdscr, 0, i * 7, 2, i * 7 + 6);
            mvprintw(1, i * 7 + 2, "%3d", bf_memat(ui->bf, i+ui->offset));
            mvprintw(3, i * 7 + 1, "%5d", i+ui->offset);
            if (iscurrent)
                attroff(COLOR_PAIR(2));
        }

        mousebutton_draw(nextbutton);
        mousebutton_draw(rightbutton);
        mousebutton_draw(leftbutton);

        /* draw output */
        tui_draw_instruction(ui);

        mvprintw(11, 0, "output : %s", ui->bf->out);

        refresh();
        ch = getch();

        switch (ch) {
            case 'i':
                bf_interpretone(ui->bf);
                break;
            case 'I':
                bf_interpretall(ui->bf);
                break;
            case 'a':
            case KEY_LEFT:
                tui_left(ui);
                break;
            case 'd':
            case KEY_RIGHT:
                tui_right(ui);
                break;
            case KEY_MOUSE:
            {
                MEVENT mev;
                if ((getmouse(&mev)) != OK)
                    warn("getmouse: error getting mouse event");
                if (mousebutton_isover(nextbutton, mev))
                    bf_interpretone(ui->bf);
                if (mousebutton_isover(rightbutton, mev))
                    tui_right(ui);
                if (mousebutton_isover(leftbutton, mev))
                    tui_left(ui);
            }
            default:
                break;
        }
    }

    return 0;
}

