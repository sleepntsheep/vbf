#include <stdint.h>
#include <stdlib.h>
#include <ncurses.h>
#include <stdbool.h>
#include "tui.h"

/* datas */
struct tui_button {
    char *text;
    int x, y, w, h;
    int slen;
};

struct tui *
tui_init(struct bf *bf)
{
    struct tui *ui = xmalloc(sizeof(struct tui));
    ui->bf = bf;
    ui->offset = 0;
    ui->run = false;
    return ui;
}

/* vars */
const static struct tui_button nextbutton = (struct tui_button) { .text = "next", .x = 0,  .y = 5, .w = 6, .h = 2 };
const static struct tui_button rightbutton= (struct tui_button) { .text = ">",    .x = 11, .y = 5, .w = 2, .h = 2 };
const static struct tui_button leftbutton = (struct tui_button) { .text = "<",    .x = 8,  .y = 5, .w = 2, .h = 2 };
const static struct tui_button runbutton  = (struct tui_button) { .text = "auto", .x = 15, .y = 5, .w = 6, .h = 2 };

/* functions declaration */
void tui_left(struct tui *ui);
void tui_right(struct tui *ui);
void tui_draw_instruction(struct tui *ui);
void tui_button_draw(struct tui_button btn);
bool tui_button_isover(struct tui_button btn, MEVENT m);
void rect(WINDOW *win, int y1, int x1, int y2, int x2);
int tui_draw(struct tui *ui);
int tui_run(struct tui *ui);
int tui_auto(struct tui *ui);

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
    int truncate = ui->w - 10, n;
    char *curr = ui->bf->iptr;
    size_t idx = bf_inst_idx(ui->bf);
    mvprintw(y, 0, "instruction: %n", &n);
    if (idx > truncate)
        mvprintw(y, n, "%s", curr - truncate);
    else 
        mvprintw(y, n, "%s", ui->bf->inst->b);
    if (curr == NULL) return;
    attron(A_REVERSE);
    mvprintw(y, n+idx, "%c", *curr);
    attroff(A_REVERSE);
}

void
tui_button_draw(struct tui_button btn)
{
    attron(A_REVERSE);
    for (int i = 0; i < btn.h; i++) {
        mvprintw(i+btn.y, btn.x, "%.*s", btn.w, "                                                                                                ");
        if (i == btn.h / 2)
            mvprintw(i+btn.y, btn.x, "%.*s", btn.w, btn.text);
    }
    attroff(A_REVERSE);
}

bool
tui_button_isover(struct tui_button btn, MEVENT m)
{
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
tui_draw(struct tui *ui)
{
    erase();
    getmaxyx(stdscr, ui->h, ui->w);

    ui->ncell = ui->w / 7;

    /* draw cells */
    for (int i = 0; i < ui->ncell; i++) {
        size_t midx = i + ui->offset;
        bool iscurrent = midx == bf_data_idx(ui->bf);
        if (iscurrent)
            attron(COLOR_PAIR(2));
        rect(stdscr, 0, i * 7, 2, i * 7 + 6);
        mvprintw(1, i * 7 + 2, "%3d", bf_memat(ui->bf, midx));
        mvprintw(3, i * 7 + 1, "%5ld", midx);
        if (iscurrent)
            attroff(COLOR_PAIR(2));
    }

    tui_button_draw(nextbutton);
    tui_button_draw(rightbutton);
    tui_button_draw(leftbutton);
    if (ui->run)
        attron(COLOR_PAIR(2));
    tui_button_draw(runbutton);
    attroff(COLOR_PAIR(2));

    /* draw output */
    tui_draw_instruction(ui);

    mvprintw(11, 0, "output : %s", ui->bf->out->b);

    refresh();

    return 0;
}

int
tui_run(struct tui *ui)
{
    int ch;
    mmask_t old;
    initscr();
    noecho();
    curs_set(0);
    start_color();
    init_pair(2,  COLOR_GREEN, COLOR_BLACK);
    mousemask (ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, &old);
    clear();
    refresh();
    keypad(stdscr, true);
    nodelay(stdscr, true);

    for (;;) {
        if (ui->run) 
            bf_interpretone(ui->bf);

        tui_draw(ui);
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
                if (tui_button_isover(nextbutton, mev))
                    bf_interpretone(ui->bf);
                if (tui_button_isover(rightbutton, mev))
                    tui_right(ui);
                if (tui_button_isover(leftbutton, mev))
                    tui_left(ui);
                if (tui_button_isover(runbutton, mev))
                    ui->run ^= 1;
            }
            default:
                break;
        }
        napms(10);
    }

    return 0;
}

