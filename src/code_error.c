#include "../include/error.h"

#include "../include/ui.h"

#include <stdio.h>
#include <stdlib.h>

void core_error(const char *msg)
{
    fprintf(stderr, "%s\n", msg);
    ncurses_free();
    exit(1);
}
