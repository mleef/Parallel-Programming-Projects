default: matmul_serial

all: matmul_serial matmul_mp matmul_cilk matmul_tbb

matmul_serial: matmul.c
	icc -o matmul_serial matmul.c -fopenmp
	icc -o matmul_improved improved_matmul.c -fopenmp

matmul_mp:

matmul_cilk:
	icc -o matmul_cilk cilk_matmul.c -lcilkrts -fopenmp
matmul_tbb:

clean:
	rm matmul_serial matmul_improved 2>/dev/null
