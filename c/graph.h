#ifndef GRAPH_H
#define GRAPH_H

// A structure to represent an adjacency list node
struct Node
{
    int label;
    struct Node* next;
    struct Node* prev;
    struct Node* twin; // points to the other node in the adjaceny list that also represents this edge.  For (u,v), twin would point to (v,u).

};

// A structure to represent an adjacency list 
struct NodeList
{
    int label;
    int neighborhood;
    struct Node* head;  // pointer to head node of list
};

// A structure to represent a graph. A graph is an array of adjacency lists.
// Size of array will be V (number of vertices in graph)
struct Graph
{
    int V;
    int d;
    struct NodeList* array; // label of every node list must equal its index in this array.
};

// A structure that tracks the degree of every node.  Allows for min degree
// look up in amortized O(d) time.
struct NeighborhoodStacks
{
    struct NodeList* array;
};

// A utility function to create a new adjacency list node
struct Node* createNode(int label);

// A utility function that creates a graph of V vertices
struct Graph* initGraph(int V,int d);

// Create right sides
int *createRightSides(int N, int d);

// returns a random d-regular bipartite graph
struct Graph* createRandomRegBipartite(int N, int d, int removeMultiEdges, int *right_sides);

void resetGraph(struct Graph *graph); 

// Removes duplicates from the NodeList.  label_table is a char array of length 2*N. 
void uniquifyNodeList(struct NodeList *list, int N, short *label_table, struct Graph *graph);


void computeNeighborhoods(struct Graph *graph);

void deleteNodeFromNodelist(struct Node *node, int sourceLabel, struct Graph *graph);

// A utility function that creates a NeighborhoodStacks struct
struct NeighborhoodStacks* createNeighborhoodStacks(struct Graph *graph);

// Stack functionality for NeighborhoodStacks struct
void pushNS(int label, int neighborhood, struct NeighborhoodStacks* ns);
struct Node* popNS(int neighborhood, struct NeighborhoodStacks* ns);

void printNeighborhoodStacks(struct NeighborhoodStacks* ns, int d);

void freeGraph(struct Graph *graph);

// Adds an edge to an undirected graph
void addEdge(struct Graph* graph, int a, int b);

// A utility function to print the adjacency list representation of graph
void printGraph(struct Graph* graph);

// return a random number between 0 and limit inclusive.
int rand_lim(int limit);

#endif

