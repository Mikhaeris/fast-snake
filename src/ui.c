#include "../include/ui.h"

#include "../include/constants.h"

#include <curses.h>

#include <stdlib.h>
#include <string.h>

void ncurses_init(screen *scr)
{
    initscr();
    cbreak();
    timeout(delay_duration);
    keypad(stdscr, 1);
    noecho();
    curs_set(0);

    getmaxyx(stdscr, scr->row, scr->col);
}

void ncurses_free()
{
    endwin();
}

void print_cell(const point *p, char ch)
{
    move(p->y, p->x);
    addch(ch);
}

void clear_cell(const point *p)
{
    move(p->y, p->x);
    addch(' ');
}

void print_msg_exit(const screen *scr, const char *msg)
{
    for (int i = 0; i < scr->row; i++) {
        for (int j = 0; j < scr->col; j++) {
            move(i, j);
            addch(' ');
        }
    }
    refresh();

    move(scr->row / 2, (scr->col - strlen(msg) - 1) / 2);
    wprintw(stdscr, "%s", msg);

    int key;
    while ((key = getch()) != key_escape)
        {}
    ncurses_free();
    exit(0);
}
