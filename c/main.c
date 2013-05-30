#define SEED 2013

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "graph.h"
#include "laurens.h"
#include "alternating.h"
#include "testing.h"


int main ( int argc, char *argv[] ) 
{ 
    FILE *graphFile=fopen("NoteworthyGraphs.txt","a+");
    int max = 40;
    char* userInput = (char*)malloc(max); 
    char* N = (char*)malloc(max); 
    char* d = (char*)malloc(max); 

    printf("\n1 ---> altTest() - runs QM and tests all alternating-path-based fixup methods (bfs, bfs2bfs, etc.)\n");
    printf("2 ---> basicQuickMatch()        - runs QM and prints num unmatched\n");
    printf("3 ---> basicQuickMatchPersist() - runs QM and saves results to disk.  Records unmatched everytime, and right_sides only if we reach a new maxUnmatched.\n");
    printf("4 ---> basicHopcroft()        - runs hopcroft and prints num unmatched\n");
    printf("5 ---> hopcroftTest() - runs QM and tests all hermit fixup methods (bfs, bfs2bfs, etc.)\n");
    printf("6 ---> edgeColoring()\n");

    getInput("\nSelect a test: ", userInput, max);
    int selection = atoi(userInput);
    
    if (selection==1) {
        printf("\naltTest() selected\n");
        getInput("Filename: ", userInput, max);
        getInput("N: ", N, max);
        getInput("d: ", d, max);
        altTest(atoi(N),atoi(d),userInput);
    }    

    else if (selection==2) {
        printf("basicQuickMatch() selected\n");
        getInput("N: ", N, max);
        getInput("d: ", d, max);
        getInput("Forever? (y/n) ",userInput,max);
        printf("\n");
        if (userInput[0] == 'y') 
            basicLaurens(atoi(N),atoi(d),true);
        else if (userInput[0] == 'n')
            basicLaurens(atoi(N),atoi(d),false);
        else 
            printf("Didn't recognize input: %c\n\n",userInput[0]);
    }

    else if (selection==4) {
        printf("basicHopcroft() selected\n");
        getInput("N: ", N, max);
        getInput("d: ", d, max);
        getInput("Forever? (y/n) ",userInput,max);
        printf("\n");
        if (userInput[0] == 'y') 
            basicHopcroft(atoi(N),atoi(d),true);
        else if (userInput[0] == 'n')
            basicHopcroft(atoi(N),atoi(d),false);
        else 
            printf("Didn't recognize input: %c\n\n",userInput[0]);
    }

    else if (selection==5) {
        printf("\nhopcroftTest() selected\n");
        getInput("Filename: ", userInput, max);
        getInput("N: ", N, max);
        getInput("d: ", d, max);
        hopcroftTest(atoi(N),atoi(d),userInput);
    }    

    else if (selection==6) {
        printf("\nedgeColoring() selected\n");
        getInput("N: ", N, max);
        getInput("d: ", d, max);
        edgeColoring(atoi(N),atoi(d));
    }    

    else 
        printf("\nInvalid Selection\n");
    
    free(userInput);
    free(N);
    free(d);
}


