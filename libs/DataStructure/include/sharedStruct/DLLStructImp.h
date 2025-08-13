#ifndef LL_STRUCT_IMP_H
#define LL_STRUCT_IMP_H

#include <stdbool.h>
#include "Callbacks/CallbackObj_LL.h"

typedef struct DLLNode
{
    void *data;
    DLLNode *next;
    DLLNode *previous;
} DLLNode;
typedef struct DLinkedList
{
    DLLNode *head;
    DLLNode *tail;
    int length;
	int data_size;
	CallbackObj_LL callObj;
} DLinkedList;


#endif