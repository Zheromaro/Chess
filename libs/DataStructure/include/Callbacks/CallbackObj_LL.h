#ifndef CALLBACK_OBJ_LL_H
#define CALLBACK_OBJ_LL_H

#include <stdbool.h>
#include "CallBacks.h"

/*  Has those Callbacks 

    - CallbackFree : 
        not necessary for simple type
        add "free_callback" if you want to free variables in complex struct
    - CallbackPrint : 
        "##Print" Will print the address of node->data in console
        add "print_callback" if you want to print other values
    - CallbackCompare : 
        "##Has", "##FindNode" and "##Remove" will look for node with the same data pointer 
        add "compare_callback" if you want to compare different things
*/
typedef struct CallbackObj_LL
{
	CallbackFree callbackFree;
	CallbackPrint callbackPrint;
	CallbackCompare callbackCompare;
} CallbackObj_LL;

#endif 