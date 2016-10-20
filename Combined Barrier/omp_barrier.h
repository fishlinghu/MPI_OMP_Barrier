#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <omp.h>
#include <assert.h>

using namespace std;

template <class T>
inline T fetch_and_dec(T *ptr, T val) 
    {
    // #ifdef GCC_EXTENSION
    // return __sync_fetch_and_add(ptr, val);
    // #endif
    // #ifdef OPENMP_3_1
    T t;
    #pragma omp critical
        { t = *ptr; *ptr += val; }
    return t;
    // #endif
    }

class barrier
    {
    public:
        barrier(int n);
        // a Fetch&Inc/Dec() object with initial 
        // value 'n' this object supports read
        // and write    
        int count;
        // Initially FALSE
        bool sense;
        // intially, psense[i]=TRUE
        // for each 1 <= i <= n
        //bool mysense[4];
        vector<bool> mysense;
        // This contain number of threads...
        // only read this after initialization 
        // never modify this variable
        int num_threads;
    };

//barrier::barrier(int n);
void omp_cbarrier(barrier *B, int index);