#include "../include/snake.h"
#include "../include/ui.h"
#include "../include/error.h"

#include <curses.h>

#include <stdlib.h>
#include <string.h>

snake *snake_init(screen *scr)
{
    snake *s = malloc(sizeof(*s));
    if (s == NULL) {
        core_error("Bad alloc!");
    }

    s->grow_up = 0;
    s->head = 0;
    s->tail = 0;
    s->circle_buf_size = scr->row * scr->col;
    s->circle_buf = malloc(sizeof(*s->circle_buf) * s->circle_buf_size);
    if (s->circle_buf == NULL) {
        core_error("Bad alloc!");
    }

    s->collision_mask = calloc(scr->row * scr->col, sizeof(*s->collision_mask));
    if (s->collision_mask == NULL) {
        core_error("Bad alloc!");
    }

    point *p = &s->circle_buf[s->head];
    p->x = scr->col/2;
    p->y = scr->row/2;

    return s;
}

inline void snake_grow_up(snake *s)
{
    s->grow_up = 1;
}

static inline uint8_t snake_check_collision(snake *s, const screen *scr)
{
    size_t x = s->circle_buf[s->head].x;
    size_t y = s->circle_buf[s->head].y;
    return s->collision_mask[y * scr->col + x] == 1;
}

static inline void snake_set_collision(snake *s, const screen *scr, size_t pos, uint8_t val)
{
    size_t x = s->circle_buf[pos].x;
    size_t y = s->circle_buf[pos].y;
    s->collision_mask[y * scr->col + x] = val;
}

static inline void check(int *coord, int max)
{
    if (*coord < 0) {
        *coord += max;
    } else if (*coord > max) {
        *coord -= max;
    }
}

void snake_move(snake *s, const screen *scr)
{
    /* set head cell from @ to # */
    print_cell(&s->circle_buf[s->head], SNAKE_SYMBOL);

    /* move tail */
    if (s->grow_up == 0) {
        clear_cell(&s->circle_buf[s->tail]);
        snake_set_collision(s, scr, s->tail, 0);
        s->tail = (s->tail + 1) % s->circle_buf_size;
    } else {
        s->grow_up = 0;
    }

    /* move head */
    point new_p = s->circle_buf[s->head];
    new_p.x += s->dx;
    check(&new_p.x, scr->col-1);
    new_p.y += s->dy;
    check(&new_p.y, scr->row-1);

    s->head = (s->head + 1) % s->circle_buf_size;
    s->circle_buf[s->head] = new_p;

    if (snake_check_collision(s, scr)) {
        print_msg_exit(scr, "Game over!");
    }

    snake_set_collision(s, scr, s->head, 1);

    print_cell(&s->circle_buf[s->head], SNAKE_HEAD_SYMBOL);

    refresh();
}

inline void set_direction(snake *s, int dx, int dy)
{
    s->dx = dx;
    s->dy = dy;
}
