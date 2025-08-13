#ifndef BINARY_TREE_H
#define BINARY_TREE_H


typedef struct BTreeNode BTreeNode;

bool BSTreeAdd(BTreeNode **rootptr, int value);
bool BSTreeRemove(BTreeNode **rootptr, int value);
bool BSTreeHas(BTreeNode *root, int value);
int  BSTreeDepth(BTreeNode *root);
void BSTreePrint(BTreeNode *root);

#endif