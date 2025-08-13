#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "sharedStruct/LLStruct.h"
#include "Callbacks/CallbackObj_HT.h"

typedef struct HashTabel HashTabel;

HashTabel *NewHT(uint32_t size, CallbackObj_HT callObj);
void FreeHT(HashTabel **htptr);
bool HTInsert(HashTabel *ht, const char *constKey, void *value);
void HTDelete(HashTabel *ht, const char *constKey);
bool HTHas(HashTabel *ht, const char *constKey);
void HTPrint(HashTabel *ht);

#endif 