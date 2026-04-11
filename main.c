#include "./include/constants.h"
#include "./include/ui.h"
#include "./include/snake.h"
#include "./include/apple.h"

#include <curses.h>

#include <stdlib.h>
#include <time.h>

int main()
{
    srand(time(NULL));

    screen scr;
    ncurses_init(&scr);

    snake *s = snake_init(&scr);
    set_direction(s, 0, 0);

    apple *a = apple_init(&scr);

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
        case ERR:
            snake_move(s, a, &scr);
            if (check_apple_collision(a, s)) {
                snake_grow_up(s);
                apple_generate(a);
            }
            break;
        case KEY_RESIZE:
            print_msg_exit(&scr, "Oops!");
            break;
        }

    }

    ncurses_free();
    return 0;
}
