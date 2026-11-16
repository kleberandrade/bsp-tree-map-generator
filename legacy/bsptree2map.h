#ifndef __BSPTREE2MAP_H__
#define __BSPTREE2MAP_H__

#include "binarytree.h"
#include "map.h"
#include "polygon.h"

void BuildRoomsByTree(TreeNode *node, int minSpace){
    if (node == NULL)
        return;

    if (node->left == NULL && node->right == NULL){
        int w = Random(minSpace, node->data.w);
        int h = Random(minSpace, node->data.h);
        int dw = node->data.w - w;
        int dh = node->data.h - h;

        node->data.x = Random(node->data.x, node->data.x + dw);
        node->data.y = Random(node->data.y, node->data.y + dh);
        node->data.w = w;
        node->data.h = h;
    } else {
        BuildRoomsByTree(node->left, minSpace);
        BuildRoomsByTree(node->right, minSpace);
    }
}

void FillMapWithRectangle(Map* map, const Rectangle rect, const char simbol){
    int row, colunm;
    for (row = rect.y; row < rect.y + rect.h; ++row){
        for (colunm = rect.x; colunm < rect.x + rect.w; ++colunm){
            map->table[row][colunm] = simbol;
        }
    }
}

void FillMapWithTreeNode(Map* map, TreeNode *node, const char simbol){
    if (node == NULL)
        return;

    if (node->left == NULL && node->right == NULL){
        FillMapWithRectangle(map, node->data, simbol);
    } else {
        FillMapWithTreeNode(map, node->left, simbol);
        FillMapWithTreeNode(map, node->right, simbol);
    }
}

void FillHallBeetwenRectangles(Map *map, Rectangle rect1, Rectangle rect2, const char simbol){

    Point2 p1 = BuildPoint2(Random(rect1.x, rect1.x + rect1.w), Random(rect1.y, rect1.y + rect1.h));
    Point2 p2 = BuildPoint2(Random(rect2.x, rect2.x + rect2.w), Random(rect2.y, rect2.y + rect2.h));

    Point2 dir = BuildPoint2(p2.x - p1.x, p2.y - p1.y);

    Rectangle xHall, yHall;

    if (dir.x > 0)
        xHall = BuildRectangle(p1.x, p1.y, abs(dir.x) + 1, 1);
    else
        xHall = BuildRectangle(p1.x - abs(dir.x), p1.y, abs(dir.x), 1);

    if (dir.y > 0)
        yHall = BuildRectangle(p2.x, p2.y - abs(dir.y), 1, abs(dir.y));
    else
        yHall = BuildRectangle(p2.x, p2.y, 1, abs(dir.y) + 1);

    FillMapWithRectangle(map, xHall, simbol);
    FillMapWithRectangle(map, yHall, simbol);
}

Rectangle GetRoom(TreeNode* node){
    if (node->left != NULL && node->right != NULL){
        if (Random(0,1))
            return GetRoom(node->left);
        else
            return GetRoom(node->right);
    }
    else
        return node->data;
}

void FillHallInMapByTreeNodes(Map* map, TreeNode *node, const char simbol){
    if (node->left != NULL && node->right != NULL){
        FillHallBeetwenRectangles(map, GetRoom(node->left), GetRoom(node->right), simbol);
        FillHallInMapByTreeNodes(map, node->left, simbol);
        FillHallInMapByTreeNodes(map, node->right, simbol);
    }
}

void BuildMapWithBspTree(Map* map, BinaryTree *tree, const int treeDepth, const int space, const char simbol){
    BuildBspTreeByDepth(tree->root, treeDepth);
    BuildRoomsByTree(tree->root, space);
    FillMapWithTreeNode(map, tree->root, simbol);
    FillHallInMapByTreeNodes(map, tree->root, simbol);
}

#endif // __BSPTREE2MAP_H__
