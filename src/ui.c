#include "../include/ui.h"

#include <curses.h>

#include "../include/constants.h"

void init_ncurses(screen *scr)
{
    initscr();
    cbreak();
    timeout(delay_duration);
    keypad(stdscr, 1);
    noecho();
    curs_set(0);

    getmaxyx(stdscr, scr->row, scr->col);
}

void handle_resize(screen *scr)
{
    getmaxyx(stdscr, scr->row, scr->col);
}

void print_cell(point *p, char ch)
{
    move(p->y, p->x);
    addch(ch);
    refresh();
}

void clear_cell(point *p)
{
    move(p->y, p->x);
    addch(' ');
    refresh();
}
