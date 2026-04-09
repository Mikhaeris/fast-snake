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

void game_over(screen *scr)
{
    for (size_t i = 0; i < scr->row; i++) {
        for (size_t j = 0; j < scr->col; j++) {
            move(i, j);
            addch(' ');
        }
    }
    refresh();

    const char msg_go[] = "Game over!";
    move(scr->row / 2, (scr->col - sizeof(msg_go) - 1) / 2);
    wprintw(stdscr, "%s", msg_go);

    int key;
    while ((key = getch()) != key_escape)
        {}
    ncurses_free();
    exit(0);
}
