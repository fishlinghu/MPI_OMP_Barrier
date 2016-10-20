
#include <iostream>
#include <omp.h>
#include <assert.h>
#include<sys/time.h>
#include "centralized_barrier.h"

using namespace std;

int main(int argc, char const *argv[])
{
	// Pointer to barrier object.
	struct timeval start, end;

	if(argc!=3)
  	{
        	exit(0);
  	}
	int num_thread = atoi(argv[1]);
	int barrier_count = atoi(argv[2]);

	barrier B;
	unsigned long x;
	x = 2;
	my_cbarrier_init(num_thread, &B);
	gettimeofday(&start, NULL);
	#pragma omp parallel num_threads(num_thread) shared(x)
	{
		// while (1) {
		// 	if (omp_get_thread_num() == 0)
		// 	{
		// 		x += 1;
		// 	} 
		// 	my_cbarrier(&B, omp_get_thread_num());
		// 	if (omp_get_thread_num() == 0)
		// 	{
		// 		/*Print 2*/
		// 		printf("2: Thread# %d: x = %ld\n", omp_get_thread_num(), x);
		// 	} else if (omp_get_thread_num() == 1) {
		// 		printf("3: Thread# %d: x = %ld\n", omp_get_thread_num(), x);			
		// 	} else if (omp_get_thread_num() == 2) {
		// 		printf("4: Thread# %d: x = %ld\n", omp_get_thread_num(), x);			
		// 	}else if (omp_get_thread_num() == 3) {
		// 		printf("5: Thread# %d: x = %ld\n", omp_get_thread_num(), x);			
		// 	}
			
		// 	if (x == 100) {
		// 		break;
		// 	}
		// my_cbarrier(&B, omp_get_thread_num());
		// }
		//////////////////////////Experimental/////////////////////////////////////////
		for (int i = 0; i < barrier_count; ++i)
		{
			// printf("thread# %d entering the barrier\n", omp_get_thread_num());
			if (omp_get_thread_num() == 0)
				x += 1;
			my_cbarrier(&B, omp_get_thread_num());
			// 	printf("barrier complete!!\n");
			// my_cbarrier(&B, omp_get_thread_num());
			// printf("thread# %d leaving the barrier\n", omp_get_thread_num());
		}
		printf( "Thread# %d: x = %ld\n", omp_get_thread_num(), x);
	}
	gettimeofday(&end, NULL);

	// printf("Exit of pragma\n");
	printf("Total Time (in micro-seconds) in executing the barrier-Centralized sense reversal:    %ld for Threads: %d, for barrier_count: %d\n", 
		((end.tv_sec * 1000000 + end.tv_usec)- (start.tv_sec * 1000000 + start.tv_usec)),
		num_thread, barrier_count);
	return 0;
}