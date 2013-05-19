#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "testing.h"


int main ( int argc, char *argv[] ) 
{ 
    assert(argc == 6);
    int d = atoi(argv[1]); 
    int N_lower = atoi(argv[2]);
    int N_upper = atoi(argv[3]);
    int N_interval = atoi(argv[4]);

    NTest(d, N_lower, N_upper, N_interval, argv[5]);
}    



