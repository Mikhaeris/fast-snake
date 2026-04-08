#include "../include/snake.h"
#include "../include/ui.h"
#include "../include/error.h"

#include <curses.h>

#include <stdlib.h>

snake *snake_init(screen *scr, point *p)
{
    snake *s = malloc(sizeof(*s));
    if (s == NULL) {
        core_error("Bad alloc!");
    }

    s->head = 0;
    s->tail = 0;
    s->buf = malloc(sizeof(*s->buf) * def_buf_size);
    if (s->buf == NULL) {
        core_error("Bad alloc!");
    }

    s->collision_mask = malloc(sizeof(uint8_t) * scr->row * scr->col);
    if (s->collision_mask == NULL) {
        core_error("Bad alloc!");
    }

    s->buf[s->head] = *p;

    return s;
}

void snake_update(snake *s, int new_row, int new_col)
{
    s->collision_mask = realloc(s->collision_mask,
                                sizeof(uint8_t) * new_row * new_col);
}

void snake_move(snake *s, direction *dr)
{

    /* print new head cell*/

    /* create new point with new coord */
    point new_p = s->buf[s->head];
    new_p.x += dr->x;
    new_p.y += dr->y;

    s->head += 1;
    s->buf[s->head] = new_p;

    print_cell(&s->buf[s->head], SNAKE_SYMBOL);
    point test = {0, 0};
    print_cell(&test, 'a');

    /* clear tail cell */
    clear_cell(&s->buf[s->tail]);
    s->tail += 1;

    refresh();
}

void set_direction(direction *dr, int dx, int dy)
{
    dr->x = dx;
    dr->x = dy;
}

static void check(int *coord, int max)
{
    if (*coord < 0) {
        *coord += max;
    } else if (*coord > max) {
        *coord -= max;
    }
}
