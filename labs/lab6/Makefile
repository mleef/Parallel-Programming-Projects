all: crypto_serial crypto_mp crypto_tbb crypto_cilk

default: all

crypto_mp:
	 icc -o mp_crypto mp_crypto.c key.c -fopenmp 
	
crypto_tbb:
	 icpc -o tbb_crypto tbb_crypto.cpp key.c -ltbb 

crypto_cilk:
	 icc -o cilk_crypto cilk_crypto.c key.c -lcilkrts

crypto_serial: serial.c key.c
	icc -std=c99 -o crypto_serial serial.c key.c

clean:
	rm -f *.o crypto_serial mp_crypto tbb_crypto cilk_crypto decryptedOut encryptedOut
