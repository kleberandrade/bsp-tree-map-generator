#ifndef __RANDOM_H__
#define __RANDOM_H__

#include <stdio.h>
#include <time.h>
#include <math.h>

void InitializeSeedRandom(){
    srand((unsigned)time(NULL));
}

int Random(int minLimit, int maxLimit){
    int delta = abs(maxLimit - minLimit) + 1;
    return minLimit + rand() % delta;
}

#endif // __RANDOM_H__
