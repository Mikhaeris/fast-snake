#ifndef APPLE_H_SENTRY
#define APPLE_H_SENTRY

#include "./game_status.h"
#include "./point.h"
#include "./ui.h"

#include <stddef.h>
#include <stdint.h>

#define APPLE_SYMBOL '*'

typedef struct apple {
    point apple_p;
    point *cells;
    size_t k;
    size_t *pos;
} apple;

game_status apple_generate(apple *a);

void apple_update_cells(apple *a, const point *p, const screen *scr, uint8_t flag);

apple *apple_init(screen *scr);

void apple_free();

#endif /* APPLE_H_SENTRY */
