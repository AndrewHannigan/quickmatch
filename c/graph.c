#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "graph.h" 


// A utility function to create a new adjacency list node
struct Node* createNode(int label)
{
    struct Node* node = (struct Node*) malloc(sizeof(struct Node));
    node->label = label;
    node->next = NULL;
    node->prev = NULL;
    node->twin = NULL;
    return node;
}
 


// A utility function that creates a graph of V vertices
struct Graph* initGraph(int V, int d)
{
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
    graph->V = V;
    graph->d = d;
 
    // Create an array of adjacency lists.  Size of array will be V
    graph->array = (struct NodeList*) malloc(V * sizeof(struct NodeList));
 
     // Initialize each adjacency list as empty by making head as NULL
    int i;
    for (i = 0; i < V; ++i) {
        graph->array[i].head = NULL;
        graph->array[i].label = i;
    }

    return graph;
}

int *createRightSides(int N, int d)
{
    int* right_sides = malloc(N * d * sizeof(int));
    int i, j;
    for (i=0; i<N; ++i) {
        for (j=0; j<d; ++j) {
            right_sides[i*d+j] = i+N;
        }
    }

    // shuffle right_sides
    int temp,r;
    for (i=N*d-1; i>1; --i) {
        temp = right_sides[i];
        r = rand_lim(i);
        right_sides[i] = right_sides[r];
        right_sides[r] = temp;
    }
    return right_sides;
}


struct Graph* createRandomRegBipartite(int N, int d, int removeMultiEdges, int *right_sides)
{
    assert(removeMultiEdges == 0);
    struct Graph* graph = initGraph(N*2, d);

    // update adjacency list
    struct NodeList* array = graph->array;
    int i,j;
    struct Node *leftNode, *rightNode;
    for (i=0; i<N; ++i) {
        for (j=0; j<d; ++j) {
            // add to the left's adj list
            leftNode = createNode(right_sides[i*d+j]);            
            leftNode->next = array[i].head;
            leftNode->prev = NULL;
            if (leftNode->next)
                leftNode->next->prev = leftNode;
            array[i].head = leftNode;

            // add to the right's adj list
            rightNode = createNode(i);            
            rightNode->next = array[right_sides[i*d+j]].head;
            rightNode->prev = NULL;
            if (rightNode->next)
                rightNode->next->prev = rightNode;
            array[right_sides[i*d+j]].head = rightNode;  

            // connect nodes via twin field 
            leftNode->twin = rightNode;
            rightNode->twin = leftNode;
        }
    }


    // remove multi-edges if specified
    if (removeMultiEdges) { 
        short *label_table = malloc(N * 2 * sizeof(short));
        memset(label_table, 0, N*2*sizeof(short));
        for (i=0; i < graph->V; ++i)
            uniquifyNodeList(&graph->array[i], graph->V, label_table,graph);
        free(label_table);
    } 

    computeNeighborhoods(graph);

    return graph;
}


void resetGraph(struct Graph *graph)
{
    int i,count=0;
    struct Node *crawler;
    for (i=0; i<graph->V; i++) {
        crawler = graph->array[i].head;
        while (crawler!=NULL) {
            count++;
            crawler = crawler->next;
        }
        graph->array[i].neighborhood = count;
        count = 0;
    } 
}


// Removes duplicates from the NodeList.  label_table is a char array of length 2*N.
void uniquifyNodeList(struct NodeList *list, int N, short *label_table, struct Graph *graph)
{
    
    // Check that list is not empty
    if (list->head == NULL)
        return; 

    struct Node *cur;
    struct Node *temp;

    // zero out label_table 
    cur = list->head;
    while(cur != NULL) {
        label_table[cur->label] = 0; 
        cur = cur->next;
    }

    cur = list->head;
    while(cur != NULL) {
        if (label_table[cur->label] == 1 ) {
            // remove duplicate
            temp = cur->next;
            deleteNodeFromNodelist(cur, list->label, graph);
            cur = temp;
        } 
        else {
            label_table[cur->label] = 1;
            cur = cur->next;
        }
    }
  

    return;
}


void computeNeighborhoods(struct Graph *graph) 
{
    int i, neighborhood;
    struct Node *cur;

    // label_table
    short *label_table = malloc(graph->V * sizeof(short));
    for (i=0; i<graph->V; ++i) {
        label_table[i] = 0;
    }

    // compute neighborhoods
    for(i=0; i < graph->V; ++i) { 
        cur = graph->array[i].head;
        neighborhood = 0;
        while (cur != NULL) {
            if (label_table[cur->label] == 0) {
                ++neighborhood;
                label_table[cur->label] = 1; 
            }
            cur = cur->next;
        }
        graph->array[i].neighborhood = neighborhood;
        cur = graph->array[i].head;
        while (cur != NULL) {
            label_table[cur->label] = 0;
            cur = cur->next;
        }
    } 
    free(label_table);
}


void deleteNodeFromNodelist(struct Node *node, int sourceLabel, struct Graph *graph)
{
    if (node->next)
        node->next->prev = node->prev;
    if (node->prev)
        node->prev->next = node->next;
    if (graph->array[sourceLabel].head == node) 
        graph->array[sourceLabel].head = node->next;
    free(node);
}


struct NeighborhoodStacks* createNeighborhoodStacks(struct Graph *graph)
{
    struct NeighborhoodStacks* ns = (struct NeighborhoodStacks*) malloc(sizeof(struct NeighborhoodStacks)); 
    ns->array = (struct NodeList*) malloc((graph->d+1) * sizeof(struct NodeList)); 
    int i;
    for (i=0; i < graph->d+1; ++i) {
        ns->array[i].head = NULL;
    }
    for(i=0; i < graph->V; ++i) {
        pushNS(graph->array[i].label, graph->array[i].neighborhood, ns);
    }
    return ns;
}


void pushNS(int label, int neighborhood, struct NeighborhoodStacks* ns)
{
    struct Node* node = createNode(label); 
    node->next = ns->array[neighborhood].head;
    ns->array[neighborhood].head = node;
}    


struct Node* popNS(int neighborhood, struct NeighborhoodStacks* ns)
{
    struct Node* node = ns->array[neighborhood].head;
    if (node != NULL)
        ns->array[neighborhood].head = node->next;
    return node;
}    


void printNodeList(struct NodeList list, int label) 
{
    struct Node* pCrawl = list.head;
    printf("\n Stack for neighborhood %i ", label);
    while (pCrawl)
    {
        printf("-> %d", pCrawl->label);
        pCrawl = pCrawl->next;
    }
    printf("\n");
}

void printNeighborhoodStacks(struct NeighborhoodStacks* ns, int d)
{
    int i;
    for (i = 0; i < d+1; ++i)
    {
        printNodeList(ns->array[i],i);
    //    struct Node* pCrawl = ns->array[i].head;
    //    printf("\n Stack for neighborhood %i ", i);
    //    while (pCrawl)
    //    {
    //        printf("-> %d", pCrawl->label);
    //        pCrawl = pCrawl->next;
    //    }
    //    printf("\n");
    }
}


// Adds an edge to an undirected graph
void addEdge(struct Graph* graph, int a, int b)
{
    // Add an edge from a to b.  A new node is added to the adjacency
    // list of a.  The node is added at the begining
    struct Node* node = createNode(b);
    node->next = graph->array[a].head;
    graph->array[a].head = node;
 
    // Since graph is undirected, add an edge from b to a also
    node = createNode(a);
    node->next = graph->array[b].head;
    graph->array[b].head = node;
}
 
// A utility function to print the adjacency list representation of graph
void printGraph(struct Graph* graph)
{
    int v;
    for (v = 0; v < graph->V; ++v)
    {
        struct Node* pCrawl = graph->array[v].head;
        printf("\n Adjacency list of vertex %d\n head ", v);
        while (pCrawl)
        {
            printf("-> %d", pCrawl->label);
            pCrawl = pCrawl->next;
        }
        printf("  neighborhood=%i", graph->array[v].neighborhood); 
        printf("\n\n");
    }
}

void freeNodeList(struct NodeList *nodeList)
{
    struct Node *crawler = nodeList->head;
    struct Node *temp;
    while (crawler != NULL) {
        temp = crawler->next;
        free(crawler);
        crawler = temp;
    }
}



void freeNS(struct NeighborhoodStacks *ns, struct Graph *graph) 
{
    int d = graph->d;
    int i;
    struct Node *crawler, *temp;
    for (i=0; i<d+1; ++i) {
        crawler = ns->array[i].head;
        while (crawler != NULL) {
            temp = crawler->next;
            free(crawler);
            crawler = temp;
        }
    }
    free(ns->array);
    free(ns);
}



void freeGraph(struct Graph *graph)
{
    struct Node *crawler, *temp;
    int i;
    for (i=0; i<graph->V; ++i) {
        crawler = graph->array[i].head;
        while (crawler != NULL) {
            temp = crawler->next;
            free(crawler);
            crawler = temp;
        }
    }
    free(graph->array);
    free(graph);
}


// return a random number between 0 and limit exclusive.  Remember to seed (srand).
int rand_lim(int limit) 
{
    return random() % limit;
}

//int main()
//{
//    // Test
//    srandom(time(NULL));
//    int N = 5;
//    int d = 3;
//    int i;
//
//    struct Graph *graph = createRandomRegBipartite(N,d,true);
//    struct NeighborhoodStacks *ns = createNeighborhoodStacks(graph);
//    printNeighborhoodStacks(ns,graph->d);
//    popNS(3,ns);
//    printNeighborhoodStacks(ns,graph->d);
//    getNextNode(ns,matched,graph->d);
//    
//}
