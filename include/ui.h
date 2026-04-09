#ifndef UI_H_SENTRY
#define UI_H_SENTRY

#include "./point.h"

typedef struct screen {
    int row;
    int col;
} screen;

void ncurses_init(screen *scr);

void ncurses_free();

void handle_resize(screen *scr);

void print_cell(point *p, char);

void clear_cell(point *p);

void game_over(screen *scr);

#endif /* UI_H_SENTRY */
