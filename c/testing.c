#define SEED time(NULL)

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>
#include "graph.h"
#include "laurens.h"
#include "timer.h"
#include "alternating.h"

#define FLUSH_TIME 30
/*
    Add hopcroft tests
        1) just hopcroft
        2) laurens until N^(2/3), then hopcroft, then bfs

    Add timing for all testing

    Keep all of this in the same csv.  Keep original columns the same so don't have to adjust python plotting code.
*/

int dTest(int N, int d_upper, int d_interval, char *file_name) {

/*

unmatched, d, N, p (ratio of N / (N-unmatched))

*/
    long last_flush = 0;
    long cur_time = 0;
    FILE *file; 
    file = fopen(file_name,"a+"); // append file (add text to a file or 
                                  // create a file if it does not exist.
    assert(file != NULL);

    srandom(SEED);
    int d;
    while (true) {
        for (d=3; d<=d_upper; d+=d_interval) {
            int *right_sides;
            int *matching;
            int unmatched = 0;
            int laurens_steps;
            struct Graph *graph;

            right_sides = createRightSides(N,d);
            graph = createRandomRegBipartite(N,d,0,right_sides);
            laurens_steps = laurens(graph,&matching,&unmatched);

            // Validate
            validateMatching(matching, graph);
            
            fprintf(file,"%i,%i,%i,%f\n", unmatched, d, N, ((float) N) / (N-unmatched));

            cur_time = (long) time(NULL);
            if (cur_time >= last_flush + FLUSH_TIME) {
                fflush(file);
                last_flush = cur_time;
            }

            // Free
            freeGraph(graph);
            free(matching);
            free(right_sides);
        }
    }
}



int NTest(int d, int N_lower, int N_upper, int N_interval, char *file_name) 
{
/*

unmatched, d, N, p (ratio of N / (N-unmatched))

*/
    long last_flush = 0;
    long cur_time = 0;
    FILE *file; 
    file = fopen(file_name,"a+"); // append file (add text to a file or 
                                  // create a file if it does not exist.
    assert(file != NULL);

    srandom(SEED);
    int N;
    while (true) {
        for (N=N_lower; N<=N_upper; N=N*N_interval) {
            int *right_sides;
            int *matching;
            int unmatched = 0;
            int laurens_steps;
            struct Graph *graph;

            right_sides = createRightSides(N,d);
            graph = createRandomRegBipartite(N,d,0,right_sides);
            laurens_steps = laurens(graph,&matching,&unmatched);

            // Validate
            validateMatching(matching, graph);
            
            fprintf(file,"%i,%i,%i,%f\n", unmatched, d, N, ((float) N) / (N-unmatched));

            cur_time = (long) time(NULL);
            if (time(NULL) >= last_flush + FLUSH_TIME) {
                fflush(file);
                last_flush = cur_time;
            }

            // Free
            freeGraph(graph);
            free(matching);
            free(right_sides);
        }
    }
}


int altTest(int N, int d, char *file_name) {
/*

N, d, unmatched, bfs_steps, dfs_steps, bfs2bfs_steps, dfs2bfs_steps, dfs2dfs_steps, hopcroft_steps, bfs_time, dfs_time, bfs2bfs_time, dfs2bfs_time, dfs2dfs_time, hopcroft_time

*/
    long last_flush = 0;
    long cur_time = 0;
    FILE *file; 
    file = fopen(file_name,"a+"); // append file (add text to a file or 
                                  // create a file if it does not exist.
    assert(file != NULL);

    //srandom(time(NULL));
    srandom(SEED);
    int laurens_steps;
    int bfs_steps;
    int dfs_steps;
    int bfs2bfs_steps;
    int dfs2bfs_steps;
    int dfs2dfs_steps;
    int hopcroft_steps;
    float laurens_time;
    float bfs_time;
    float dfs_time;
    float bfs2bfs_time;
    float dfs2bfs_time;
    float dfs2dfs_time;
    float hopcroft_time;
    int i;
    while (true) {
        int *right_sides;
        int *matching;
        int unmatched = 0;
        struct Graph *graph;

        right_sides = createRightSides(N,d);
        graph = createRandomRegBipartite(N,d,0,right_sides);
          START_TIMER
          laurens_steps = laurens(graph,&matching,&unmatched);
          STOP_TIMER
        laurens_time = seconds;
    

        // Copy a matching for each process
        int* matching1 = copyMatching(matching, N);
        int* matching2 = copyMatching(matching, N);
        int* matching3 = copyMatching(matching, N);
        int* matching4 = copyMatching(matching, N);
        int* matching5 = copyMatching(matching, N);
        int* matching6 = copyMatching(matching, N);


        // Perform
        resetGraph(graph);
      
        bfs_steps=0;
        dfs_steps=0;
        bfs2bfs_steps=0;
        dfs2bfs_steps=0;
        dfs2dfs_steps=0;
        hopcroft_steps=0;
        bfs_time=0;
        dfs_time=0;
        bfs2bfs_time=0;
        dfs2bfs_time=0;
        dfs2dfs_time=0;
        hopcroft_steps=0;

        int hopcroft_unmatched = unmatched;
        int temp, num_inserted;
        for (unmatched=unmatched; unmatched>0; unmatched--) {
            START_TIMER             
            bfs_steps = bfs(graph,matching1);
            STOP_TIMER
            bfs_time = seconds;

            START_TIMER
            dfs_steps = dfs(graph,matching5);
            STOP_TIMER
            dfs_time = seconds;

            START_TIMER
            bfs2bfs_steps = bfs2bfs(graph,matching2);
            STOP_TIMER
            bfs2bfs_time = seconds;

            START_TIMER
            dfs2bfs_steps = dfs2bfs(graph,matching3);
            STOP_TIMER
            dfs2bfs_time = seconds;

            START_TIMER
            dfs2dfs_steps = dfs2dfs(graph,matching4);
            STOP_TIMER
            dfs2dfs_time = seconds;

            // this logic accounts for the fact that sometimes hopcroftPhase inserts more than one edge into matching 
            if (hopcroft_unmatched == unmatched) {
                hopcroft_steps = 0;
                temp = hopcroft_unmatched;
                  START_TIMER
                  hopcroftPhase(graph, matching6, &hopcroft_unmatched, &hopcroft_steps);
                  STOP_TIMER
                num_inserted = temp - hopcroft_unmatched;
                //assert(hopcroft_unmatched == validateMatching(matching6, graph));
                //assert(temp>hopcroft_unmatched);
                hopcroft_steps = hopcroft_steps/num_inserted;
                hopcroft_time = seconds/num_inserted;
            }

            fprintf(file,"%i,%i,%i,%i,%i,%i,%i,%i,%i,%f,%f,%f,%f,%f,%f\n", N, d, unmatched, bfs_steps, dfs_steps, bfs2bfs_steps, dfs2bfs_steps, dfs2dfs_steps, hopcroft_steps, bfs_time, dfs_time, bfs2bfs_time, dfs2bfs_time, dfs2dfs_time, hopcroft_time);

        }

        // Validate
        validateMatching(matching1, graph);
        validateMatching(matching2, graph);
        validateMatching(matching3, graph);
        validateMatching(matching4, graph);
        validateMatching(matching5, graph);
        validateMatching(matching6, graph);

        assert(isMatchingComplete(graph, matching1));
        assert(isMatchingComplete(graph, matching2));
        assert(isMatchingComplete(graph, matching3));
        assert(isMatchingComplete(graph, matching4));
        assert(isMatchingComplete(graph, matching5));
        assert(isMatchingComplete(graph, matching6));
    
        float unmatchedFloat = (float) unmatched;
        
    
        cur_time = (long) time(NULL);
        if (time(NULL) >= last_flush + FLUSH_TIME) {
            fflush(file);
            last_flush = cur_time;
        }

        // Free
        freeGraph(graph);
        free(matching);
        free(matching1);
        free(matching2);
        free(matching3);
        free(matching4);
        free(matching5);
        free(matching6);
        free(right_sides);
        //free(visited);
        //free(targets);
    }
    fclose(file); 
}



int bigTest(int N, int d, char *file_name) {
/*

unmatched, laurens_steps, laurens_time, bfs_steps/unmatchedFloat, bfs2bfs_steps/unmatchedFloat, dfs2bfs_steps/unmatchedFloat, dfs2dfs_steps/unmatchedFloat, dfs_steps/unmatchedFloat, bfs_time, bfs_time, dfs_time, bfs2bfs_time, dfs2bfs_time, dfs2dfs_time

*/
    long last_flush = 0;
    long cur_time = 0;
    FILE *file; 
    file = fopen(file_name,"a+"); // append file (add text to a file or 
                                  // create a file if it does not exist.
    assert(file != NULL);

    //srandom(time(NULL));
    srandom(SEED);
    int laurens_steps;
    int bfs_steps;
    int dfs_steps;
    int bfs2bfs_steps;
    int dfs2bfs_steps;
    int dfs2dfs_steps;
    int hopcroft_steps;
    float laurens_time;
    float bfs_time;
    float dfs_time;
    float bfs2bfs_time;
    float dfs2bfs_time;
    float dfs2dfs_time;
    float hopcroft_time;
    int i;
    while (true) {
        int *right_sides;
        int *matching;
        int unmatched = 0;
        struct Graph *graph;

        right_sides = createRightSides(N,d);
        graph = createRandomRegBipartite(N,d,0,right_sides);
          START_TIMER
          laurens_steps = laurens(graph,&matching,&unmatched);
          STOP_TIMER
        laurens_time = seconds;
    

        // Copy a matching for each process
        int* matching1 = copyMatching(matching, N);
        int* matching2 = copyMatching(matching, N);
        int* matching3 = copyMatching(matching, N);
        int* matching4 = copyMatching(matching, N);
        int* matching5 = copyMatching(matching, N);
        int* matching6 = copyMatching(matching, N);


        // Perform
        resetGraph(graph);
      
        bfs_steps=0;
        dfs_steps=0;
        bfs2bfs_steps=0;
        dfs2bfs_steps=0;
        dfs2dfs_steps=0;
        hopcroft_steps=0;
        bfs_time=0;
        dfs_time=0;
        bfs2bfs_time=0;
        dfs2bfs_time=0;
        dfs2dfs_time=0;
        hopcroft_steps=0;

        for (i=0; i<unmatched; i++) {
            START_TIMER             
            bfs_steps += bfs(graph,matching1);
            STOP_TIMER
            bfs_time += seconds;

            START_TIMER
            dfs_steps += dfs(graph,matching5);
            STOP_TIMER
            dfs_time += seconds;

            START_TIMER
            bfs2bfs_steps += bfs2bfs(graph,matching2);
            STOP_TIMER
            bfs2bfs_time += seconds;

            START_TIMER
            dfs2bfs_steps += dfs2bfs(graph,matching3);
            STOP_TIMER
            dfs2bfs_time += seconds;

            START_TIMER
            dfs2dfs_steps += dfs2dfs(graph,matching4);
            STOP_TIMER
            dfs2dfs_time += seconds;

            START_TIMER
            hopcroft_steps += hopcroftPartial(graph,matching6);
            STOP_TIMER
            hopcroft_time += seconds;
        }

        // Validate
        validateMatching(matching1, graph);
        validateMatching(matching2, graph);
        validateMatching(matching3, graph);
        validateMatching(matching4, graph);
        validateMatching(matching5, graph);
        validateMatching(matching6, graph);

        assert(isMatchingComplete(graph, matching1));
        assert(isMatchingComplete(graph, matching2));
        assert(isMatchingComplete(graph, matching3));
        assert(isMatchingComplete(graph, matching4));
        assert(isMatchingComplete(graph, matching5));
        assert(isMatchingComplete(graph, matching6));
    
        float unmatchedFloat = (float) unmatched;
        
        fprintf(file,"%i,%i,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n", unmatched, laurens_steps, laurens_time, bfs_steps/unmatchedFloat, bfs2bfs_steps/unmatchedFloat, dfs2bfs_steps/unmatchedFloat, dfs2dfs_steps/unmatchedFloat, dfs_steps/unmatchedFloat, hopcroft_steps/unmatchedFloat, bfs_time, dfs_time, bfs2bfs_time, dfs2bfs_time, dfs2dfs_time, hopcroft_time);
    
        cur_time = (long) time(NULL);
        if (time(NULL) >= last_flush + FLUSH_TIME) {
            fflush(file);
            last_flush = cur_time;
        }

        // Free
        freeGraph(graph);
        free(matching);
        free(matching1);
        free(matching2);
        free(matching3);
        free(matching4);
        free(matching5);
        free(matching6);
        free(right_sides);
        //free(visited);
        //free(targets);
    }
    fclose(file); 
}


int hopcroftTest(int N, int d, char *file_name) {
/*
    hopcroft_steps, hopcroft_time, laurensHopcroft_steps, laurensHopcroft_time, laurensHopcroft_unmatched
*/
    FILE *file; 
    file = fopen(file_name,"a+"); // append file (add text to a file or 
                                  // create a file if it does not exist.
    assert(file != NULL);

    srandom(SEED);
    int hopcroft_steps=0;
    int laurensHopcroft_steps=0;

    float hopcroft_time=0.0;
    float laurensHopcroft_time=0.0;
    int i;
    while (true) {
        int *right_sides;
        int *matching1, *matching2;
        int unmatched = 0;
        struct Graph *graph;

        right_sides = createRightSides(N,d);
        graph = createRandomRegBipartite(N,d,1,right_sides);
    

        START_TIMER
        hopcroft_steps = hopcroft(graph, &matching1); 
        STOP_TIMER
        hopcroft_time = seconds;

        resetGraph(graph);

        START_TIMER
        laurensHopcroft_steps = laurens_hopcroft(graph, &matching2); 
        STOP_TIMER
        laurensHopcroft_time = seconds;


        // Validate
        assert(0 == validateMatching(matching1, graph));
        int laurensHopcroft_unmatched = validateMatching(matching2, graph);

        
        printf("printing to file\n");
        fprintf(file,"%i,%f,%i,%f,%i\n", hopcroft_steps, hopcroft_time, laurensHopcroft_steps, laurensHopcroft_time, laurensHopcroft_unmatched);        // Free
        fflush(file);
        freeGraph(graph);
        free(right_sides);
        free(matching1);
        free(matching2);
        //free(visited);
        //free(targets);
    }
    fclose(file); 
}


void basicLaurens(int N, int d, int runForever) {
    srandom(time(NULL));
    int *right_sides, *matching, *matchingRand, unmatchedNormal;
    struct Graph *graph;
    if (runForever) {
        while(true) {
            right_sides = createRightSides(N,d);
            graph = createRandomRegBipartite(N,d,0,right_sides);
              START_TIMER
              int steps = laurens(graph,&matching,&unmatchedNormal); 
              STOP_TIMER
            printf("Missed %i out of %i matches\n", unmatchedNormal, N);
            printf("%f seconds\n", seconds);
            printf("%i steps\n\n",steps);
            freeGraph(graph);
            free(right_sides);
            free(matching);
        }
    }
    else {
        right_sides = createRightSides(N,d);
        graph = createRandomRegBipartite(N,d,0,right_sides);
          START_TIMER
          int steps = laurens(graph,&matching,&unmatchedNormal); 
          STOP_TIMER
        printf("Missed %i out of %i matches\n", unmatchedNormal, N);
        printf("%f seconds\n", seconds);
        printf("%i steps\n\n",steps);
        free(matching);
        freeGraph(graph);
        free(right_sides);
    }
}


void basicHopcroft(int N, int d, int runForever) {
    srandom(time(NULL));
    int *right_sides, *matching, unmatched;
    struct Graph *originalGraph;

    if (runForever) {
        while (true) {
            right_sides = createRightSides(N,d);
            originalGraph = createRandomRegBipartite(N,d,false,right_sides);
            int steps = hopcroft(originalGraph, &matching);
            printf("unmatched: %i\n", validateMatching(matching, originalGraph));
            printf("steps: %i\n", steps);
            freeGraph(originalGraph);
            free(right_sides);
            free(matching);
        }
    }
    else {
        right_sides = createRightSides(N,d);
        originalGraph = createRandomRegBipartite(N,d,false,right_sides);
        START_TIMER
        int steps = hopcroft(originalGraph, &matching);
        STOP_TIMER
        printf("unmatched: %i\n", validateMatching(matching, originalGraph));
        printf("%f seconds\n", seconds);
        printf("%i steps\n\n",steps);
        freeGraph(originalGraph);
        free(right_sides);
        free(matching);
    }
}



int basicLaurensPersist(int N, int d, char *file_name, FILE *graphFile)
{
    FILE *file; 
    // append file (add text to a file or create a file if it does not exist
    if( access( file_name, F_OK ) == -1 ) {
        file = fopen(file_name,"a+");
        fprintf(file,"Number of missed matches by laurens method for N=%i, d=%i.\n",N,d);
    }
    else {
        file=fopen(file_name,"a+");
    }

    struct Graph *graph;
    int *right_sides, *matching, unmatched, maxUnmatched=0,i; 

    while (true) {
        right_sides = createRightSides(N,d);
        graph = createRandomRegBipartite(N,d,true,right_sides);
        laurens(graph,&matching,&unmatched);
        free(matching);
        if (unmatched > maxUnmatched) {
            fprintf(graphFile,"Laurens Method - N=%i, d=%i - %i missed matches.\n",N,d,unmatched);            
            fprintf(graphFile,"{%i", right_sides[0]);
            for (i=1; i<N*d; i++) { 
                fprintf(graphFile,",%i",right_sides[1]);
            }
            fprintf(graphFile,"}\n\n");
            maxUnmatched = unmatched;
        }

    }

    fclose(file); 
    return 0; 
}



char *getInput(char *prompt, char *userInput, int max)
{
    int i;
    for (i=0;i<max;i++) {
        userInput[i] = 0;
    }
    
    printf("%s", prompt);

    while (true) { // skip leading whitespace
        int c = getchar();
        if (c == EOF) break; // end of file
        if (!isspace(c)) {
             ungetc(c, stdin);
             break;
        }
    }

    i = 0;
    while (true) {
        int c = getchar();
        if (isspace(c) || c == EOF) { // at end, add terminating zero
            userInput[i] = 0;
            break;
        }
        userInput[i] = c;
        if (i==max-1) { // buffer full
            break;
        }
        i++;
    }

    return userInput;
}


