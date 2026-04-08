#include <curses.h>

enum { delay_duration = 100 };
enum { key_escape = 27 };

typedef struct snake_test {
    int cur_x;
    int cur_y;
    int dx;
    int dy;
} snake_test;

static void show_snake(const snake_test *snake)
{
    move(snake->cur_y, snake->cur_x);
    addch('*');
    refresh();
}

static void hide_snake(const snake_test *snake)
{
    move(snake->cur_y, snake->cur_x);
    addch(' ');
    refresh();
}

static void check(int *coord, int max)
{
    if (*coord < 0) {
        *coord += max;
    } else if (*coord > max) {
        *coord -= max;
    }
}

static void move_snake(snake_test *snake, int max_x, int max_y)
{
    hide_snake(snake);
    snake->cur_x += snake->dx;
    check(&snake->cur_x, max_x);
    snake->cur_y += snake->dy;
    check(&snake->cur_y, max_y);
    check(&snake->cur_y, max_y);
    show_snake(snake);
}

static void set_direction(snake_test *snake, int dx, int dy)
{
    snake->dx = dx;
    snake->dy = dy;
}

static void handle_resize(snake_test *snake, int *col, int*row)
{
    getmaxyx(stdscr, *row, *col);
}


int main()
{
    initscr();
    cbreak();
    timeout(delay_duration);
    keypad(stdscr, 1);
    noecho();
    curs_set(0);

    int row, col;
    getmaxyx(stdscr, row, col);

    snake_test snake;
    snake.cur_x = col/2;
    snake.cur_y = row/2;
    set_direction(&snake, 0, 0);

    int key;
    while ((key = getch()) != key_escape) {
        switch (key) {
        case KEY_UP:
            set_direction(&snake, 0, -1);
            break;
        case KEY_DOWN:
            set_direction(&snake, 0, 1);
            break;
        case KEY_LEFT:
            set_direction(&snake, -1, 0);
            break;
        case KEY_RIGHT:
            set_direction(&snake, 1, 0);
            break;
        case ERR:
            move_snake(&snake, col-1, row-1);
            break;
        case KEY_RESIZE:
            handle_resize(&snake, &col, &row);
        }
    }

    endwin();
    return 0;
}
