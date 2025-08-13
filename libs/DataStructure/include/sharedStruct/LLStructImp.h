#ifndef LL_STRUCT_IMP_H
#define LL_STRUCT_IMP_H

#include <stdbool.h>
#include "Callbacks/CallbackObj_LL.h"


typedef struct LLNode
{
    void *data;
    LLNode *next;
} LLNode;
typedef struct LinkedList
{
    LLNode *head;
    int length;
	int data_size;
    CallbackObj_LL callObj;
} LinkedList;


#endif