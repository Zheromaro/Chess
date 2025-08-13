#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdbool.h>
#include "Callbacks/CallbackObj_LL.h"
#include "sharedStruct/LLStruct.h"


LinkedList *NewLL(int dataSize, CallbackObj_LL callObj);
void FreeLL(LinkedList **list);
LLNode *NewLLNode(LinkedList *list, void *data);
void LLAdd(LinkedList *list, void *data);
void LLAddNode(LinkedList *list, LLNode *newNode);
void LLAddBefore (LinkedList *list, LLNode *node, LLNode *newNode);
void LLAddAfter (LinkedList *list, LLNode *node, LLNode *newNode);
void LLRemove(LinkedList *list, void *data);
void LLRemoveNode(LinkedList *list, LLNode *node);
void LLRemoveAll(LinkedList *list);
void LLPrint(LinkedList *list);
bool LLHas(LinkedList *list, void *data);
bool LLHasNode(LinkedList *list, LLNode *node);
LLNode *LLFindNode(LinkedList *list, void *data);
int LLGetSize(LinkedList *list);

#endif 