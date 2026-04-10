#include "../include/apple.h"

#include "../include/error.h"

#include <stdlib.h>

void apple_generate(apple *a)
{
    const point *p = &a->cells[1 + rand() % (a->k-1)];
    a->apple_p = *p;
    print_cell(&a->apple_p, '*');
}

void apple_update_cells(apple *a, const point *p, const screen *scr, uint8_t flag)
{
    /* get 1D pos from 2D */
    int idx = p->x * scr->row + p->y;

    /* swap el in cells */
    point tmp_p = a->cells[a->pos[idx]];
    a->cells[a->pos[idx]] = a->cells[a->k];
    a->cells[a->k] = tmp_p;

    if (flag) {
        a->k -= 1;
    } else {
        a->k += 1;
    }

    /* update pos */
    size_t tmp_idx = a->pos[idx];
    a->pos[idx] = a->pos[a->k];
    a->pos[a->k] = tmp_idx;
}

apple *apple_init(screen *scr)
{
    apple *a = malloc(sizeof(*a));
    if (a == NULL) {
        core_error("Bad alloc!");
    }

    size_t screen_size = scr->row * scr->col;
    a->cells = malloc(sizeof(*a->cells) * screen_size);
    if (a->cells == NULL) {
        core_error("Bad alloc!");
    }
    a->k = screen_size;

    for (int i = 0; i < scr->row; i++) {
        for (int j = 0; j < scr->col; j++) {
            point *p = &a->cells[i * scr->row + j];
            p->x = i;
            p->y = j;
        }
    }

    a->pos = malloc(sizeof(*a->pos) * screen_size);
    if (a->pos == NULL) {
        core_error("Bad alloc!");
    }
    for (int i = 0; i < scr->row; i++) {
        for (int j = 0; j < scr->col; j++) {
            a->pos[i * scr->row + j] = i * scr->row + j;
        }
    }

    point p = {
        .x = scr->col/2,
        .y = scr->row/2
    };
    apple_update_cells(a, &p, scr, 1);

    apple_generate(a);
    return a;
}
