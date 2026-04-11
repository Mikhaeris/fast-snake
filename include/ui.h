#ifndef UI_H_SENTRY
#define UI_H_SENTRY

#include "./point.h"

typedef struct screen {
    int row;
    int col;
} screen;

screen *ncurses_init();

void ncurses_free();

void print_cell(const point *p, char);

void clear_cell(const point *p);

void print_msg(const screen *scr, const char *msg);

#endif /* UI_H_SENTRY */
