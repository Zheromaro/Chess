#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>
#include "sharedStruct/LLStruct.h"
#include "Callbacks/CallbackObj_LL.h"

typedef struct ALGraph ALGraph;

ALGraph *NewALGraph(int numNodes);
void FreeALGraph(ALGraph **pg);
bool ALGraphInsertEdge(ALGraph *g, unsigned int fromNode, unsigned int toNode);
bool ALGraphRemoveEdge(ALGraph *g, unsigned int fromNode, unsigned int toNode);
bool ALGraphHasEdge(ALGraph *g, unsigned int fromNode, unsigned int toNode);
void ALGraphPrint(ALGraph *g);

#endif 