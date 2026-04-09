#include "../include/apple.h"

#include "../include/error.h"

#include <stdlib.h>

void apple_generate(apple *a, snake *s, screen *scr)
{
    a->apple_p.x = 1 + rand() % (scr->col - 1);
    a->apple_p.y = 1 + rand() % (scr->row - 1);
    while (s->collision_mask[a->apple_p.y * scr->col + a->apple_p.x] == 1) {
        a->apple_p.x = 1 + rand() % (scr->col - 1);
        a->apple_p.y = 1 + rand() % (scr->row - 1);
    }
    print_cell(&a->apple_p, '*');
}

apple *apple_init(snake *s, screen *scr)
{
    apple *a = malloc(sizeof(*a));
    if (a == NULL) {
        core_error("Bad alloc!");
    }

    apple_generate(a, s, scr);
    return a;
}

uint8_t check_apple_collision(apple *a, snake *s)
{
    const point *head = &s->circle_buf[s->head];
    return (a->apple_p.x == head->x) && (a->apple_p.y == head->y);
}
