all: pass_init pass_mp pass_tbb pass_cilk

default: pass_init pass_mp pass_tbb pass_cilk

pass_mp:
	 icc -o openmp_pass_init openmp_pass_init.c -fopenmp -lcrypto
pass_tbb:
	icpc -o tbb_pass_init tbb_pass_init.cpp -ltbb -lcrypto
pass_cilk:
	icc -o cilk_pass_init cilk_pass_init.c -lcilkrts -lcrypto
pass_init: pass_init.c
	icc -std=c99 -o pass_init pass_init.c -lcrypto

clean:
	rm pass_init cilk_pass_init tbb_pass_init openmp_pass_init






