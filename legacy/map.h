#ifndef __MAP_H__
#define __MAP_H__

#include <stdio.h>
#include <stdlib.h>

typedef struct _Map {
    int width;
    int height;
    char **table;
} Map;

void ClearMap(Map* map, const char simbol){
    int row, colunm;
    for (row = 0; row < map->height; ++row){
        for (colunm = 0; colunm < map->width; ++colunm){
            map->table[row][colunm] = simbol;
        }
    }
}

Map* BuildMap(int width, int height, const char simbol){
    int row;
    Map* map = (Map*)malloc(sizeof(Map));
    map->width = width;
    map->height = height;

    map->table = (char**)malloc(sizeof(char*) * height);
    for (row = 0; row < height; ++row){
        map->table[row] = (char*)malloc(sizeof(char) * width);
    }

    ClearMap(map, simbol);

    return map;
}

void PrintMap(Map *map){
    int row, colunm;
    for (row = 0; row < map->height; ++row){
        for (colunm = 0; colunm < map->width; ++colunm){
            printf("%c", map->table[row][colunm]);
        }
        printf("\n");
    }
    printf("\n");
}

void SaveMap(Map* map, char* fileName, const char delimiter){
    FILE *file = fopen(fileName, "w");
    int row, colunm;
    for (row = 0; row < map->height; ++row){
        for (colunm = 0; colunm < map->width; ++colunm){
            fprintf(file, "%c%c", map->table[row][colunm], delimiter);
        }
        fprintf(file, "\n");
    }
    fprintf(file, "\n");
    fclose(file);
}

#endif // __MAP_H__
