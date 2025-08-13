#ifndef CALLBACK_OBJ_HT_H
#define CALLBACK_OBJ_HT_H

#include <stdbool.h>
#include "CallBacks.h"

/*  Has those Callbacks 

    - CallbackFree : 
        not necessary for simple type
        add "callbackFree" if you want to free variables in complex struct
    - CallbackPrint : 
        "##Print" Will print the address of node->data in console
        add "callbackPrint" if you want to print other values
    - CallbackHash : 
        not nesesary there is a default Hash function
        add "callbackHash" for custom made hash function
*/
typedef struct CallbackObj_HT
{
    CallbackFree callbackFree;
	CallbackPrint callbackPrint;
	CallbackHash callbackHash;
} CallbackObj_HT;

#endif 