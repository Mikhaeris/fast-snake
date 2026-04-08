#include "../include/snake.h"
#include "../include/ui.h"
#include "../include/error.h"

#include <curses.h>

#include <stdlib.h>
#include <string.h>

enum { def_buf_size = 10 };

snake *snake_init(screen *scr, point *p)
{
    snake *s = malloc(sizeof(*s));
    if (s == NULL) {
        core_error("Bad alloc!");
    }

    s->head = 0;
    s->tail = 0;
    s->buf_size = def_buf_size;
    s->buf = malloc(sizeof(*s->buf) * s->buf_size);
    if (s->buf == NULL) {
        core_error("Bad alloc!");
    }

    s->row = scr->row;
    s->col = scr->col;
    s->collision_mask = calloc(s->row * s->col, sizeof(*s->collision_mask));
    if (s->collision_mask == NULL) {
        core_error("Bad alloc!");
    }

    s->buf[s->head] = *p;

    return s;
}

void snake_resize(snake *s, screen *old_scr, screen *new_scr)
{
    size_t new_size = new_scr->row * new_scr->col;

    uint8_t *raw = calloc(new_size, sizeof(*s->collision_mask));
    char (*new_matrix)[new_scr->col] = (char (*)[new_scr->col])raw;

    char (*old_matrix)[s->col] = (char (*)[s->col])s->buf;
    for (size_t i = 0; i < old_scr->row; i++) {
        for (size_t j = 0; j < old_scr->col; j++) {
            new_matrix[i][j] = old_matrix[i][j];
        }
    }
}

static void check(int *coord, int max)
{
    if (*coord < 0) {
        *coord += max;
    } else if (*coord > max) {
        *coord -= max;
    }
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
    }
}

void snake_move(snake *s, screen *scr, int flag)
{
    /* mode tail */
    if (flag == 0) {
        clear_cell(&s->buf[s->tail]);
        s->tail = (s->tail + 1) % s->buf_size;
    } else {
        s->length += 1;
        if (s->length >= s->buf_size) {
            snake_buf_realloc(s);
        }
    }

    /* set head cell from @  to # */
    print_cell(&s->buf[s->head], SNAKE_SYMBOL);

    /* move head */
    point new_p = s->buf[s->head];
    new_p.x += s->dx;
    check(&new_p.x, scr->col-1);
    new_p.y += s->dy;
    check(&new_p.y, scr->row-1);

    s->head = (s->head + 1) % s->buf_size;
    s->buf[s->head] = new_p;

    print_cell(&s->buf[s->head], SNAKE_HEAD_SYMBOL);

    refresh();
}

void set_direction(snake *s, int dx, int dy)
{
    s->dx = dx;
    s->dy = dy;
}
