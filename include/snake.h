#ifndef SNAKE_H_SENTRY
#define SNAKE_H_SENTRY

#include <stddef.h>
#include <stdint.h>

#include "./ui.h"
#include "./point.h"

#define SNAKE_SYMBOL '#'

typedef struct direction {
    int x;
    int y;
} direction;

enum { def_buf_size = 10 };

typedef struct snake {
    size_t head;
    size_t tail;
    point *buf;
    uint8_t *collision_mask;
} snake;

snake *snake_init(screen *scr, point *p);

void snake_update(snake *s, int new_row, int new_col);

void snake_move(snake *s, direction *dr);

void set_direction(direction *dr, int dx, int dy);

#endif /* SNAKE_H_SENTRY */
