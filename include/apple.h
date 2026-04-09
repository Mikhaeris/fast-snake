#ifndef APPLE_H_SENTRY
#define APPLE_H_SENTRY

#include "./snake.h"

#include <stdint.h>

typedef struct apple {
    point apple_p;
} apple;

void apple_generate(apple *a, snake *s, screen *scr);

apple *apple_init(snake *s, screen *scr);

uint8_t check_apple_collision(apple *a, snake *s);

#endif /* APPLE_H_SENTRY */
