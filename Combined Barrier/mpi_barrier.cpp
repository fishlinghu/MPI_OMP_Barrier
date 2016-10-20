#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <vector>
#include <omp.h>
#include <assert.h>
#include <unistd.h>
#include "mpi.h"


void tree_DS_initialize(int my_id, int num_processes, int &parent_id, int &num_of_child, int child_arr[])
    {
    int child_id;
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
    return;
    }


void tree_barrier(int finish_msg[], int wake_msg[], int child_arr[], int my_id, int parent_id, int num_of_child)
    {
    MPI_Status mpi_result;
    int tag = 1;
    finish_msg[0] = my_id;

    // printf("Id: %d, %d\n", my_id, num_of_child);
    int i = num_of_child - 1;
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
    }