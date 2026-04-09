#include "./include/constants.h"
#include "./include/ui.h"
#include "./include/snake.h"

#include <curses.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    srand(time(NULL));

    screen scr;
    screen old_scr;
    ncurses_init(&scr);

    snake *s = snake_init(&scr);
    set_direction(s, 0, 0);

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
            snake_grow_up(s);
            break;
        case ERR:
            snake_move(s, &scr);
            break;
        case KEY_RESIZE:
            old_scr = scr;
            handle_resize(&scr);
            snake_update(s, &old_scr, &scr);
            break;
        }
    }

    ncurses_free();
    return 0;
}
