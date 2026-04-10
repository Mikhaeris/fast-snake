#ifndef APPLE_H_SENTRY
#define APPLE_H_SENTRY

#include "./point.h"
#include "./ui.h"

#include <stddef.h>
#include <stdint.h>

typedef struct apple {
    point apple_p;
    point *cells;
    size_t k;
    size_t *pos;
} apple;

void apple_generate(apple *a);

void apple_update_cells(apple *a, const point *p, const screen *scr, uint8_t flag);

apple *apple_init(screen *scr);

#endif /* APPLE_H_SENTRY */
