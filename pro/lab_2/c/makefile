source: source.o algo.o inout.o
	gcc source.o algo.o inout.o -o main

source.o: source.c
	gcc -c source.c

algo.o: algo.c algo.h struct.h
	gcc -c algo.c

inout.o: inout.c inout.h struct.h
	gcc -c inout.c

clean:
	rm -rf *.o main