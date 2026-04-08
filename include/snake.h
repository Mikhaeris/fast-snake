#ifndef SNAKE_H_SENTRY
#define SNAKE_H_SENTRY

#include <stddef.h>
#include <stdint.h>

#include "./ui.h"
#include "./point.h"

#define SNAKE_SYMBOL '#'
#define SNAKE_HEAD_SYMBOL '@'

typedef struct snake {
    struct {
        int dx;
        int dy;
    };
    size_t length;
    size_t head;
    size_t tail;
    point *buf;
    size_t buf_size;
    uint8_t *collision_mask;
    struct {
        int row;
        int col;
    };
} snake;

snake *snake_init(screen *scr, point *p);

void snake_resize(snake *s, screen *old_scr, screen *new_scr);

void snake_move(snake *s, screen *scr, int flag);

void set_direction(snake *s, int dx, int dy);

#endif /* SNAKE_H_SENTRY */
