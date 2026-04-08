#include <curses.h>

#include "./include/ui.h"
#include "./include/snake.h"
#include "./include/constants.h"

int main()
{
    screen scr;
    screen old_scr;
    init_ncurses(&scr);

    point p = {
        .x = scr.col/2,
        .y = scr.row/2
    };
    snake *s = snake_init(&scr, &p);
    set_direction(s, 1, 0);

    int flag_grow_up = 0;
    int key;
    while ((key = getch()) != key_escape) {
        switch (key) {
        case KEY_UP:
            set_direction(s, 0, -1);
            break;
        case KEY_DOWN:
            set_direction(s, 0, 1);
            break;
        case KEY_LEFT:
            set_direction(s, -1, 0);
            break;
        case KEY_RIGHT:
            set_direction(s, 1, 0);
            break;
        case ' ':
            flag_grow_up = 1;
            break;
        case ERR:
            snake_move(s, &scr, flag_grow_up);
            flag_grow_up = 0;
            break;
        case KEY_RESIZE:
            old_scr = scr;
            handle_resize(scr);
            snake_resize(s, &old_scr, &scr);
            break;
        }
    }

    endwin();
    return 0;
}
