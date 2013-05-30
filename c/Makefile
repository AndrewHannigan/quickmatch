CC=gcc

all: ntest dtest alttest

test: main.c testing.c quickmatch.c augmenting.c graph.c cycles.c color.c 
	$(CC) -o test.out main.c testing.c quickmatch.c augmenting.c graph.c cycles.c color.c -g

sand: sand.c quickmatch.c augmenting.c graph.c cycles.c color.c 
	$(CC) -o sand.out sand.c quickmatch.c augmenting.c graph.c cycles.c color.c -g

dtest: dtest.c testing.c quickmatch.c augmenting.c graph.c cycles.c color.c
	gcc -O3 -o ../scripts/dtest.out dtest.c testing.c quickmatch.c augmenting.c graph.c cycles.c color.c

ntest: ntest.c testing.c quickmatch.c augmenting.c graph.c cycles.c color.c
	gcc -O3 -o ../scripts/ntest.out ntest.c testing.c quickmatch.c augmenting.c graph.c cycles.c color.c

alttest: alttest.c testing.c quickmatch.c augmenting.c graph.c cycles.c color.c
	gcc -O3 -o ../scripts/alttest.out alttest.c testing.c quickmatch.c augmenting.c graph.c cycles.c color.c

clean: 
	rm -rf *.o; rm -f test; rm -f sand; rm -f ../scripts/*.out
