#ifndef DOUBLE_LINKED_LIST_H
#define DOUBLE_LINKED_LIST_H

#include <stdbool.h>
#include "Callbacks/CallbackObj_LL.h"
#include "sharedStruct/DLLStruct.h"


DLinkedList *NewDLL(int dataSize, CallbackObj_LL callObj);
void FreeDLL(DLinkedList **list);
DLLNode *NewDLLNode(DLinkedList *list, void *data); 
void DLLAdd(DLinkedList *list, void *data);
void DLLAddNode(DLinkedList *list, DLLNode *newNode);
void DLLAddBefore (DLinkedList *list, DLLNode *node, DLLNode *newNode);
void DLLAddAfter(DLinkedList *list, DLLNode *node, DLLNode *newNode);
void DLLRemove(DLinkedList *list, void *data);
void DLLRemoveNode(DLinkedList *list, DLLNode *node);
void DLLRemoveAll(DLinkedList *list);
void DLLPrint(DLinkedList *list);
bool DLLHas(DLinkedList *list, void *data);
bool DLLHasNode(DLinkedList *list, DLLNode *node);
DLLNode *DLLFindNode(DLinkedList *list, void *data);
int DLLGetSize(DLinkedList *list);

#endif 