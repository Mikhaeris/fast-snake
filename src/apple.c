#include "../include/apple.h"

#include "../include/error.h"

#include <stdlib.h>

void apple_generate(apple *a, snake *s)
{
    point *p = &a->cells[1 + rand() % (a->k)];
    a->apple_p = *p;
    print_cell(&a->apple_p, '*');
}

void apple_update_cells(apple *a, point *p, screen *scr, uint8_t flag)
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

apple *apple_init(snake *s, screen *scr)
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

    for (size_t i = 0; i < scr->row; i++) {
        for (size_t j = 0; j < scr->col; j++) {
            point *p = &a->cells[i * scr->row + j];
            p->x = i;
            p->y = j;
        }
    }

    a->pos = malloc(sizeof(*a->pos) * screen_size);
    if (a->pos == NULL) {
        core_error("Badd alloc!");
    }
    for (size_t i = 0; i < scr->row; i++) {
        for (size_t j = 0; j < scr->col; j++) {
            a->pos[i * scr->row + j] = i * scr->row + j;
        }
    }

    apple_generate(a, s);
    return a;
}

uint8_t check_apple_collision(apple *a, snake *s)
{
    const point *head = &s->circle_buf[s->head];
    return (a->apple_p.x == head->x) && (a->apple_p.y == head->y);
}
