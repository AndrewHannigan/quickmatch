#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "testing.h"


int main ( int argc, char *argv[] ) 
{ 
    assert(argc == 5);
    int N = atoi(argv[1]); 
    int d_upper = atoi(argv[2]);
    int d_interval = atoi(argv[3]);

    dTest(N, d_upper, d_interval, argv[4]);
}    



