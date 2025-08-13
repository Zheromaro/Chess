#ifndef AVL_TREE_H
#define AVL_TREE_H


typedef struct AVLTreeNode AVLTreeNode;

// TODO: make AVLTreeAdd & AVLTreeRemove return's bool

AVLTreeNode *AVLTreeAdd(AVLTreeNode *root, int value);
AVLTreeNode *AVLTreeRemove(AVLTreeNode *root, int value);
bool AVLTreeHas(AVLTreeNode *root, int value);
int  AVLTreeDepth(AVLTreeNode *root);
void AVLTreePrint(AVLTreeNode *root);

#endif