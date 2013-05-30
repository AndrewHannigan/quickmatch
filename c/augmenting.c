#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "graph.h"
#include "augmenting.h"
#include "quickmatch.h"


/*
   NOTES:

    *All of these methods assume that the matching array is length 2*N

    *TODO: look into whether dfs() is working correctly.  InitVisitedDFS is sticking the target vertices into the stack at the start.  This should be screwing up dfs().



     _______   _______     _______.        ___      .__   __.  _______     .______    _______     _______.
    |       \ |   ____|   /       |       /   \     |  \ |  | |       \    |   _  \  |   ____|   /       |
    |  .--.  ||  |__     |   (----`      /  ^  \    |   \|  | |  .--.  |   |  |_)  | |  |__     |   (----`
    |  |  |  ||   __|     \   \         /  /_\  \   |  . `  | |  |  |  |   |   _  <  |   __|     \   \
    |  '--'  ||  |    .----)   |       /  _____  \  |  |\   | |  '--'  |   |  |_)  | |  |    .----)   |   
    |_______/ |__|    |_______/       /__/     \__\ |__| \__| |_______/    |______/  |__|    |_______/    
                                                                                        
*/


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

void traceBackAugmentingPath(int *matching, struct QsNode *theStart)
/* 
    Flip all the edges on the path, stop when we reach the end of the path
*/
{
    assert(theStart != NULL);
    struct QsNode *cur = theStart;
    struct QsNode *next = theStart->prevSearch;
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


int bfs(struct Graph *graph, int *matching, int *bfs_path_length)
{

    // initialize visited, targets, and oldQueue  (REFACTOR WITH initVisit?)
    struct QsNode **visited = malloc(graph->V * sizeof(struct QsNode*));
    struct QsNode **targets = malloc(graph->V * sizeof(struct QsNode*));    

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
    *bfs_path_length = 0;
    while (targetFound == -1) {
        targetFound = bfsWave(graph, level, targets, visited, oldQueue, newQueue, graph->V/2, matching, &steps);
        free(oldQueue);
        oldQueue = newQueue;
        newQueue = createQueue();
        (*bfs_path_length)++;
        level++;
    }

    free(newQueue);
    traceBackAugmentingPath(matching, visited[targetFound]);

    freeQsNodePtrArray(visited, graph->V/2);
    freeQsNodePtrArray(targets, graph->V/2);
    return steps;
}


int dfs(struct Graph *graph, int *matching)
/*
    !! Must realloc matching so it contains nodes on right too !!
    
    DFS from the left, BFS from the right
*/
{
    struct Stack *stack = createStack();
    struct QsNode **visited = initVisitedDFS(graph, matching, stack, LEFT);
    struct QsNode **targets = initVisitedDFS(graph, matching, stack, RIGHT);

    int targetFound = -1;
    int steps = 0;

    while (true) {
        // left wave
        targetFound = dfsWave(graph, LEFT, targets, visited, stack, graph->V/2, matching, &steps);
        if (targetFound != -1) {break;}
    }

    traceBackAugmentingPath(matching, visited[targetFound]);

    freeQsNodePtrArray(visited, graph->V/2);
    freeQsNodePtrArray(targets, graph->V/2);
    free(stack);
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

    traceBackAugmentingPath(matching, leftVisited[targetFound]);
    traceBackAugmentingPath(matching, rightVisited[targetFound]);

    freeQsNodePtrArray(leftVisited, graph->V/2);
    freeQsNodePtrArray(rightVisited, graph->V/2);
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
    
    int dfs_steps = 0;
    int bfs_steps = 0;
    while (true) {
        // left wave
        while (dfs_steps <= bfs_steps) {
            targetFound = dfsWave(graph, LEFT, rightVisited, leftVisited, stack, graph->V/2, matching, &dfs_steps);
            if (targetFound != -1) {break;}
        }

        // right wave
        targetFound = bfsWave(graph, rightLevel, leftVisited, rightVisited, oldQueue, newQueue, graph->V/2, matching, &bfs_steps);
        rightLevel++;
        if (targetFound != -1) {break;}
        free(oldQueue);
        oldQueue = newQueue;
        newQueue = createQueue();
    }

    traceBackAugmentingPath(matching, leftVisited[targetFound]);
    traceBackAugmentingPath(matching, rightVisited[targetFound]);

    freeQsNodePtrArray(leftVisited, graph->V/2);
    freeQsNodePtrArray(rightVisited, graph->V/2);
    free(stack);
    return dfs_steps+bfs_steps;
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

    traceBackAugmentingPath(matching, leftVisited[targetFound]);
    traceBackAugmentingPath(matching, rightVisited[targetFound]);

    freeQsNodePtrArray(leftVisited, graph->V/2);
    freeQsNodePtrArray(rightVisited, graph->V/2);
    free(stackLeft);
    free(stackRight);
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




/*
     __    __    ______   .______     ______ .______        ______    _______ .___________.       __  ___      ___      .______      .______   
    |  |  |  |  /  __  \  |   _  \   /      ||   _  \      /  __  \  |   ____||           |      |  |/  /     /   \     |   _  \     |   _  \  
    |  |__|  | |  |  |  | |  |_)  | |  ,----'|  |_)  |    |  |  |  | |  |__   `---|  |----`______|  '  /     /  ^  \    |  |_)  |    |  |_)  | 
    |   __   | |  |  |  | |   ___/  |  |     |      /     |  |  |  | |   __|      |  |    |______|    <     /  /_\  \   |      /     |   ___/  
    |  |  |  | |  `--'  | |  |      |  `----.|  |\  \----.|  `--'  | |  |         |  |           |  .  \   /  _____  \  |  |\  \----.|  |      
    |__|  |__|  \______/  | _|       \______|| _| `._____| \______/  |__|         |__|           |__|\__\ /__/     \__\ | _| `._____|| _|      
                                                                                                                                              
*/



struct Graph *buildUnmatchedSubgraph(struct Graph *graph, int *matching, int **labelMap, int *steps) 
{
    int i, cur=0, oldN = graph->V/2;
    struct NodeList nodeList;
    struct Node *crawler=NULL, *temp;

    int newN = 0;
    for (i=0; i<graph->V/2; i++) 
        if (matching[i] == -1) newN++;

    *labelMap = (int *) malloc(newN * 2 * sizeof(int)); // subgraph label to original graph label 
    int *old2NewLabelMap = (int *) malloc(oldN * 2 * sizeof(int));

    for (i=0; i<2*oldN; i++) {
        if (matching[i] == -1) {
            (*labelMap)[cur] = i;
            old2NewLabelMap[i] = cur; 
            cur++;
        }
        else {
            old2NewLabelMap[i] = -2;
        }
    }

    struct Graph *subgraph = initGraph(2*newN, graph->d);
    
    *steps += newN + newN*graph->d;
  
    cur = 0;
    for (i=0; i<2*oldN; i++) {
        if (matching[i] == -1) {
            assert(cur<2*newN); 
            subgraph->array[cur].label = old2NewLabelMap[i];
            // copy adjaceny list
            crawler = graph->array[i].head;
            while(crawler!=NULL) {
                if (matching[crawler->label] == -1) {
                    temp = subgraph->array[cur].head;
                    subgraph->array[cur].head = createNode(old2NewLabelMap[crawler->label]);
                    subgraph->array[cur].head->next = temp;
                    *steps += 3;
                }
                crawler = crawler->next;
                *steps += 3;
            }
            cur++;
            *steps += 5;
        }
    }

    assert(cur == newN*2);

    computeNeighborhoods(subgraph);
    free(old2NewLabelMap);
    return subgraph;
}


void insertToHungarian(struct Graph *hungarianGraph, int srcLabel, int destLabel) {
    struct Node *destNode = createNode(destLabel);
    struct Node *curHead  = hungarianGraph->array[srcLabel].head;

    if (curHead == NULL) {
        hungarianGraph->array[srcLabel].head = destNode;
    } else {
        curHead->prev = destNode;
        destNode->next = curHead; 
        destNode->prev = NULL;
        hungarianGraph->array[srcLabel].head = destNode;   

    }
}



int hungarianWave(struct Graph *originalGraph, struct Graph *hungarianGraph, int level, int *targets, int *visited, struct Queue *oldQueue, struct Queue *newQueue, int *matching, int *steps)
/*
    Creates the next level of a hungarian tree.  A hungarian tree contains EVERY and ONLY the shortest length augmenting paths in originalGraph.
*/
{

    struct QsNode *cur, *temp;
    struct Node *crawler, *newNode, *currentHead;
    enum SIDE side;
    int waveResult;
    if (level % 2 == 0) {
        side = HOME;
        waveResult = 2; 
    } 
    else {
        side = AWAY;
        waveResult = 0;
    }

    while (oldQueue->head != NULL) {
        cur = dequeue(oldQueue);

        if (side == HOME) { 
            // traverse to all unvisited nodes, use edges not in matching
            crawler = originalGraph->array[cur->label].head;
            while (crawler != NULL) {
                (*steps) = (*steps) + 1;
                if (visited[crawler->label] == 0) {
                    if (waveResult==2) waveResult = 0; // 2 indicates that there definitely are NO augmenting paths left.  0 indicates we just haven't found one yet.
                    // insert into hungarianGraph
                    insertToHungarian(hungarianGraph, cur->label, crawler->label);
                    if (targets[crawler->label] == 1) {
                        waveResult = 1;
                    } 
                    enqueue(createQsNode(crawler->label), newQueue);
                }
                crawler = crawler->next;
            }
        } else { // side == AWAY 
            assert(matching[cur->label] != -1); // if it does, we should have stopped in previous iteration, unmatched vertex on AWAY was found
            insertToHungarian(hungarianGraph, cur->label, matching[cur->label]);
            enqueue(createQsNode(matching[cur->label]), newQueue);
            (*steps) = (*steps) + 1;
        }
        free(cur);
    }

    // mark nodes visited this wave at the end (allows algo to revisit nodes within this wave)
    cur = newQueue->head;
    while (cur != NULL) {
//        (*steps)++
        if (visited[cur->label] == 0) {
            visited[cur->label] = 1; 
            cur = cur->next;
        } else if (visited[cur->label] == 1) {
            // cur is a duplicate, delete from queue
            temp = cur->next;
            deleteQsNodeFromQueue(cur, newQueue);        
            cur = temp;
        } else {
            assert(false);
        }
    }

    return waveResult;
}



void deleteQsNodeFromQueue(struct QsNode *node, struct Queue *queue)
{
    // BUG - even though this should delete node from queue, we somehow see it in the queue later on.
 
    if (queue->head == node) {
        queue->head = node->next;
    }
    if (queue->end == node) {
        queue->end = node->prev;
    }
    if (node->next != NULL) {
        node->next->prev = node->prev;
    }
    if (node->prev != NULL) {
        node->prev->next = node->next;
    }
    free(node);
}



int *initTargetsHungarian(struct Graph *graph, int *matching)
{
    // initialize visited 
    int *targets = malloc(graph->V * sizeof(int));
    int i;
    for (i=0; i<graph->V; i++) {
        targets[i] = 0;
    }

    // Don't enqueue if we are on the right, these are the targets.
    for (i=graph->V/2; i<graph->V; i++) {
        if (matching[i] == -1) {
            targets[i] = 1;
        }
    }    
    return targets;
}


int *initVisitedHungarian(struct Graph *graph, int *matching, struct Queue **queue)
{
    // initialize visited 
    int *visited = malloc(graph->V * sizeof(int));
    int i;
    for (i=0; i<graph->V; i++) {
        visited[i] = 0;
    }
    

    *queue = createQueue();
    for (i=0; i<graph->V/2; i++) {
        if (matching[i] == -1) {
            enqueue(createQsNode(i), *queue);
            visited[i] = 1;
        }
    }

    return visited;
}


struct QsNode **initVisitedHopcroftDFS(struct Graph *graph, int *matching, struct Stack *stack)
{
    // initialize visited, targets, and oldQueue 
    struct QsNode **visited = malloc(graph->V * sizeof(struct QsNode*));
    int i;
    for (i=0; i<graph->V; i++) {
        visited[i] = NULL;
    }

    for (i=0; i<graph->V/2; i++) {
        if (matching[i] == -1) {
            struct QsNode* newNode = createQsNode(i);
            newNode->prevSearch = NULL;
            newNode->source = NULL;
            push(newNode,stack);
            visited[i] = newNode;
        }
    }    
    return visited;
}


void visitNewNodeHopcroftDFS(struct Node *discoveredNode, struct QsNode *cur, struct QsNode **visited, struct Stack *stack, int *steps) 
{
    struct QsNode* newNode = createQsNode(discoveredNode->label);
    newNode->prevSearch = cur;
    newNode->source = discoveredNode;
    push(newNode,stack);
    visited[discoveredNode->label] = newNode;
}


int hopcroftDFS(struct Graph *hungarianGraph, int *matching, int *steps)
{
    struct Stack *stack = createStack();
    struct QsNode **visited = initVisitedHopcroftDFS(hungarianGraph, matching, stack);
    //(*steps) += hungarianGraph->V;

    int targetFound = false;
    struct QsNode *cur;
    struct Node *discovered;

    while (!targetFound && stack->head != NULL) {
        cur = pop(stack);

        // Delete cur from hungarian graph
        if (matching[cur->label] != -1)
            deleteNodeFromNodelist(cur->source, cur->prevSearch->label, hungarianGraph);

        // Explore nodes
        discovered = hungarianGraph->array[cur->label].head;
        (*steps) = (*steps) + 1;

        while (discovered != NULL) { 
            if (visited[discovered->label] == NULL) {
                visitNewNodeHopcroftDFS(discovered, cur, visited, stack, steps);
                if ((matching[discovered->label] == -1) && (discovered->label >= hungarianGraph->V/2)) {
                    traceBackAugmentingPath(matching, visited[discovered->label]);
                    deleteNodeFromNodelist(discovered, cur->label, hungarianGraph);
                    targetFound = true;
                    break;
                }
            }
            discovered = discovered->next;
        }
    }

    // update unmatched array
    freeQsNodePtrArray(visited, hungarianGraph->V/2);
    free(stack);
    return targetFound;

}


struct Graph *createHungarianGraph(struct Graph *originalGraph, int *matching, int *steps)
{
    int waveResult = 0, N = originalGraph->V/2;
    struct Queue *oldQueue;
    int *visited = initVisitedHungarian(originalGraph, matching, &oldQueue);
    int *targets = initTargetsHungarian(originalGraph, matching);

    struct Queue *newQueue = createQueue();
    struct Graph *hungarianGraph = initGraph(2*N, originalGraph->d);
    int level = 0;

    //*steps += 3*originalGraph->V;   NOT COUNTING MALLOCS AGAINST HOPCROFT
    
    // BUILD HUNGARIAN GRAPH
    while (waveResult == 0) {
        // 0 = no target found yet, 1 = target found, 2 = no targets can ever be reached
        waveResult = hungarianWave(originalGraph, hungarianGraph, level, targets, visited, oldQueue, newQueue, matching, steps);
        if (waveResult == 2) {
            // TODO free hungarianGraph, newQueue, etc.
            // TODO make waveResult 0,1,2 stuff nicer, use ENUM maybe
            return NULL;
        }
        freeQueue(oldQueue);
        oldQueue = newQueue;
        newQueue = createQueue();
        level++;
    }

    computeNeighborhoods(hungarianGraph);
  
    // free stuff
    free(visited); 
    free(targets);
    freeQueue(newQueue);
    freeQueue(oldQueue);

    //*steps += 2*hungarianGraph->V;

    assert(hungarianGraph != NULL);
    return hungarianGraph;
}


int hopcroftPhase(struct Graph *originalGraph, int *matching, int *unmatched, int *steps) 
{
    int isMatchingMaximal;

    struct Graph *hungarianGraph = createHungarianGraph(originalGraph, matching, steps);
    if (hungarianGraph == NULL) {
        isMatchingMaximal = true;
        return isMatchingMaximal;
    } else {
        isMatchingMaximal = false;
    }

    int targetFound = true;
    while (targetFound) {
        targetFound = hopcroftDFS(hungarianGraph, matching, steps);
        if (targetFound)
            *unmatched = *unmatched - 1;
    }

    // free stuff
    freeGraph(hungarianGraph);
    return isMatchingMaximal;
}



int hopcroft(struct Graph *graph, int **matching) {
    *matching = malloc(graph->V*sizeof(int));
    int i,steps=0;
    for (i=0; i<graph->V; i++) {
        (*matching)[i] = -1;
    }

    //steps += graph->V;
    steps += hopcroftPartial(graph, *matching);
    return steps;
}



int hopcroftPartial(struct Graph *graph, int *matching) {
    int steps=0,i,unmatched=0;
    for (i=0; i<graph->V/2; i++) {
        if (matching[i] == -1) {
            unmatched++;
        }
    }
    

    int isMatchingMaximal = false;
    while (unmatched > 0 && !isMatchingMaximal) {
        isMatchingMaximal = hopcroftPhase(graph, matching, &unmatched, &steps);
    }

    return steps;
}



int quickmatch_hopcroft(struct Graph *graph, int **matching)
{
    assert(false);
//    int unmatched;
//   
//    // (1) Run quickmatch until unmatched subgraph is size N^(2/3)
//    int stopSize = (int) pow(graph->V/2, 0.6666);
//    int steps = quickmatchPartial(graph, matching, &unmatched, stopSize);
//
//    assert(unmatched==stopSize);
//    completeMatching(matching, graph->V/2);
//
//
//    // (2) Build unmatched subgraph
//    int *labelMap, *include, i;
//    int N = graph->V/2;
//    include = malloc(2*N*sizeof(int));
//    for (i=0; i<2*N; i++) {
//        if ((*matching)[i] == -1)
//            include[i] = 1;
//        else
//            include[i] = 0;
//    }
//    struct Graph *subgraph = buildUnmatchedSubgraph(graph, *matching, &labelMap, &steps);
//
//
//    // (3) Run hopcroft on remaining subgraph
//    int *subgraphMatching;
//    steps += hopcroft(subgraph, &subgraphMatching);
//
//
//    // (4) Convert 
//    for (i=0; i<subgraph->V; i++) {
//        if (subgraphMatching[i] != -1) {
//           // assert((*matching)[labelMap[i]] == -1);
//            (*matching)[labelMap[i]] = labelMap[subgraphMatching[i]];
//        }
//    }
//    freeGraph(subgraph);
//    free(subgraphMatching);
//  
//    return steps;
}



/*

      ______      __    __   _______  __    __   _______     _______.        ___      .__   __.  _______          _______.___________.    ___       ______  __  ___      _______.
     /  __  \    |  |  |  | |   ____||  |  |  | |   ____|   /       |       /   \     |  \ |  | |       \        /       |           |   /   \     /      ||  |/  /     /       |
    |  |  |  |   |  |  |  | |  |__   |  |  |  | |  |__     |   (----`      /  ^  \    |   \|  | |  .--.  |      |   (----`---|  |----`  /  ^  \   |  ,----'|  '  /     |   (----`
    |  |  |  |   |  |  |  | |   __|  |  |  |  | |   __|     \   \         /  /_\  \   |  . `  | |  |  |  |       \   \       |  |      /  /_\  \  |  |     |    <       \   \
    |  `--'  '--.|  `--'  | |  |____ |  `--'  | |  |____.----)   |       /  _____  \  |  |\   | |  '--'  |   .----)   |      |  |     /  _____  \ |  `----.|  .  \  .----)   |   
     \_____\_____\\______/  |_______| \______/  |_______|_______/       /__/     \__\ |__| \__| |_______/    |_______/       |__|    /__/     \__\ \______||__|\__\ |_______/    

*/


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
    node->prev = NULL;
    node->source = NULL;
    node->flag = 999;
    return node;
}


void freeQsNodePtrArray(struct QsNode **array, int N)
{
    int i;
    for (i=0; i<2*N; i++) {
        if (array[i] != NULL) {
            free(array[i]);
        }
    }
    free(array);
}

struct Queue* createQueue() {
    struct Queue *queue = (struct Queue*) malloc(sizeof(struct Queue)); 
    queue->head = NULL;
    queue->end  = NULL;
    return queue;
}


void freeQueue(struct Queue *queue) {
    struct QsNode *node = queue->head, *temp;
    while (node) {
        temp = node->next;
        free(node); 
        node = temp;
    }
    free(queue);
}


void enqueue(struct QsNode *node, struct Queue *queue)
{
    if (queue->end == NULL) {
        assert(queue->head == NULL);
        queue->end = node;
        queue->head = node;
        node->prev = NULL;
        node->next = NULL;
    }
    else {
        queue->end->next = node;
        node->prev = queue->end;
        node->next = NULL;
        queue->end = node;
    }
} 



struct QsNode* dequeue(struct Queue *queue)
{
    struct QsNode *temp = queue->head;
    if (temp == NULL) {
        return NULL;
    }

    queue->head = queue->head->next;

    if (queue->head != NULL) {
        queue->head->prev = NULL;
    }

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

void printIntArray(int *array, int length) {
    int i;
    for (i=0; i<length; i++) {
        printf("%i\n", array[i]);
    }
}
