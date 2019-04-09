#include <stdio.h>
#include <stdlib.h>

#include "random.h"
#include "polygon.h"
#include "bsptree.h"
#include "map.h"
#include "bsptree2map.h"

#define RECT_X              1
#define RECT_Y              1
#define RECT_W              256
#define RECT_H              256
#define TREE_DEPTH          5

#define MAP_PADDING         2
#define MAP_DELIMITER       ';'
#define MAP_NAME            "map.txt"
#define MAP_WALL            '0'
#define MAP_FLOOR           '1'
#define MAP_ROOM_SPACE      5

int main() {
    InitializeSeedRandom();

    Map *map = BuildMap(RECT_W + MAP_PADDING, RECT_H + MAP_PADDING, MAP_WALL);

    BinaryTree tree;
    tree.root = BuildTreeNode(BuildRectangle(RECT_X, RECT_Y, RECT_W, RECT_H));

    BuildMapWithBspTree(map, &tree, TREE_DEPTH, MAP_ROOM_SPACE, MAP_FLOOR);

    PrintMap(map);

    SaveMap(map, MAP_NAME, MAP_DELIMITER);

    return 0;
}
