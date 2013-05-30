
#ifndef LAURENS_H
#define LAURENS_H


int quickmatchPartial(struct Graph *graph, int **outMatching, int *outUnmatched, int stopSize);

int quickmatch(struct Graph *graph, int **outMatching, int *outUnmatched);

int *initMatching(int N);

short *initMatched(int N);

void insertIntoMatching(int a, int b, int *matching, short *matched, struct NodeList *nodeLists, struct NeighborhoodStacks *ns, int *steps, short *crawler_table);

void printMatched(short *matched, int N); 

void printMatching(int *matching, int N);

struct Node* getNextNode(struct NeighborhoodStacks *ns, struct Graph *graph, short *matched, int d, int *steps);

struct Node* getMatch(struct NodeList *current, struct Graph *graph, short *matched, int *steps);

int countUnmatched(int *matching, int N);

int validateMatching(int *matching, struct Graph *graph);

int isMatchingComplete(struct Graph *graph, int *matching);

int *copyMatching(int *matching, int N);

void completeMatching(int **matching, int N);

#endif
