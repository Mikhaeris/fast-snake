#include "../include/error.h"

#include <stdio.h>
#include <stdlib.h>

void core_error(const char *msg)
{
    fprintf(stderr, "%s\n", msg);
    exit(1);
}
