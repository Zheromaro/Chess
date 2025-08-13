#ifndef STACK_H
#define STACK_H

#include <limits.h>
#include "Callbacks/CallbackObj_LL.h"
#include "sharedStruct/LLStruct.h"

#define STACK_EMPTY INT_MIN

typedef LLNode StackElement;
typedef LinkedList Stack;

// TODO: FreeStack, Better PrintStack

Stack *NewStack(int dataSize, CallbackObj_LL callObj);
void Puch(Stack *stack, void *value);
void *Pop(Stack *stack);
void PrintStack(Stack *stack);

#endif 