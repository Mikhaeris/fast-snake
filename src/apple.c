#include "../include/apple.h"

#include "../include/error.h"

#include <stdlib.h>

void apple_generate(apple *a)
{
    if (a->k == 0) {
        return;
    }
    const point *p = &a->cells[rand() % a->k];
    a->apple_p = *p;
    print_cell(&a->apple_p, APPLE_SYMBOL);
}

void apple_update_cells(apple *a, const point *p, const screen *scr, uint8_t flag)
{
    /* 2D to 1D */
    int idx = p->y * scr->col + p->x;

    if (flag) {
        if (a->k == 0) return;

        size_t cur_pos = a->pos[idx];
        size_t last    = a->k - 1;

        point last_p = a->cells[last];
        int last_idx = last_p.y * scr->col + last_p.x;

        a->cells[cur_pos] = last_p;
        a->cells[last]    = *p;

        a->pos[last_idx] = cur_pos;
        a->pos[idx]      = last;

        a->k--;
    } else {
        a->cells[a->k] = *p;
        a->pos[idx]    = a->k;
        a->k++;
    }
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
            point *p = &a->cells[i * scr->col + j];
            p->x = j;
            p->y = i;
        }
    }

    a->pos = malloc(sizeof(*a->pos) * screen_size);
    if (a->pos == NULL) {
        core_error("Bad alloc!");
    }
    for (int i = 0; i < scr->row; i++) {
        for (int j = 0; j < scr->col; j++) {
            a->pos[i * scr->col + j] = i * scr->col + j;
        }
    }

    point p = {
        .x = scr->col / 2,
        .y = scr->row / 2
    };
    apple_update_cells(a, &p, scr, 1);
    apple_generate(a);

    return a;
}
