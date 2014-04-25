all: stencil_serial stencil_openmp stencil_cilk stencil_tbb

stencil_serial: stencil_serial.cpp
	icpc -std=c++11 -o stencil_serial stencil_serial.cpp -Wall -Wextra -lopencv_core -lopencv_highgui -lm

stencil_openmp: 
	icpc -std=c++11 -o omp_stencil stencil_omp.cpp -Wall -Wextra -lopencv_core -lopencv_highgui -lm -fopenmp 

stencil_cilk:
	icpc -std=c++11 -o cilk_stencil stencil_cilk.cpp -Wall -Wextra -lopencv_core -lopencv_highgui -lm -lcilkrts
	
stencil_tbb:
	icpc -std=c++11 -o tbb_stencil stencil_tbb.cpp -Wall -Wextra -lopencv_core -lopencv_highgui -lm -ltbb



clean:
	rm -f *.o stencil_serial omp_stencil cilk_stencil tbb_stencil
	
.PHONY: clean
