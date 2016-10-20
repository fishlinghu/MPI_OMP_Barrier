#include "centralized_barrier.h"

/////////////////////////////////////////////////////
// my_cbarrier_init(): This function initializes the
// the barrir structure...
/////////////////////////////////////////////////////
void my_cbarrier_init(int total_thread, barrier *B) {
	/*Initial Barrier Structure*/
	B->count = total_thread;
	B->num_threads = total_thread;
	B->sense = false;
	for (int i = 0; i < total_thread; ++i)
	{
		B->mysense[i] = true;
	}
}

///////////////////////////////////////////////////////////
// my_cbarrier(): This is barrier implementation
///////////////////////////////////////////////////////////
void my_cbarrier(barrier *B, int index) {
	// Code for P(i)
	int position = fetch_and_dec(&(B->count), -1);
	// #pragma omp atomic
	// 	position = B->count--;

	if (position == 1)
	{
		B->count = B->num_threads;
		B->sense = B->mysense[index];
	} else {
		while(B->sense != B->mysense[index]);
	}
	B->mysense[index] = 1 - B->mysense[index];
}