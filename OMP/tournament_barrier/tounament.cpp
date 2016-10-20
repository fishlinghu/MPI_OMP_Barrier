#include "tounament.h"

int main(int argc, char **argv)
{
	bool x = false;

	if(argc!=3)
  	{
        	exit(0);
  	}
	struct timeval start, end;
	int thread_count = atoi(argv[1]);
	int barrier_count = atoi(argv[2]);
	int num_rounds = ceil( log(thread_count)/log(2) );
	for(int i=0;i<thread_count;i++)
	{
		for(int k=0;k<=num_rounds;k++)
		{
			array[i][k].flag = false;
			array[i][k].role = -1;
			array[i][k].opponent = &x;
		}
	}
	int second_check, first_check=0;

	for(int i=0 ; i<thread_count; i++)
	{
		for(int k=0;k<=num_rounds;k++)
		{
			second_check = ceil( pow(2,k) );
			first_check = ceil( pow(2,k-1) );

			if((k > 0) && (i%second_check==0) && ((i + (first_check))< thread_count) && (second_check < thread_count))
			{
				array[i][k].role = winner;
			}

			if((k > 0) && (i%second_check == 0) && ((i + first_check)) >= thread_count)
			{
				array[i][k].role = bye;
			}

			if((k > 0) && ((i%second_check == first_check)))
			{
				array[i][k].role = loser;
			}

			if((k > 0) && (i==0) && (second_check >= thread_count))
			{
				array[i][k].role = champion;
			}

			if(k==0)
			{
				array[i][k].role = dropout;
			}
			if(array[i][k].role == loser)
			{
				array[i][k].opponent = &array[i-first_check][k].flag;
			}
			if(array[i][k].role == winner || array[i][k].role == champion)
			{
				array[i][k].opponent = &array[i+first_check][k].flag;
			}
		}
	}
	unsigned long y;
	y = 2;
	gettimeofday(&start, NULL);
	#pragma omp parallel num_threads(thread_count) shared(array, y)
	{
		int thread_num=0;
		bool *sense;
		bool temp = true;
		#pragma omp critical
		{
			thread_num = omp_get_thread_num();
			sense = &temp;
		}
		for( int i = 0; i < barrier_count; i++ )
		{
			if (omp_get_thread_num() == 0)
				y += 1;
			tournament_barrier(thread_num,sense,num_rounds);
	    }
	printf( "Thread# %d: x = %ld\n", omp_get_thread_num(), y);
	}
	gettimeofday(&end, NULL);
	printf("Total Time (in micro-seconds) in executing the barrier-Tournament barrier:    %ld for Threads: %d, for barrier_count: %d\n", 
		((end.tv_sec * 1000000 + end.tv_usec)- (start.tv_sec * 1000000 + start.tv_usec)),
		thread_count, barrier_count);
	return 0;
}