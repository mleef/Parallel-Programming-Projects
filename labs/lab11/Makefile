default: all

all: badIndex

badIndex: badIndex.c
	gcc -g -o badIndex badIndex.c -fopenmp

clean:
	rm badIndex *.o 2>/dev/null
