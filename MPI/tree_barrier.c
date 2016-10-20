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
    int finish_msg[1];
    int wake_msg[1];
    //vector<int> child_vec;
    int child_arr[4];
    int num_of_child = 0;
    int parent_id, child_id;
    MPI_Status mpi_result;

    MPI_Init(&argc, &argv);

    if( argc != 2 )
        {
        printf("We need a program parameter to specify the # of rounds for testing the barrier. \n");
        printf("Example command: mpirun -np 100 tree_barrier 10. \n");
        printf("This means: run 100 MPI processes and run 10 rounds. \n");
        return 0;
        }

    int num_of_round = atoi( argv[1] );

    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

    /* Figure out who is the parent and children of a process */
    if(my_id != 0)
        {
        parent_id = (my_id - 1) / 4;
        }   
    
    int i = 1;
    while(i <= 4)
        {
        child_id = ( my_id << 2 ) + i;
        if( child_id < num_processes )
            {
            ++num_of_child;
            child_arr[i-1] = child_id;
            }
        ++i;
        }

    my_dst = (my_id + 1) % num_processes;
    my_src = (my_id - 1) % num_processes;
    while (my_src < 0)
        my_src += num_processes;

    
    wake_msg[0] = -1;
    finish_msg[0] = my_id;
    //my_msg[1] = num_processes;

    // Be careful of deadlock when using blocking sends and receives!
    //MPI_Send(&my_msg, 2, MPI_INT, my_dst, tag, MPI_COMM_WORLD);
    //MPI_Recv(&my_msg, 2, MPI_INT, my_src, tag, MPI_COMM_WORLD, &mpi_result);
    int round = 0;
    clock_gettime(CLOCK_REALTIME, &start_time);
    while( round < num_of_round )
    	{
    	//printf("proc %d enters round %d \n", my_id, round);
	    //printf("Id: %d, %d\n", my_id, num_of_child);
	    i = num_of_child - 1;
	    while(i >= 0)
	        {
	        MPI_Recv( &finish_msg, 1, MPI_INT, child_arr[i], tag, MPI_COMM_WORLD, &mpi_result);
	        // printf("proc %d: received message from proc %d of %d\n", my_id, finish_msg[0], num_processes);
	        --i;
	        }

	    if(my_id != 0)
	        MPI_Send(&finish_msg, 1, MPI_INT, parent_id, tag, MPI_COMM_WORLD);

	    //printf("proc %d: received message from proc %d of %d\n", my_id, my_msg[0], my_msg[1]);
	    if( my_id != 0 )
	    	{
	    	// Recive wake up message from parent
	    	MPI_Recv( &wake_msg, 1, MPI_INT, parent_id, tag, MPI_COMM_WORLD, &mpi_result);
	    	// printf("proc %d waked up by proc %d\n", my_id, parent_id);
	    	}

	    
	    i = num_of_child - 1;
	    while( i >= 0 )
	    	{
	    	MPI_Send( &wake_msg, 1, MPI_INT, child_arr[i], tag, MPI_COMM_WORLD);
	    	--i;
	    	}
	    
        //sleep(1);
	   	++round;
	    }
    clock_gettime(CLOCK_REALTIME, &end_time);
    print_timer_message(start_time, end_time);
    
    MPI_Finalize();
    return 0;
    }

