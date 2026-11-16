#ifndef BSP_RANDOM_H
#define BSP_RANDOM_H

#include <stdint.h>

/* Local state makes independent generations reproducible and reentrant. */
typedef struct {
    uint32_t state;
} BspRandom;

int bsp_random_between(BspRandom *random, int minimum, int maximum);

#endif
