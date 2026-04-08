#include <curses.h>

#include "./include/ui.h"
#include "./include/snake.h"

enum { delay_duration = 100 };
enum { key_escape = 27 };

static void init_ncurses(screen *scr)
{
    initscr();
    cbreak();
    timeout(delay_duration);
    keypad(stdscr, 1);
    noecho();
    curs_set(0);

    getmaxyx(stdscr, scr->row, scr->col);
}

int main()
{
    screen scr;

    init_ncurses(&scr);

    direction dr;
    set_direction(&dr, 0, 0);

    point p = {
        .x = scr.col/2,
        .y = scr.row/2
    };
    snake *s = snake_init(&scr, &p);

    int key;
    while ((key = getch()) != key_escape) {
        switch (key) {
        case KEY_UP:
            set_direction(&dr, 0, -1);
            break;
        case KEY_DOWN:
            set_direction(&dr, 0, 1);
            break;
        case KEY_LEFT:
            set_direction(&dr, -1, 0);
            break;
        case KEY_RIGHT:
            set_direction(&dr, 1, 0);
            break;
        case ERR:
            snake_move(s, &dr);
            break;
        case KEY_RESIZE:
            //handle_resize(&snake, &col, &row);
            break;
        }
    }

    endwin();
    return 0;
}
