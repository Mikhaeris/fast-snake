#include "../include/ui.h"

#include <curses.h>

void print_cell(point *p, char ch)
{
    move(p->y, p->x);
    addch(ch);
    refresh();
}

void clear_cell(point *p)
{
    move(p->y, p->x);
    addch('&');
    refresh();
}
