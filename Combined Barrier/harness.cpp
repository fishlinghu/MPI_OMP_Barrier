#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <vector>
#include <omp.h>
#include <assert.h>
#include <unistd.h>
#include "mpi.h"
#include <sys/time.h>
#include <time.h>
#include "omp_barrier.h"
//#define NUM_OF_ROUND 10
//#define NUM_OF_THREAD 4

using namespace std;

void tree_DS_initialize(int my_id, int num_processes, int &parent_id, int &num_of_child, int child_arr[]);
void tree_barrier(int finish_msg[], int wake_msg[], int child_arr[], int my_id, int parent_id, int num_of_child);

struct timespec start_time;
struct timespec end_time;

void print_timer_message(timespec start_time, timespec end_time)
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
    
    int finish_msg[1];
    int wake_msg[1];
    //vector<int> child_vec;
    int child_arr[4];
    int num_of_child = 0;
    int parent_id;
    

    MPI_Init(&argc, &argv);

    if(argc != 3)
        {
        cout << "You need to specify # of OMP threads and # of rounds as the program parameters. " << endl;
        cout << "Example command: mpirun -np 100 harness 20 30" << endl;
        cout << "This means: run 100 MPI processes, with 20 OMP threads on each node, running 30 rounds. " << endl;
        return 0;
        }

    int num_of_threads, num_of_rounds;
    num_of_threads = atoi( argv[1] );
    num_of_rounds = atoi( argv[2] );

    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

    tree_DS_initialize(my_id, num_processes, parent_id, num_of_child, child_arr);

    // vector for checking the correctness of sync
    
    wake_msg[0] = -1;

    // Be careful of deadlock when using blocking sends and receives!
    //MPI_Send(&my_msg, 2, MPI_INT, my_dst, tag, MPI_COMM_WORLD);
    //MPI_Recv(&my_msg, 2, MPI_INT, my_src, tag, MPI_COMM_WORLD, &mpi_result);
    barrier* B;
    int round = 0;
    clock_gettime(CLOCK_REALTIME, &start_time);
    while( round < num_of_rounds )
    	{
    	//printf("proc %d enters round %d \n", my_id, round);
        // OpenMP should be put here
        
        B = new barrier( num_of_threads );
        //
        #pragma omp parallel num_threads( num_of_threads ) 
            {
            //check_vec[round * num_processes + my_id][ omp_get_thread_num() ] = 1;
            omp_cbarrier(B, omp_get_thread_num());
            //printf(" - OMP Thread %d in proc %d pass barrier in round %d\n", omp_get_thread_num(), my_id, round);
            }
        
        tree_barrier(finish_msg, wake_msg, child_arr, my_id, parent_id, num_of_child);
//
        delete B;
	    //usleep(1000000);
	   	++round;
	    }
    /*i = 0;
    while( i < num_of_rounds * num_processes )
        {
        int j = 0;
        while( j <  num_of_threads )
            {
            cout << check_vec[i][j];
            ++j;
            }
        cout << endl;
        ++i;
        }*/

    clock_gettime(CLOCK_REALTIME, &end_time);
    print_timer_message(start_time, end_time);

    MPI_Finalize();
    return 0;
    }

