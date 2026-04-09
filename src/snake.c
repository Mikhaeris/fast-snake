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
    s->buf_size = scr->row * scr->col;
    s->buf = malloc(sizeof(*s->buf) * s->buf_size);
    if (s->buf == NULL) {
        core_error("Bad alloc!");
    }

    s->collision_mask = calloc(scr->row * scr->col, sizeof(*s->collision_mask));
    if (s->collision_mask == NULL) {
        core_error("Bad alloc!");
    }

    point *p = &s->buf[s->head];
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

static void snake_buf_realloc(snake *s)
{
    size_t old_size = s->buf_size;
    s->buf_size *= 2;
    if (s->head < s->tail) {
        point *raw = malloc(sizeof(*raw) * s->buf_size);
        if (raw == NULL) {
            core_error("Bad alloc!");
        }

        size_t last_size = (old_size - s->tail);
        memcpy(raw, &s->buf[s->tail], sizeof(*s->buf) * last_size);

        size_t old_head = s->head;
        s->tail = 0;
        s->head = last_size + old_head;

        memcpy(raw + last_size, s->buf, sizeof(*s->buf) * (old_head + 1));
        free(s->buf);
        s->buf = raw;
    } else /* (s->head > s->tail) */ {
        s->buf = realloc(s->buf, sizeof(*s->buf) * s->buf_size);
        if (s->buf == NULL) {
            core_error("Bad alloc!");
        }
    }
}

void snake_update(snake *s, screen *old_scr, screen *new_scr)
{
    snake_collision_realloc(s, old_scr, new_scr);
    snake_buf_realloc(s);
}

void snake_grow_up(snake *s)
{
    s->grow_up = 1;
}

static uint8_t snake_check_collision(snake *s, const screen *scr)
{
    char (*collision_matrix)[scr->col] = (char (*)[scr->col])s->collision_mask;
    return collision_matrix[s->buf[s->head].x][s->buf[s->head].y] == 1;
}

static void snake_set_collision(snake *s, const screen *scr, size_t pos, uint8_t val)
{
    char (*collision_matrix)[scr->col] = (char (*)[scr->col])s->collision_mask;
    collision_matrix[s->buf[pos].x][s->buf[pos].y] = val;
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
    print_cell(&s->buf[s->head], SNAKE_SYMBOL);

    /* move tail */
    if (s->grow_up == 0) {
        clear_cell(&s->buf[s->tail]);
        snake_set_collision(s, scr, s->tail, 0);
        s->tail = (s->tail + 1) % s->buf_size;
    } else {
        s->grow_up = 0;
    }

    /* move head */
    point new_p = s->buf[s->head];
    new_p.x += s->dx;
    check(&new_p.x, scr->col-1);
    new_p.y += s->dy;
    check(&new_p.y, scr->row-1);

    s->head = (s->head + 1) % s->buf_size;
    s->buf[s->head] = new_p;

    if (snake_check_collision(s, scr)) {
        game_over(scr);
    }

    snake_set_collision(s, scr, s->head, 1);

    print_cell(&s->buf[s->head], SNAKE_HEAD_SYMBOL);

    refresh();
}

void set_direction(snake *s, int dx, int dy)
{
    s->dx = dx;
    s->dy = dy;
}
