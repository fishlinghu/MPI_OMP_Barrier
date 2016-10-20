#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <vector>
#include <map>
#include <string>
#include <fstream>
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

void readfile(char *filename, vector<string> &str_vec)
    {
    string temp_str;
    int i;
    long int temp;

    ifstream file;
    file.open ( filename );

    while ( file >> temp_str )
        {
        if( temp_str[ temp_str.size()-1 ]>=33 && temp_str[ temp_str.size()-1 ]<=47 )
            temp_str.erase( temp_str.end()-1 );
        str_vec.push_back( temp_str );
        }

    /*i = 0;
    while(i < string_vec.size() )
        {   
        cout << string_vec[i] << ", ";
        ++i;
        }
    cout << endl;*/
    file.close();
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
        cout << "You need to specify # of OMP threads and file name as the program parameters. " << endl;
        cout << "Example command: mpirun -np 100 harness 20 input.txt" << endl;
        cout << "This means: run 100 MPI processes, with 20 OMP threads on each node. Input file name intput.txt " << endl;
        return 0;
        }

    int num_of_threads;
    
    num_of_threads = atoi( argv[1] );
    //filename = atoi( argv[3] );

    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

    vector<string> string_vec;
    readfile(argv[2], string_vec);

    // partition the workload among processes
    long int part_file_len = string_vec.size() / num_processes;
    long int start_idx = part_file_len * my_id;
    long int end_idx = start_idx + part_file_len - 1;
    if( my_id == num_processes - 1 )
        end_idx = string_vec.size() - 1;

    //cout << "NUMBER of str" << string_vec.size() << endl;
    // DS for storing the result
    vector< map<string, int> > vec_of_map;
    int i;
    i = 0;
    while( i < num_of_threads )
        {
        map<string, int> temp_map;
        vec_of_map.push_back( temp_map );
        ++i;
        }

    tree_DS_initialize(my_id, num_processes, parent_id, num_of_child, child_arr);

    // vector for checking the correctness of sync
    
    wake_msg[0] = -1;

    barrier* B;

    clock_gettime(CLOCK_REALTIME, &start_time);
    
    long int workload = (end_idx - start_idx + 1) / num_of_threads;

    B = new barrier( num_of_threads );
    //
    #pragma omp parallel num_threads( num_of_threads ) 
        {
        string temp_str;
        int thread_id = omp_get_thread_num();
        long int thread_start_idx = workload * thread_id;
        long int thread_end_idx = thread_start_idx + workload - 1;
        if( thread_id == (num_of_threads - 1) )
            thread_end_idx = end_idx;

        int j = thread_end_idx;
        while( j >= thread_start_idx )
            {
            temp_str = string_vec[j];
            //cout << temp_str << endl;
            if( vec_of_map[thread_id].count( temp_str ) == 0 )
                vec_of_map[thread_id][temp_str] = 1;
            else
                ++vec_of_map[thread_id][temp_str];
            --j;
            }

        omp_cbarrier(B, thread_id);
        }

    // Sum up the result from each thread
    map<string, int> sub_reduced_map;
    i = 0;
    while( i < num_of_threads )
        {
        for (map<string,int>::iterator it=vec_of_map[i].begin(); it!=vec_of_map[i].end(); ++it)
            {
            string temp_str = it->first;
            //cout << temp_str;
            if( sub_reduced_map.count( temp_str ) == 0 )
                sub_reduced_map[temp_str] = it->second;
            else
                sub_reduced_map[temp_str] += it->second;
            }
        ++i;
        }

    fstream fout;
    string filename = "temp";
    char id_char = my_id + 48;
    filename += id_char;

    fout.open(filename.c_str(), ios::out);
    if(!fout)
        {
        cout<<"Fail to open file: "<<filename<<endl;
        } 
    
    for (map<string,int>::iterator it=sub_reduced_map.begin(); it!=sub_reduced_map.end(); ++it)
        {
        //cout << it->first << "  " << it->second << endl;    
        fout << it->first << "  " << it->second << endl;
        }
    
    fout.close();

    tree_barrier(finish_msg, wake_msg, child_arr, my_id, parent_id, num_of_child);

    delete B;
    
    clock_gettime(CLOCK_REALTIME, &end_time);

    if(my_id == 0)
        {
        map<string, int> reduced_map;
        fstream fin;
        string word;
        int count;
        i = 0;
        while(i < num_processes)
            {
            filename = "temp";
            id_char = i + 48;
            filename += id_char;
            fin.open( filename.c_str() );
            while ( fin >> word )
                {
                fin >> count;
                if( reduced_map.count( word ) == 0 )
                    reduced_map[ word ] = count;
                else
                    reduced_map[ word ] += count;
                }
            fin.close();
            // delete file here
            remove( filename.c_str() );
            ++i;
            }
        // Output the total count
        filename = "output.txt";
        fout.open( filename.c_str(), ios::out);
        for (map<string,int>::iterator it=reduced_map.begin(); it!=reduced_map.end(); ++it)
            {
            fout << it->first << "  " << it->second << endl;
            }
        fout.close();
        }

    print_timer_message(start_time, end_time);

    MPI_Finalize();

    return 0;
    }

