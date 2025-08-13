#ifndef CALLBACKs_H
#define CALLBACKs_H

typedef void(*CallbackFree)(void *);
typedef void(*CallbackPrint)(void *);
typedef bool(*CallbackCompare)(void *a, void *b);
typedef unsigned int (*CallbackHash)(const char*, int);

#endif