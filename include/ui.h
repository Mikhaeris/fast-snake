#ifndef UI_H_SENTRY
#define UI_H_SENTRY

#include "./point.h"

typedef struct screen {
    int row;
    int col;
} screen;

void ncurses_init(screen *scr);

void ncurses_free();

void print_cell(const point *p, char);

void clear_cell(const point *p);

void print_msg_exit(const screen *scr, const char *msg);

#endif /* UI_H_SENTRY */
