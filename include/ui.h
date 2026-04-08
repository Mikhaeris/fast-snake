#ifndef UI_H_SENTRY
#define UI_H_SENTRY

#include "./point.h"

typedef struct screen {
    int row;
    int col;
} screen;

void print_cell(point *p, char);

void clear_cell(point *p);

#endif /* UI_H_SENTRY */
