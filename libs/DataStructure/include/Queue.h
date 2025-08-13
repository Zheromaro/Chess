#ifndef QUEUE_H
#define QUEUE_H

#include <limits.h>
#include "Callbacks/CallbackObj_LL.h"
#include "sharedStruct/DLLStruct.h"

typedef DLLNode QueueElement;
typedef DLinkedList Queue;

// TODO: FreeQueue, Better PrintQueue

Queue *NewQueue(int dataSize, CallbackObj_LL callObj);
void Enqueue(Queue *q, void *value);
void *Dequeue(Queue *q);
void PrintQueue(Queue *q);

#endif