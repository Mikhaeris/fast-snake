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

static void snake_collision_realloc(snake *s, const screen *old_scr, const screen *new_scr)
{
    size_t new_size = new_scr->row * new_scr->col;

    uint8_t *raw = calloc(new_size, sizeof(*s->collision_mask));
    char (*new_matrix)[new_scr->col] = (char (*)[new_scr->col])raw;

    char (*old_matrix)[old_scr->col] = (char (*)[old_scr->col])s->collision_mask;
    for (size_t i = 0; i < old_scr->row; i++) {
        for (size_t j = 0; j < old_scr->col; j++) {
            new_matrix[i][j] = old_matrix[i][j];
        }
    }
    free(s->collision_mask);
    s->collision_mask = raw;
}

static void snake_circle_buf_realloc(snake *s)
{
    size_t old_size = s->circle_buf_size;
    s->circle_buf_size *= 2;
    if (s->head < s->tail) {
        point *raw = malloc(sizeof(*raw) * s->circle_buf_size);
        if (raw == NULL) {
            core_error("Bad alloc!");
        }

        size_t last_size = (old_size - s->tail);
        memcpy(raw, &s->circle_buf[s->tail], sizeof(*s->circle_buf) * last_size);

        size_t old_head = s->head;
        s->tail = 0;
        s->head = last_size + old_head;

        memcpy(raw + last_size, s->circle_buf, sizeof(*s->circle_buf) * (old_head + 1));
        free(s->circle_buf);
        s->circle_buf = raw;
    } else /* (s->head > s->tail) */ {
        s->circle_buf = realloc(s->circle_buf, sizeof(*s->circle_buf) * s->circle_buf_size);
        if (s->circle_buf == NULL) {
            core_error("Bad alloc!");
        }
    }
}

void snake_update(snake *s, screen *old_scr, screen *new_scr)
{
    snake_collision_realloc(s, old_scr, new_scr);
    snake_circle_buf_realloc(s);
}

void snake_grow_up(snake *s)
{
    s->grow_up = 1;
}

static uint8_t snake_check_collision(snake *s, const screen *scr)
{
    size_t x = s->circle_buf[s->head].x;
    size_t y = s->circle_buf[s->head].y;
    return s->collision_mask[y * scr->col + x] == 1;
}

static void snake_set_collision(snake *s, const screen *scr, size_t pos, uint8_t val)
{
    size_t x = s->circle_buf[pos].x;
    size_t y = s->circle_buf[pos].y;
    s->collision_mask[y * scr->col + x] = val;
}

static void check(int *coord, int max)
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
        game_over(scr);
    }

    snake_set_collision(s, scr, s->head, 1);

    print_cell(&s->circle_buf[s->head], SNAKE_HEAD_SYMBOL);

    refresh();
}

void set_direction(snake *s, int dx, int dy)
{
    s->dx = dx;
    s->dy = dy;
}
