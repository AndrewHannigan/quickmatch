#ifndef CYCLES_H
#define CYCLES_H


void removeMatching(struct Graph *graph, int *matching);

void traceEvenDegree(struct Graph *graph, struct Graph **outGraph1, struct Graph **outGraph2);

#endif
