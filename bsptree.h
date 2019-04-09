#ifndef __BSPTREE_H__
#define __BSPTREE_H__

#include "binarytree.h"
#include "polygon.h"
#include "random.h"

#define HORIZONTAL        0
#define VERTICAL          1
#define HALF_SPACE        2
#define MIN_SPACE_DIV     2
#define MAX_SPACE_DIV     4

void SplitRectangleRandom(const Rectangle parent, Rectangle* leftChild, Rectangle* rightChild, int vertical, int minDiv, int maxDiv){
    if (vertical){
        *leftChild  = BuildRectangle(parent.x, parent.y, parent.w / Random(minDiv, maxDiv), parent.h);
        *rightChild = BuildRectangle(parent.x + leftChild->w, parent.y, parent.w - leftChild->w, parent.h);
    } else {
        *leftChild  = BuildRectangle(parent.x, parent.y, parent.w, parent.h / Random(minDiv, maxDiv));
        *rightChild = BuildRectangle(parent.x, parent.y + leftChild->h, parent.w, parent.h - leftChild->h);
    }
}

void BuildBspTreeByDepth(TreeNode* node, int depth){

    if (!depth) return;

    Rectangle leftRect, rightRect;

    SplitRectangleRandom(node->data,&leftRect,&rightRect,Random(HORIZONTAL, VERTICAL),MIN_SPACE_DIV,MAX_SPACE_DIV);

    node->left = BuildTreeNode(leftRect);
    node->right = BuildTreeNode(rightRect);

    BuildBspTreeByDepth(node->left,  depth - 1);
    BuildBspTreeByDepth(node->right, depth - 1);
}

#endif // __BSPTREE_H__
