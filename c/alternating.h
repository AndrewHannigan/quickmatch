#ifndef ALTERNATING_H
#define ALTERNATING_H

enum SIDE            
{
    HOME = 0,
    AWAY = 1,
    LEFT = 2,
    RIGHT = 3
};

struct PrevSearchElem
{
    struct PrevSearchElem* next;
    struct QsNode* node;
};

struct QsNode
{
    int flag;
    int label;
    struct QsNode* prevSearch;
    struct QsNode* next;
    struct QsNode* prev;
    struct Node* source; // does this header file know about Node datatype?
};

struct Queue 
{
    struct QsNode *end;
    struct QsNode *head;
};


struct Stack 
{
    struct QsNode *head;
};


void printQueue(struct Queue *queue);

struct QsNode* createQsNode(int label);

void freeQueue(struct Queue *queue);

struct Queue* createQueue();

void enqueue(struct QsNode *node, struct Queue *queue);

struct QsNode* dequeue(struct Queue *queue);

struct Stack* createStack();

void push(struct QsNode *node, struct Stack *stack);

struct QsNode* pop(struct Stack *stack);

void deleteQsNodeFromQueue(struct QsNode *node, struct Queue *queue);

void printStack(struct Stack *stack);

void freeStack(struct Stack *stack);

void visitNewNodeBFS(int label, struct QsNode *cur, struct QsNode **visited, struct Queue *queue);

void visitNewNodeDFS(int label, struct QsNode *cur, struct QsNode **visited, struct Stack *stack);
 
void freeQsNodePtrArray(struct QsNode **array, int N);

struct QsNode **initVisitedBFS(struct Graph *graph, int *matching, struct Queue **queue, enum SIDE side);

struct QsNode **initVisitedDFS(struct Graph *graph, int *matching, struct Stack *stack, enum SIDE side);

int bfs2bfs(struct Graph *graph, int *matching);

int dfs2dfs(struct Graph *graph, int *matching);

int dfs2bfs(struct Graph *graph, int *matching);

int bfsWave(struct Graph *graph, int level, struct QsNode **targets, struct QsNode **visited, struct Queue *oldQueue, struct Queue *newQueue, int N, int *matching, int *steps);

int dfsWave(struct Graph *graph, enum SIDE startingSide, struct QsNode **targets, struct QsNode **visited, struct Stack *stack, int N, int *matching, int *steps);

int bfs(struct Graph *graph, int *matching, int *bfs_path_length);

int dfs(struct Graph *graph, int *matching);

void traceBackAlternatingPath(int *matching, struct QsNode *start);

struct Graph *buildUnmatchedSubgraph(struct Graph *graph, int *matching, int **labelMap, int *steps);



/* Hopcroft-Karp */

void visitNewNodeHopcroftDFS(struct Node *discoveredNode, struct QsNode *cur, struct QsNode **visited, struct Stack *stack, int *steps);

void insertToHungarian(struct Graph *hungarianGraph, int srcLabel, int destLabel);

void insertToHungarian(struct Graph *hungarianGraph, int srcLabel, int destLabel);

int hungarianWave(struct Graph *originalGraph, struct Graph *hungarianGraph, int level, int *targets, int *visited, struct Queue *oldQueue, struct Queue *newQueue, int *matching, int *steps);

int *initTargetsHungarian(struct Graph *graph, int *matching);

int *initVisitedHungarian(struct Graph *graph, int *matching, struct Queue **queue);

int hopcroftPhase(struct Graph *originalGraph, int *matching, int *unmatched, int *steps);

int hopcroftDFS(struct Graph *hungarianGraph, int *matching, int *steps);

int hopcroft(struct Graph *graph, int **matching);

int hopcroftPartial(struct Graph *graph, int *matching);

int laurens_hopcroft(struct Graph *graph, int **matching);

#endif
