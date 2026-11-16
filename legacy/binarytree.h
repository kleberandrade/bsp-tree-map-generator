#ifndef __BINARYTREE_H__
#define __BINARYTREE_H__

#include <stdio.h>
#include "polygon.h"

typedef struct _TreeNode {
    Rectangle data;
    struct _TreeNode* left;
    struct _TreeNode* right;
} TreeNode;

typedef struct _BinaryTree {
    TreeNode* root;
} BinaryTree;

TreeNode* BuildTreeNode(Rectangle data){
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void PrintInOrderBinaryTree(TreeNode* node){
    if (node != NULL){
        PrintInOrderBinaryTree(node->left);
        PrintRectangle(node->data);
        PrintInOrderBinaryTree(node->right);
    }
}

#endif // __BINARYTREE_H__
