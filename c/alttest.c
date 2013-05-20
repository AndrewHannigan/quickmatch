#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "testing.h"


int main ( int argc, char *argv[] ) 
{ 
    assert(argc == 4);
    int N = atoi(argv[1]); 
    int d = atoi(argv[2]);
    altTest(N, d, argv[3]);
}    



