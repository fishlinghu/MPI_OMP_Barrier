
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <omp.h>
#include <assert.h>

struct barrier
{
	// a Fetch&Inc/Dec() object with initial 
	// value 'n' this object supports read
	// and write	
	int count;
	// Initially FALSE
	bool sense;
	// intially, psense[i]=TRUE
	// for each 1 <= i <= n
	bool mysense[8];
	// This contain number of threads...
	// only read this after initialization 
	// never modify this variable
	int num_threads;
};

template <class T>
inline T fetch_and_dec(T *ptr, T val) {
  // #ifdef GCC_EXTENSION
  // return __sync_fetch_and_add(ptr, val);
  // #endif
  // #ifdef OPENMP_3_1
  T t;
  // #pragma omp atomic capture
  #pragma omp critical
  { 
  	t = *ptr; 
  	*ptr += val; 
  }
  return t;
  // #endif
}
void my_cbarrier_init(int total_thread, barrier *B);
void my_cbarrier(barrier *B, int index);