#include "random.h"

int bsp_random_between(BspRandom *random, int minimum, int maximum)
{
    random->state = random->state * UINT32_C(1664525) + UINT32_C(1013904223);
    return minimum + (int)(random->state % (uint32_t)(maximum - minimum + 1));
}
