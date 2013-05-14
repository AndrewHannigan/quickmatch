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

int coloringsCount = 0;
int **colorings;

int *convertGraphToMatching(struct Graph *graph) {
	assert(graph->d == 1);
	int *matching = malloc(graph->V * sizeof(int));
	int i;
	for (i=0; i<graph->V; i++) {
		matching[i] = graph->array[i].head->label;
	}
	free(graph);
	return matching;
}

int **colorGraphLaurens(struct Graph *graph)
{
	int d = graph->d;
	colorings = malloc(graph->d * sizeof(struct Graph*));
	colorLaurens(graph);
	return colorings;
}



void colorLaurens(struct Graph *graph)
{
    printf("Degree: %i\n", graph->d);

    // record matching
    if (graph->d == 1) {
    	colorings[coloringsCount] = convertGraphToMatching(graph);
    	coloringsCount++;
    }
    
    // trace out cycles to divide into two graphs
    else if (graph->d % 2 == 0) {
    	struct Graph *outGraph1;
    	struct Graph *outGraph2;
    	traceEvenDegree(graph, &outGraph1, &outGraph2);
    	colorLaurens(outGraph1);
    	colorLaurens(outGraph2);
    }
    
    // compute and remove a matching
    else {
        int *matching, unmatched, i;
        laurens(graph,&matching,&unmatched);
        for (i=0; i<unmatched; i++) {
            bfs2bfs(graph,matching);
        }

        removeMatching(graph, matching);
        colorings[coloringsCount] = matching;
        coloringsCount++;
        colorLaurens(graph);
    }
}
