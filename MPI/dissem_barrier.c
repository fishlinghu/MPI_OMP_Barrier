#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <omp.h>
#include <assert.h>
#include <unistd.h>
#include "mpi.h"
#include <sys/time.h>
#include <time.h>

struct timespec start_time;
struct timespec end_time;

void print_timer_message(struct timespec start_time, struct timespec end_time)
    {
    printf("\n***************** Total time *****************\n");
    printf("s_time.tv_sec:%ld, s_time.tv_nsec:%09ld\n", start_time.tv_sec, start_time.tv_nsec);
    printf("e_time.tv_sec:%ld, e_time.tv_nsec:%09ld\n", end_time.tv_sec, end_time.tv_nsec);
    if(end_time.tv_nsec > start_time.tv_nsec)
        {
        printf("[diff_time:%ld.%09ld sec]\n",
        end_time.tv_sec - start_time.tv_sec,
        end_time.tv_nsec - start_time.tv_nsec);
        }
    else
        {
        printf("[diff_time:%ld.%09ld sec]\n",
        end_time.tv_sec - start_time.tv_sec - 1,
        end_time.tv_nsec - start_time.tv_nsec + 1000*1000*1000);
        }
    return;
    }

int main(int argc, char **argv)
    {
    int my_id, my_dst, my_src, num_processes;
    int tag = 1;
    MPI_Status mpi_result;

    MPI_Init(&argc, &argv);

    if( argc != 3 )
        {
        printf("We need a program parameter to specify the length of MPI message and the # of rounds for testing the barrier. \n");
        printf("Example command: mpirun -np 100 tree_barrier 10 20. \n");
        printf("This means: run 100 MPI processes and run 10 rounds, the MPI message contains 20 integers. \n");
        return 0;
        }

    int num_of_round = atoi( argv[1] );
    int msg_len = atoi( argv[2] );

    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);


    int round = 0;
    int *finish_msg;

    finish_msg = malloc(sizeof(int) * msg_len);
    
    int i = msg_len - 1;
    while( i >= 0 )
        {
        finish_msg[i] = i;
        --i;
        }

    clock_gettime(CLOCK_REALTIME, &start_time);

    while( round < num_of_round )
    	{
    	//printf("proc %d enters round %d \n", my_id, round);

        int stride = 1;
        int semi_round;
        
        for( semi_round = num_processes - 1; semi_round > 0; semi_round = semi_round >> 1 )
            {
            //printf("%d\n", semi_round);
            my_dst = (my_id + stride) % num_processes;
            my_src = (my_id - stride + num_processes) % num_processes;
            MPI_Send(finish_msg, msg_len, MPI_INT, my_dst, tag, MPI_COMM_WORLD);
            MPI_Recv(finish_msg, msg_len, MPI_INT, my_src, tag, MPI_COMM_WORLD, &mpi_result);
            stride = stride >> 1;
            }
        
        //printf("proc %d \n", );
	    
	    //sleep(1);
	   	++round;
	    }
    
    clock_gettime(CLOCK_REALTIME, &end_time);
    print_timer_message(start_time, end_time);

    free(finish_msg);

    MPI_Finalize();
    return 0;
    }

