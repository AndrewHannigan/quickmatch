CC=gcc

all: test


test: main.c testing.c laurens.c alternating.c graph.c cycles.c color.c 
	$(CC) -o test.out main.c testing.c laurens.c alternating.c graph.c cycles.c color.c -g

sand: sand.c laurens.c alternating.c graph.c cycles.c color.c 
	$(CC) -o sand.out sand.c laurens.c alternating.c graph.c cycles.c color.c -g

bigtest: bigtest.c testing.c laurens.c alternating.c graph.c cycles.c color.c
	gcc -O3 -o ../scripts/bigtest.out bigtest.c testing.c laurens.c alternating.c graph.c cycles.c color.c

dtest: dtest.c testing.c laurens.c alternating.c graph.c cycles.c color.c
	gcc -O3 -o ../scripts/dtest.out dtest.c testing.c laurens.c alternating.c graph.c cycles.c color.c
clean: 
	rm -rf *.o; rm -f test; rm -f sand
