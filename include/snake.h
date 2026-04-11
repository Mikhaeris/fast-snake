#ifndef SNAKE_H_SENTRY
#define SNAKE_H_SENTRY

#include <stddef.h>
#include <stdint.h>

#include "./ui.h"
#include "./point.h"
#include "./apple.h"
#include "./game_status.h"

#define SNAKE_SYMBOL '#'
#define SNAKE_HEAD_SYMBOL '@'

typedef struct snake {
    uint8_t grow_up;
    struct {
        int dx;
        int dy;
    };
    size_t head;
    size_t tail;
    point *circle_buf;
    size_t circle_buf_size;
    uint8_t *collision_mask;
} snake;

snake *snake_init(const screen *scr);

void snake_free(snake *s);

void snake_grow_up(snake *s);

game_status snake_move(snake *s, apple *a, const screen *scr);

void set_direction(snake *s, int dx, int dy);

uint8_t check_apple_collision(apple *a, snake *s);

#endif /* SNAKE_H_SENTRY */
