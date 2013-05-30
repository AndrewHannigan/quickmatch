#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "graph.h"
#include "cycles.h"

/*
    These functions will dismantle and/or delete nodes from graph, make sure to pass it a copy and not the original.
    
    These functions will return graphs that a ready to be consumed by quickmatch (correct neighborhoods)
*/

// O(E) time

void removeMatching(struct Graph *graph, int *matching)
{
    int i;
    struct Node *crawler;
    for (i=0; i<graph->V; i++) {
        crawler = graph->array[i].head;
        while (crawler!=NULL) {
            if (crawler->label == matching[i])
                break;
            crawler = crawler->next;
        }
        assert(crawler!=NULL);
        deleteNodeFromNodelist(crawler, i, graph);
    }
    
    graph->d = graph->d - 1;
    for (i=0; i<graph->V; i++) 
        graph->array[i].neighborhood = graph->d;

}

int moveEdge(struct Graph *srcGraph, struct Graph *destGraph, int srcLabel) {
/* 
    moves both ends of an edge out of srcGraph and into destGraph

    INPUT
    srcGraph    = the original graph
    destGraph   = one of the two small graphs of degree d/2
    srcLabel    = the node we came from
    destLabel   = the node we stepped into

    OUTPUT
    destLabel (this is where we start our next step in the random walk to find a cycle)


*/    

    struct Node *node = srcGraph->array[srcLabel].head, *twinNode;
    int destLabel = node->label;
    assert(node != NULL);

    // remove node from source graph
    srcGraph->array[srcLabel].head = node->next;
    if (node->next) {
        node->next->prev = NULL;
    }

    // add node to destination graph
    node->next = destGraph->array[srcLabel].head;
    node->prev = NULL;
    if (node->next) {
        node->next->prev = node;
    }
    destGraph->array[srcLabel].head = node;


    // remove node's twin from source graph
    twinNode = node->twin;
    if (twinNode->next) 
        twinNode->next->prev = twinNode->prev;
    if (twinNode->prev)
        twinNode->prev->next = twinNode->next;
    if (twinNode == srcGraph->array[destLabel].head)
        srcGraph->array[destLabel].head = twinNode->next;

    // add node's twin to destination graph
    twinNode->next = destGraph->array[destLabel].head;
    twinNode->prev = NULL;
    if (twinNode->next)
        twinNode->next->prev = twinNode;
    destGraph->array[destLabel].head = twinNode;

    return destLabel;
}

void traceEvenDegree(struct Graph *graph, struct Graph **outGraph1, struct Graph **outGraph2)
{
    int d=graph->d, N=graph->V/2, new_d=d/2, i;
    assert(d % 2 == 0);

    *outGraph1 = initGraph(graph->V, new_d);
    *outGraph2 = initGraph(graph->V, new_d);
    
    int cur, starting_node = 0;
    
    while (starting_node < graph->V/2) { // We should always be able to start on the left.  Once left has no edges, right should have no edges.
        // find next unvisited node
        if (graph->array[starting_node].head == NULL) {
            starting_node++;
            continue;
        }

        // traceout cycle from starting_node
        // L --> R : outGraph1
        // R --> L : outGraph2
        cur = starting_node;
        do  {
            cur = moveEdge(graph, *outGraph1, cur);
            cur = moveEdge(graph, *outGraph2, cur);
        } while (cur != starting_node);
    }

    // update neighborhoods
    computeNeighborhoods(*outGraph1); 
    computeNeighborhoods(*outGraph2); 
}

