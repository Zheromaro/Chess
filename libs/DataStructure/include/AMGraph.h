#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>

typedef struct AMGraph AMGraph;

AMGraph *NewAMGraph(unsigned int numNodes);
void FreeAMGraph(AMGraph **pg);
bool AMGraphInsertEdge(AMGraph *g, unsigned int fromNode, unsigned int toNode);
bool AMGraphRemoveEdge(AMGraph *g, unsigned int fromNode, unsigned int toNode);
bool AMGraphHasEdge(AMGraph *g, unsigned int fromNode, unsigned int toNode);
void AMGraphPrint(AMGraph *g);

#endif 