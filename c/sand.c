#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <stdbool.h>
#include "graph.h"
#include "laurens.h"
#include "alternating.h"
#include "cycles.h"
#include "color.h"


struct Thing
{
    int a;
    int b;
};


void fun(int ***array) {
    **array = NULL; 
}



int main ( int argc, char *argv[] )
{
    int N = 100000, d = 50, i;
    printf("N=%i, d=%i\n\n", N, d);
    srandom(time(NULL));
    int *right_sides, *matching, unmatched;
    struct Graph *graph, *graphCopy;

    right_sides = createRightSides(N,d);
    graph = createRandomRegBipartite(N,d,false,right_sides);
    graphCopy = createRandomRegBipartite(N,d,false,right_sides);

    int **matchings;
    matchings = colorGraphLaurens(graph);
    printf("\n\n");

    // WARNING: BAD TIME COMPLEXITY O(d*E)
    for (i=d-1; i>=0; i--) {
        validateMatching(matchings[i], graphCopy);
        printf("Matching %i valid. \n", i);
        removeMatching(graphCopy, matchings[i]);
    }




}

