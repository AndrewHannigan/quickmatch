#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "graph.h"
#include "laurens_with_rand_init.h"



int laurens(struct Graph *graph, int countSteps, int **outMatching, int *outUnmatched, int randomUntil) 
/*
    insert random matchings until N-k = randomUntil 
*/

{
/**************************/
/*     (1) Initialize     */
/**************************/
    int N = graph->V/2;
    int initCount = 0;
    int *matching = malloc((graph->V / 2) * sizeof(int)); // the actual matching
    short *matched  = malloc(graph->V * sizeof(short));   // matched[v] equals 0 if v is unmatched,  
                                                          // and 1 if v is matched.
    
    // set all matching to -1 (indicates unmatched)
    int i;
    for(i=0; i < graph->V/2; ++i) 
        matching[i] = -1;

    // set all matched to 0 
    for(i=0; i < graph->V; ++i) 
        matched[i] = 0;

    struct NodeList *nodeLists = graph->array;

    // initialize NeighboorhoodStacks
    struct NeighborhoodStacks *ns = createNeighborhoodStacks(graph);


/*****************************/
/* (2) Insert Random Edges   */
/*****************************/

    struct NodeList cur;
    struct Node *match, *crawler; 
    struct Stack *nodeStack = makeUnmatchedListForRandomMatching(graph->V/2);

    
    for (i=N; i>randomUntil; i--) {
        cur = getRandomNextNode(graph, matched, graph->d, nodeStack); 
        match = getRandomMatch(&cur, graph, matched);        

        // insert into matching
        assert(matched[cur.label] == 0);
        assert(matched[match->label] == 0) ;
        if (cur.label < match->label)
            matching[cur.label] = match->label;
        else
            matching[match->label] = cur.label;
        matched[cur.label]   = 1;
        matched[match->label] = 1;
        nodeLists[cur.label].neighborhood   = 0;
        nodeLists[match->label].neighborhood = 0;

        // update neighborhoods and stacks
        crawler = cur.head;
        while (crawler != NULL) {
            if (countSteps) {countSteps += 1;}
            if (nodeLists[crawler->label].neighborhood != 0) {
                nodeLists[crawler->label].neighborhood-= 1;
                if (nodeLists[crawler->label].neighborhood > 0) {
                    pushNS(crawler->label, nodeLists[crawler->label].neighborhood, ns);
                }
            }
            crawler = crawler->next;
        }

        crawler = nodeLists[match->label].head;
        while (crawler != NULL) {
            if (countSteps) {countSteps += 1;}
            if (nodeLists[crawler->label].neighborhood != 0) {
                nodeLists[crawler->label].neighborhood -= 1;
                if (nodeLists[crawler->label].neighborhood > 0) {
                    pushNS(crawler->label, nodeLists[crawler->label].neighborhood, ns);
                }
            }
            crawler = crawler->next;
        }
    }
    



/**************************/
/*       (3) Main         */
/**************************/
    nodeLists = graph->array;

    // find inital cur
    for(i=0;i<graph->V/2;i++) {
        if (matched[i] == 0) {
            cur = nodeLists[i];
            break;
        }
    }
    assert(i!=graph->V/2);

    for(i=0; i < graph->V; ++i) { 
        if (nodeLists[i].neighborhood < cur.neighborhood && nodeLists[i].neighborhood > 0 && matched[nodeLists[i].label] == 0)
            cur = nodeLists[i];
    }

 

    struct NodeList *temp;
    long mainCount = 0;

    printf("\nChose initial cur: %i\n", cur.label);
    while (true) { 
        
        match = getMatch(&cur,graph,matched,NULL);
        

        // insert into matching
        assert(matched[cur.label] == 0); 
        assert(matched[match->label] == 0) ;
        if (cur.label < match->label)
            matching[cur.label] = match->label;
        else 
            matching[match->label] = cur.label;
        matched[cur.label]   = 1;
        matched[match->label] = 1;
        nodeLists[cur.label].neighborhood   = 0;
        nodeLists[match->label].neighborhood = 0;
        if (countSteps) {mainCount += 5;}         

        // update neighborhoods and stacks
        crawler = cur.head;
        while (crawler != NULL) { 
            if (countSteps) {countSteps += 1;}
            if (nodeLists[crawler->label].neighborhood != 0) {
                nodeLists[crawler->label].neighborhood-= 1;
                if (nodeLists[crawler->label].neighborhood > 0) {
                    if (countSteps) {countSteps += 1;}
                    pushNS(crawler->label, nodeLists[crawler->label].neighborhood, ns);
                }
            }
            crawler = crawler->next;
        }

        crawler = nodeLists[match->label].head;
        while (crawler != NULL) { 
            if (countSteps) {countSteps += 1;}
            if (nodeLists[crawler->label].neighborhood != 0) {
                nodeLists[crawler->label].neighborhood -= 1;
                if (nodeLists[crawler->label].neighborhood > 0) {
                    if (countSteps) {countSteps += 1;}
                    pushNS(crawler->label, nodeLists[crawler->label].neighborhood, ns);
                }
            }
            crawler = crawler->next;
        }


        // get the NodeList of the next node 
        struct Node *popNode = getNextNode(ns,graph,matched,graph->d,&mainCount);
        if (popNode == NULL) 
            break;
        cur = nodeLists[popNode->label];        
        free(popNode);
    }


    // output to pointers
    *outMatching = matching;
    *outUnmatched = countUnmatched(matching, graph->V / 2);

    // free memory -- ALWAYS DO THIS AT THE VERY END
    free(ns->array);
    free(ns);
    free(matched);
    if (countSteps) {return initCount + mainCount;}
    else {return -1;}
}

struct Node* getNextNode(struct NeighborhoodStacks *ns, struct Graph *graph, short *matched, int d, long *count)
{
    int n = 1;
    struct Node *cur;
    for (n=1; n<d+1; ++n) {
        *count += 1;
        cur = popNS(n, ns);
        while (cur != NULL) {
            if(matched[cur->label] == 0 && graph->array[cur->label].neighborhood != 0)
                return cur;
            free(cur);
            cur = popNS(n,ns);
            *count += 1;
        }
    }

    return NULL; // no free node remaining, end algorithm.
}



struct Node* getMatch(struct NodeList *current, struct Graph *graph, short *matched, long *count)
{
    struct NodeList cur = *current;
    struct NodeList *nodeLists = graph->array;
    struct Node *match = NULL; 
    struct Node *crawler = nodeLists[cur.label].head;
    
    // find first valid neighbor
    while (crawler != NULL) {
        if (count!=NULL) {*count += 1;} // count steps
        if (matched[crawler->label] == 0) {
            match = crawler;
            break;
        }
        crawler = crawler->next;
    }
    assert(match != NULL); 
    
    // find min degree neighbor
    while (crawler != NULL) {
        if (count!=NULL) {*count += 4;} 
        if (nodeLists[crawler->label].neighborhood > 0 \
              && nodeLists[crawler->label].neighborhood < nodeLists[match->label].neighborhood \
              && matched[crawler->label] == 0) { 
            if (count!=NULL) {*count += 1;} 
            match = crawler; 
        }
        crawler = crawler->next;
    }
    return match;
}


struct Node* getRandomMatch(struct NodeList *current, struct Graph *graph, short *matched)
{
    struct NodeList cur = *current;
    struct NodeList *nodeLists = graph->array;
    struct Node *match = NULL; 
    struct Node *crawler = nodeLists[cur.label].head;

    int stepTo;
    int length = 0;
    // find length of adj list
    while (crawler!=NULL) {
        length++;
        crawler = crawler->next;
    }
    
    // find random match
    assert(current->neighborhood >0);
    while (match == NULL) {
        stepTo = rand_lim(length); 
        match = nodeLists[cur.label].head;
        int i;
        for (i=0;i<stepTo;i++) {
            match = match->next; 
        }
        if (matched[match->label] == 1)
            match = NULL;
    }
    return match;
}


struct Stack *makeUnmatchedListForRandomMatching(int N)
{
    int *nodes = malloc(N*2 * sizeof(int));
    int temp,r,i;

    for (i=0; i<N*2; i++) {
        nodes[i] = i;
    }

    for (i=N*2-1; i>1; --i) {
        temp = nodes[i];
        r = rand_lim(i);
        nodes[i] = nodes[r];
        nodes[r] = temp;
    }
    
    struct Stack *stack = createStack();
    for (i=0; i<N*2; i++) {
        push(createQsNode(nodes[i]), stack);
    }
    return stack;
}

struct NodeList getRandomNextNode(struct Graph *graph, short *matched, int d, struct Stack *nodeStack)
{
    struct QsNode *cur;
    while (true) {
        cur = pop(nodeStack);
        if (cur==NULL) {
        // all remaining nodes are hermits
            assert(false);
        }
        if (matched[cur->label] == 0 && graph->array[cur->label].neighborhood > 0) {
            free(cur);
            break;
        }
        free(cur);
    }
    return graph->array[cur->label]; // no free node remaining, end algorithm.
}


void printMatched(short *matched, int N) 
{
    int i;

    printf("\nMatched\n");
    for (i=0; i<N*2; ++i) {
        printf("%i: %i\n", i, matched[i]);
    }
    return;
}


void printMatching(int *matching, int N) 
{
    int i;
    printf("\nMatching:\n");
    for (i=0; i<N; ++i) {
        printf("%i <--> %i\n", i, matching[i]);
    }
}



int countUnmatched(int *matching, int N) 
{
    int i;
    int count = 0;
    for (i=0; i<N; ++i) {
        if(matching[i] == -1) {
            count++;
        } 
    }
    return count;
}

// throws error if validation fails
void validateMatching(int *matching, short *matched, struct Graph *graph)
{
    struct Node *crawler;
    int i;
    for(i=0; i<graph->V / 2; i++) {
    // check if edges actually exist
        if (matching[i] != -1) {
            crawler = graph->array[i].head;
            while(crawler != NULL) {
                if (crawler->label == matching[i]) {break;}           
                crawler = crawler->next;
            }
            assert(crawler!=NULL);
        }
    }  

    for(i=0; i<graph->V; ++i) {
        matched[i] = 0;
    }

    for (i=0; i<graph->V/2; ++i) {
        if (matching[i] != -1) {
            assert(matched[matching[i]] == 0);
            matched[matching[i]] = 1;
        }
    }
}


void printQueue(struct Queue *queue) 
{
    struct QsNode *cur;
    cur = queue->head;
    if (!cur) {
        printf("Empty Queue\n\n");
        return;
    }

    printf("head --> %i ", cur->label);
    cur = cur->next;
    while (cur) {
        printf("--> %i ", cur->label);
        cur = cur->next;
    }

    printf("\n\n");
}


struct QsNode* createQsNode(int label) {
    struct QsNode *node = (struct QsNode*) malloc(sizeof(struct QsNode)); 
    node->label = label;
    node->prevSearch = NULL;
    node->next = NULL;
    return node;
}


struct Queue* createQueue() {
    struct Queue *queue = (struct Queue*) malloc(sizeof(struct Queue)); 
    queue->head = NULL;
    queue->end  = NULL;
    return queue;
}


void enqueue(struct QsNode *node, struct Queue *queue)
{
    if (queue->end == NULL) {
        assert(queue->head == NULL);
        queue->end = node;
        queue->head = node;
        return;
    }
    queue->end->next = node;
    queue->end = node;
} 


struct QsNode* dequeue(struct Queue *queue)
{
    struct QsNode *temp = queue->head;
    if (temp == NULL) {return NULL;}

    queue->head = queue->head->next;
    return temp;
} 



struct Stack* createStack() {
    struct Stack *stack = (struct Stack*) malloc(sizeof(struct Stack)); 
    stack->head = NULL;
    return stack;
}


void push(struct QsNode *node, struct Stack *stack)
{
    node->next = stack->head;
    stack->head = node; 
} 


struct QsNode* pop(struct Stack *stack)
{
    struct QsNode *temp = stack->head;
    if (temp == NULL) {return NULL;}
    stack->head = stack->head->next;
    return temp;
} 


void freeStack(struct Stack *stack)
{
    struct QsNode *crawler = stack->head;
    struct QsNode *temp;
    while(crawler!=NULL) {
        temp = crawler;
        crawler = crawler->next;
        free(temp);
    }
    free(stack);
}


void printStack(struct Stack *stack) 
{
    struct QsNode *cur;
    cur = stack->head;
    if (!cur) {
        printf("Empty Stack\n\n");
        return;
    }

    printf("head --> %i ", cur->label);
    cur = cur->next;
    while (cur) {
        printf("--> %i ", cur->label);
        cur = cur->next;
    }

    printf("\n\n");
}


void visitNewNodeBFS(int label, struct QsNode *cur, struct QsNode **visited, struct Queue *queue) 
{
    struct QsNode* newNode = createQsNode(label);
    newNode->prevSearch = cur;
    enqueue(newNode,queue);
    visited[label] = newNode;
}


void visitNewNodeDFS(int label, struct QsNode *cur, struct QsNode **visited, struct Stack *stack) 
{
    struct QsNode* newNode = createQsNode(label);
    newNode->prevSearch = cur;
    push(newNode,stack);
    visited[label] = newNode;
}


int bfsWave(struct Graph *graph, int level, struct QsNode **targets, struct QsNode **visited, struct Queue *oldQueue, struct Queue *newQueue, int N, int *matching, int *steps)
// oldQueue - contains all nodes from the previous level
// newQueue - contains all nodes from the current level

// graph - adjacency array representation
// startingSide - indicates the side the bfs search started on
// targets - indicates the vertices we are looking for, NULL is unvisted, ptr to a QsNode means visited (usually the vertices which have been reached by the opposing search tree)
// visited - indicates vertices already reached by this bfs (ie: it is in the queue already and will eventually be analyzed.  It is also already in the search tree) 

// bfsWave returns -1 if no target was found, and node label if target was found
{
    struct QsNode *cur;
    struct Node *crawler;
    enum SIDE side;
    if (level % 2 == 0) {side = HOME;} 
    else {side = AWAY;}

    while (oldQueue->head != NULL) {

        cur = dequeue(oldQueue);

        if (side == HOME) { 
            // traverse to all unvisited nodes, use edges not in matching
            crawler = graph->array[cur->label].head;
            while (crawler != NULL) {
                if (visited[crawler->label] == NULL) {
                    visitNewNodeBFS(crawler->label, cur, visited, newQueue);
                    if (targets[crawler->label] != NULL) {
                        return crawler->label;
                    }
                }
                crawler = crawler->next;
                (*steps)++;
            }
        } else if (side == AWAY) {
            if (matching[cur->label] != -1) {
                visitNewNodeBFS(matching[cur->label], cur, visited, newQueue);
                    if (targets[ matching[cur->label] ] != NULL) {
                        return matching[cur->label];
                    }
            }
            (*steps)++;
        } else {
            assert(false);
        }
    }
    return -1;
}


void traceBackAlternatingPath(int *matching, struct QsNode *start)
/* 
    Flip all the edges on the path, stop when we reach the end of the path
*/
{
    assert(start != NULL);
    struct QsNode *cur = start;
    struct QsNode *next = start->prevSearch;
    while (next != NULL) {
        if (matching[next->label] == cur->label) { // matching[cur->label] would have been modified in previously, matching[next->label] is unmodified.
            // flip out of matching (make sure not to overwrite previous matching updates)
            if (matching[cur->label] == next->label) { 
                matching[cur->label]  = -1; 
            }
            if (matching[next->label] == cur->label) { 
                matching[next->label] = -1;
            }
        }
        else {
            // flip into matching
            matching[cur->label] = next->label;
            matching[next->label] = cur->label;
        }

        cur = next;
        next = next->prevSearch;
    }
}

int bfs(struct Graph *graph, int *matching, struct QsNode **visited, struct QsNode **targets)
/*
    !! Must realloc matching so it contains nodes on right too !!
*/
{
    // initialize visited, targets, and oldQueue  (REFACTOR WITH initVisit?)
    int i,steps=0;
    for (i=0; i<graph->V; i++) {
        visited[i] = NULL;
        if (matching[i] == -1 && i >= graph->V/2) {
            targets[i] = createQsNode(i);
        } 
        else {
            targets[i] = NULL;
        }
    }

    struct Queue *oldQueue = createQueue();
    for (i=0; i<graph->V/2; i++) {
        if (matching[i] == -1) {
            visitNewNodeBFS(i, NULL, visited, oldQueue);
        }
    }    

    int level = 0;  
    int targetFound = -1;
    struct Queue *newQueue = createQueue();
    while (targetFound == -1) {
        targetFound = bfsWave(graph, level, targets, visited, oldQueue, newQueue, graph->V/2, matching, &steps);
        free(oldQueue);
        oldQueue = newQueue;
        newQueue = createQueue();
        level++;
    }


    free(newQueue);
    traceBackAlternatingPath(matching, visited[targetFound]);
    return steps;
}


struct QsNode **initVisitedBFS(struct Graph *graph, int *matching, struct Queue **queue, enum SIDE side)
{
    // initialize visited, targets, and oldQueue 
    struct QsNode **visited = malloc(graph->V * sizeof(struct QsNode*));
    int i;
    for (i=0; i<graph->V; i++) {
        visited[i] = NULL;
    }
    
    *queue = createQueue();

    if (side == LEFT) {
        for (i=0; i<graph->V/2; i++) {
            if (matching[i] == -1) {
                visitNewNodeBFS(i, NULL, visited, *queue);
            }
        }    
    }
    else if (side == RIGHT) {
        for (i=graph->V/2; i<graph->V; i++) {
            if (matching[i] == -1) {
                visitNewNodeBFS(i, NULL, visited, *queue);
            }
        }    
    }
    else {
        assert(false); // error
    }
    return visited;
}


struct QsNode **initVisitedDFS(struct Graph *graph, int *matching, struct Stack *stack, enum SIDE side)
{
    // initialize visited, targets, and oldQueue 
    struct QsNode **visited = malloc(graph->V * sizeof(struct QsNode*));
    int i;
    for (i=0; i<graph->V; i++) {
        visited[i] = NULL;
    }
    

    if (side == LEFT) {
        for (i=0; i<graph->V/2; i++) {
            if (matching[i] == -1) {
                visitNewNodeDFS(i, NULL, visited, stack);
            }
        }    
    }
    else if (side == RIGHT) {
        for (i=graph->V/2; i<graph->V; i++) {
            if (matching[i] == -1) {
                visitNewNodeDFS(i, NULL, visited, stack);
            }
        }    
    }
    else {
        assert(false); // error
    }
    return visited;
}

int bfs2bfs(struct Graph *graph, int *matching)
/*
    !! Must realloc matching so it contains nodes on right too !!
*/
{
    int leftLevel = 0;
    struct Queue *oldLeftQueue;
    struct QsNode **leftVisited = initVisitedBFS(graph, matching, &oldLeftQueue, LEFT);

    int rightLevel = 0;
    struct Queue *oldRightQueue;
    struct QsNode **rightVisited = initVisitedBFS(graph, matching, &oldRightQueue, RIGHT);

    int targetFound = -1;
    struct Queue *newLeftQueue = createQueue();
    struct Queue *newRightQueue = createQueue();
    
    int steps = 0;
    while (true) {
        // left wave
        // BUG --> oldLeftQueue is not being initialized correctly...
        targetFound = bfsWave(graph, leftLevel, rightVisited, leftVisited, oldLeftQueue, newLeftQueue, graph->V/2, matching, &steps);
        if (targetFound != -1) {break;}
        leftLevel++;
        free(oldLeftQueue);
        oldLeftQueue = newLeftQueue;
        newLeftQueue = createQueue();

        // right wave
        targetFound = bfsWave(graph, rightLevel, leftVisited, rightVisited, oldRightQueue, newRightQueue, graph->V/2, matching, &steps);
        rightLevel++;
        if (targetFound != -1) {break;}
        free(oldRightQueue);
        oldRightQueue = newRightQueue;
        newRightQueue = createQueue();
    }

    traceBackAlternatingPath(matching, leftVisited[targetFound]);
    traceBackAlternatingPath(matching, rightVisited[targetFound]);

    freeQsNodePtrArray(leftVisited, graph->V/2);
    freeQsNodePtrArray(rightVisited, graph->V/2);
    free(leftVisited);
    free(rightVisited);
    return steps;
}


int dfs2bfs(struct Graph *graph, int *matching)
/*
    !! Must realloc matching so it contains nodes on right too !!
    
    DFS from the left, BFS from the right
*/
{
    struct Stack *stack = createStack();
    struct QsNode **leftVisited = initVisitedDFS(graph, matching, stack, LEFT);

    int rightLevel = 0;
    struct Queue *oldQueue;
    struct QsNode **rightVisited = initVisitedBFS(graph, matching, &oldQueue, RIGHT);

    int targetFound = -1;
    struct Queue *newQueue = createQueue();
    
    int steps = 0;
    while (true) {
        // left wave
        targetFound = dfsWave(graph, LEFT, rightVisited, leftVisited, stack, graph->V/2, matching, &steps);
        if (targetFound != -1) {break;}


        // right wave
        targetFound = bfsWave(graph, rightLevel, leftVisited, rightVisited, oldQueue, newQueue, graph->V/2, matching, &steps);
        rightLevel++;
        if (targetFound != -1) {break;}
        free(oldQueue);
        oldQueue = newQueue;
        newQueue = createQueue();

    }

    traceBackAlternatingPath(matching, leftVisited[targetFound]);
    traceBackAlternatingPath(matching, rightVisited[targetFound]);

    freeQsNodePtrArray(leftVisited, graph->V/2);
    freeQsNodePtrArray(rightVisited, graph->V/2);
    free(stack);
    free(leftVisited);
    free(rightVisited);
    return steps;
}


int dfs2dfs(struct Graph *graph, int *matching)
/*
    !! Must realloc matching so it contains nodes on right too !!
    
    DFS from the left, BFS from the right
*/
{
    struct Stack *stackLeft = createStack();
    struct QsNode **leftVisited = initVisitedDFS(graph, matching, stackLeft, LEFT);

    struct Stack *stackRight = createStack();
    struct QsNode **rightVisited = initVisitedDFS(graph, matching, stackRight, RIGHT);

    int targetFound = -1;
    
    int steps = 0;
    while (true) {
        // left wave
        targetFound = dfsWave(graph, LEFT, rightVisited, leftVisited, stackLeft, graph->V/2, matching, &steps);
        if (targetFound != -1) {break;}


        // right wave
        targetFound = dfsWave(graph, RIGHT, leftVisited, rightVisited, stackRight, graph->V/2, matching, &steps);
        if (targetFound != -1) {break;}
    }

    traceBackAlternatingPath(matching, leftVisited[targetFound]);
    traceBackAlternatingPath(matching, rightVisited[targetFound]);

    freeQsNodePtrArray(leftVisited, graph->V/2);
    freeQsNodePtrArray(rightVisited, graph->V/2);
    free(stackLeft);
    free(stackRight);
    free(leftVisited);
    free(rightVisited);
    return steps;
}



int dfsWave(struct Graph *graph, enum SIDE startingSide, struct QsNode **targets, struct QsNode **visited, struct Stack *stack, int N, int *matching, int *steps)
// oldQueue - contains all nodes from the previous level
// newQueue - contains all nodes from the current level

// graph - adjacency array representation
// startingSide - indicates the side the bfs search started on
// targets - indicates the vertices we are looking for, NULL is unvisted, ptr to a QsNode means visited (usually the vertices which have been reached by the opposing search tree)
// visited - indicates vertices already reached by this bfs (ie: it is in the queue already and will eventually be analyzed.  It is also already in the search tree) 

// bfsWave returns -1 if no target was found, and node label if target was found
{
    struct QsNode *cur;
    struct Node *crawler;
    enum SIDE side;
    
    cur = pop(stack);    
    assert(cur!=NULL);

    if (startingSide == LEFT) {
        if (cur->label < graph->V/2) {
            side = HOME;
        }
        else {
            side = AWAY;
        }
    }
    else if (startingSide == RIGHT) {
        if (cur->label < graph->V/2) {
            side = AWAY;
        }
        else {
            side = HOME;
        }
    }
    else {assert(false);} // error


   if (side == HOME) { 
       // traverse to all unvisited nodes, use edges not in matching
       crawler = graph->array[cur->label].head;
       while (crawler != NULL) {
           if (visited[crawler->label] == NULL) {
               visitNewNodeDFS(crawler->label, cur, visited, stack);
               if (targets[crawler->label] != NULL) {
                   return crawler->label;
               }
           }
           crawler = crawler->next;
           (*steps)++;
       }
   } else if (side == AWAY) {
       if (matching[cur->label] != -1) {
           visitNewNodeDFS(matching[cur->label], cur, visited, stack);
               if (targets[ matching[cur->label] ] != NULL) {
                   return matching[cur->label];
               }
       }
       (*steps)++;
   } else {
       assert(false);
   }

    return -1;
}



void completeMatching(int **matching, int N) 
{
    *matching = realloc(*matching, 2*N*sizeof(int));
    int i,left,right;
    for (i=N; i<N*2; i++) {
        (*matching)[i] = -1;
    }
    for (i=0; i<N; i++) {
        left = i;
        right = (*matching)[i];
        if (right != -1)
            (*matching)[right] = left;
    } 
}

void freeQsNodePtrArray(struct QsNode **array, int N)
{
    int i;
    for (i=0; i<2*N; i++) {
        free(array[i]);
    }
}


int isMatchingComplete(struct Graph *graph, int *matching)
{
    int i;
    for (i=0; i<graph->V; i++) {
        if (matching[i] == -1) 
            return false;
    }
    return true;
}


int *copyMatching(int *matching, int N) 
{
    int *outMatching = (int*) malloc(2*N*sizeof(int));
    int i;
    for (i=0;i<2*N;i++) {
        outMatching[i] = matching[i];
    }
    return outMatching;
}




