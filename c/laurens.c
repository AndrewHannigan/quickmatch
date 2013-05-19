#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "graph.h"
#include "laurens.h"


/**** MAIN METHOD ****/

int laurensPartial(struct Graph *graph, int **outMatching, int *outUnmatched, int stopSize)
{
//  (1) Initialize     
    int steps = 0;
    int N = graph->V/2, i;
    struct NodeList cur;
    struct Node *match, *crawler, *popNode;
    int *matching = initMatching(N);
    short *matched  = initMatched(N*2); 
    short *crawler_table  = initMatched(N*2); 
    struct NodeList *nodeLists = graph->array;
    struct NeighborhoodStacks *ns = createNeighborhoodStacks(graph);

    steps += N + 2*N + 2*N;

//  (2) Main         
    for (i=graph->V/2; i>stopSize; i--) {
        //printMatching(matching, graph->V/2);
        //printGraph(graph);
        // get min degree node and match to its min degree neighbor 
        popNode = getNextNode(ns,graph,matched,graph->d, &steps);
        if (popNode == NULL) 
            break;
        cur = nodeLists[popNode->label];        
        free(popNode);
        match = getMatch(&cur,graph,matched, &steps);

        // insert result into matching
        insertIntoMatching(cur.label, match->label, matching, matched, nodeLists, ns, &steps, crawler_table);
    }
                

    // output to pointers
    *outMatching = matching;
    *outUnmatched = countUnmatched(matching, graph->V / 2);

    // free dat memory doh
    free(matched);
    free(crawler_table);
    freeNS(ns, graph);

    completeMatching(outMatching, graph->V/2);

    return steps;
}



int laurens(struct Graph *graph, int **outMatching, int *outUnmatched) 
{
    return laurensPartial(graph, outMatching, outUnmatched, 0);
}


//int randomMatching(struct Graph *graph, int **outMatching)
//{
//    *matching = malloc(graph->V * sizeof(int));
//    struct Node *crawler;
//    int i = 0;
//    
//    for (i=0; i<graph->V; i++) {
//        (*matching)[i] = -1;
//    }
//
//    for (i=0; i<graph->V; i++) {
//
//        if (matching[i] != -1) 
//            continue;
//
//        crawler = graph->array[i].head;
//        while (crawler != NULL) {
//            if (matching[crawler->label] == -1) {
//                matching[i] = crawler->label;
//                matching[crawler->label] = i;
//                break; 
//            }
//            crawler = crawler->next;
//        }
//    }
//    
//
//
//}




/****  REFACTORED CODE FOR LAURENS() ****/

int *initMatching(int N) {
    int *matching = malloc(N * sizeof(int)); 
    int i;
    for(i=0; i < N; ++i)
        matching[i] = -1;
    return matching;
}

// 0 -> unmatched, 1 -> matched.
short *initMatched(int N) {
    int i;
    short *matched = malloc(N*2 * sizeof(short));
    for(i=0; i < N*2; ++i) 
        matched[i] = 0;
    return matched;
}


void insertIntoMatching(int a, int b, int *matching, short *matched, struct NodeList *nodeLists, struct NeighborhoodStacks *ns, int *steps, short *crawler_table) {
    // insert result into matching
    assert(matched[a] == 0); 
    assert(matched[b] == 0);
    if (a < b)
        matching[a] = b;
    else 
        matching[b] = a;

    matched[a] = 1;
    matched[b] = 1;
    nodeLists[a].neighborhood = 0;
    nodeLists[b].neighborhood = 0;

    // update all data structures for adjacent nodes
    struct Node *crawler = nodeLists[a].head;
    while (crawler != NULL) { 
        if (nodeLists[crawler->label].neighborhood != 0 && crawler_table[crawler->label] == 0) {
            nodeLists[crawler->label].neighborhood-= 1;
            crawler_table[crawler->label] = 1;
            if (nodeLists[crawler->label].neighborhood > 0) {
                pushNS(crawler->label, nodeLists[crawler->label].neighborhood, ns);
            }
        }
        crawler = crawler->next;
    }
    // reset crawler_table 
    crawler = nodeLists[a].head;
    while (crawler != NULL) {
        crawler_table[crawler->label] = 0;
        crawler = crawler->next;
    }



    crawler = nodeLists[b].head;
    while (crawler != NULL) { 
        if (nodeLists[crawler->label].neighborhood != 0 && crawler_table[crawler->label] == 0) {
            nodeLists[crawler->label].neighborhood -= 1;
            crawler_table[crawler->label] = 1;
            if (nodeLists[crawler->label].neighborhood > 0) {
                pushNS(crawler->label, nodeLists[crawler->label].neighborhood, ns);
            }
        }
        crawler = crawler->next;
    }

    crawler = nodeLists[b].head;
    while (crawler != NULL) {
        crawler_table[crawler->label] = 0;
        crawler = crawler->next;
    }

    *steps += 21;
}





/**** HELPER METHODS ****/

struct Node* getNextNode(struct NeighborhoodStacks *ns, struct Graph *graph, short *matched, int d, int *steps)
{
    int n = 1;
    struct Node *cur;
    *steps += 2;
    for (n=1; n<d+1; ++n) {
        cur = popNS(n, ns);
        *steps += 1;
        while (cur != NULL) {
            if(matched[cur->label] == 0 && graph->array[cur->label].neighborhood != 0)
                return cur;
            free(cur);
            cur = popNS(n,ns);
            *steps += 5;
        }
    }

    return NULL; // no free node remaining, end algorithm.
}



struct Node* getMatch(struct NodeList *current, struct Graph *graph, short *matched, int *steps)
{
    struct NodeList cur = *current;
    struct NodeList *nodeLists = graph->array;
    struct Node *match = NULL; 
    struct Node *crawler = nodeLists[cur.label].head;
    *steps += 4;
    
    // find first valid neighbor
    while (crawler != NULL) {
        if (matched[crawler->label] == 0) {
            match = crawler;
            break;
        }
        crawler = crawler->next;
        *steps += 3;
    }
    assert(match != NULL); 
     
    // find min degree neighbor
    while (crawler != NULL) {
        if (nodeLists[crawler->label].neighborhood > 0 \
              && nodeLists[crawler->label].neighborhood < nodeLists[match->label].neighborhood \
              && matched[crawler->label] == 0) { 
            match = crawler; 
        }
        crawler = crawler->next;
        *steps += 6;
    }
    return match;
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
int validateMatching(int *matching, struct Graph *graph)
{
    struct Node *crawler;
    int i;
    for(i=0; i<graph->V; i++) {
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

    short *matched = malloc(graph->V*sizeof(short));    
    for(i=0; i<graph->V; ++i) {
        matched[i] = 0;
    }

    // check that no node is matched more than once
    int unmatched = 0;
    for (i=0; i<graph->V; ++i) {
        if (matching[i] != -1) {
            assert(matched[matching[i]] == 0);
            matched[matching[i]] = 1;
        }
        else {
            unmatched++; 
        }
    }

    free(matched);
    return unmatched/2;
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




