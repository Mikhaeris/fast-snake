#ifndef APPLE_H_SENTRY
#define APPLE_H_SENTRY

#include "./snake.h"

#include <stdint.h>

typedef struct apple {
    point apple_p;
    point *cells;
    size_t k;
    size_t *pos;
} apple;

void apple_generate(apple *a, snake *s);

void apple_update_cells(apple *a, point *p, screen *scr, uint8_t flag);

apple *apple_init(snake *s, screen *scr);

uint8_t check_apple_collision(apple *a, snake *s);

#endif /* APPLE_H_SENTRY */
