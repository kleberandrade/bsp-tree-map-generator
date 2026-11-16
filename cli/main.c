#include "bsp.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

static int parse_number(const char *text, unsigned long limit, unsigned long *value)
{
    char *end;
    if (!text[0] || text[0] == '-') return 0;
    errno = 0;
    *value = strtoul(text, &end, 10);
    return !errno && !*end && *value <= limit;
}

static int print_map(const char *map, size_t width, size_t height)
{
    for (size_t row = 0; row < height; ++row) {
        if (fwrite(map + row * width, 1, width, stdout) != width ||
            putchar('\n') == EOF) return 0;
    }
    return fflush(stdout) != EOF;
}

int main(int argc, char **argv)
{
    unsigned long arguments[] = {64, 32, 5, 3, 1};
    const unsigned long limits[] = {
        BSP_MAX_DIMENSION, BSP_MAX_DIMENSION, BSP_MAX_DEPTH,
        BSP_MAX_DIMENSION - 2, UINT32_MAX
    };
    if (argc != 1 && argc != 6) {
        fprintf(stderr, "Usage: %s [width height depth min-room seed]\n", argv[0]);
        return 2;
    }
    for (int index = 1; index < argc; ++index) {
        if (!parse_number(argv[index], limits[index - 1], &arguments[index - 1])) {
            fprintf(stderr, "Invalid numeric argument: %s\n", argv[index]);
            return 2;
        }
    }

    char *map = bsp_generate((int)arguments[0], (int)arguments[1],
                             (unsigned)arguments[2], (int)arguments[3],
                             (uint32_t)arguments[4]);
    if (!map) {
        fputs("Invalid dimensions, room size, depth, or allocation failure.\n", stderr);
        return 1;
    }
    int success = print_map(map, arguments[0], arguments[1]);
    free(map);
    return success ? 0 : 1;
}
