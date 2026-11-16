#include "bsp.h"
#include "random.h"

#include <stdlib.h>
#include <string.h>

typedef struct {
    int x, y, width, height;
} Rectangle;

typedef struct {
    int x, y;
} Point;

typedef struct {
    char *cells;
    int width;
    int minimum_room;
    BspRandom random;
} Generator;

static int choose(Generator *generator, int minimum, int maximum)
{
    return bsp_random_between(&generator->random, minimum, maximum);
}

static void carve_cell(Generator *generator, int x, int y)
{
    generator->cells[(size_t)y * (size_t)generator->width + (size_t)x] = BSP_FLOOR;
}

static Point carve_room(Generator *generator, Rectangle region)
{
    int width = choose(generator, generator->minimum_room, region.width);
    int height = choose(generator, generator->minimum_room, region.height);
    int x = choose(generator, region.x, region.x + region.width - width);
    int y = choose(generator, region.y, region.y + region.height - height);

    for (int row = y; row < y + height; ++row) {
        for (int column = x; column < x + width; ++column) {
            carve_cell(generator, column, row);
        }
    }
    return (Point){x + width / 2, y + height / 2};
}

static void connect_rooms(Generator *generator, Point from, Point to)
{
    while (from.x != to.x) {
        carve_cell(generator, from.x, from.y);
        from.x += from.x < to.x ? 1 : -1;
    }
    while (from.y != to.y) {
        carve_cell(generator, from.x, from.y);
        from.y += from.y < to.y ? 1 : -1;
    }
    carve_cell(generator, to.x, to.y);
}

static void split_region(Generator *generator, Rectangle region, int vertical,
                         Rectangle *first, Rectangle *second)
{
    *first = region;
    *second = region;
    if (vertical) {
        first->width = choose(generator, generator->minimum_room,
                              region.width - generator->minimum_room);
        second->x += first->width;
        second->width -= first->width;
    } else {
        first->height = choose(generator, generator->minimum_room,
                               region.height - generator->minimum_room);
        second->y += first->height;
        second->height -= first->height;
    }
}

/* Each child returns a point in its connected component. Joining these points
 * joins all descendant rooms, without storing an allocated binary tree. */
static Point generate_region(Generator *generator, Rectangle region, unsigned depth)
{
    int vertical = choose(generator, 0, 1);
    int can_split_width = region.width >= 2 * generator->minimum_room;
    int can_split_height = region.height >= 2 * generator->minimum_room;

    if (!can_split_width) vertical = 0;
    if (!can_split_height) vertical = 1;
    if (depth == 0 || (!can_split_width && !can_split_height)) {
        return carve_room(generator, region);
    }

    Rectangle first, second;
    split_region(generator, region, vertical, &first, &second);
    Point first_room = generate_region(generator, first, depth - 1);
    Point second_room = generate_region(generator, second, depth - 1);
    connect_rooms(generator, first_room, second_room);
    return first_room;
}

char *bsp_generate(int width, int height, unsigned depth, int min_room, uint32_t seed)
{
    if (width < 3 || height < 3 || width > BSP_MAX_DIMENSION ||
        height > BSP_MAX_DIMENSION || depth > BSP_MAX_DEPTH || min_room < 1 ||
        min_room > width - 2 || min_room > height - 2) {
        return NULL;
    }

    size_t size = (size_t)width * (size_t)height;
    char *cells = malloc(size);
    if (!cells) return NULL;

    memset(cells, BSP_WALL, size);
    Generator generator = {cells, width, min_room, {seed}};
    generate_region(&generator, (Rectangle){1, 1, width - 2, height - 2}, depth);
    return cells;
}
