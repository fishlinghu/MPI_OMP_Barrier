#include "omp_barrier.h"

/////////////////////////////////////////////////////
// This constructor initializes the
// the barrir structure...
/////////////////////////////////////////////////////
barrier::barrier(int n) 
    {
    /*Initial Barrier Structure*/
    count = n;
    num_threads = n;
    sense = false;
    
    for (int i = 0; i < n; ++i)
        {
        mysense.push_back(true);
        }
    }

///////////////////////////////////////////////////////////
// omp_cbarrier(): This is barrier implementation
///////////////////////////////////////////////////////////
void omp_cbarrier(barrier *B, int index) 
    {
    // Code for P(i)
    int position = fetch_and_dec(&(B->count), -1);
    // #pragma omp atomic
    //  position = B->count--;

    if (position == 1)
        {
        B->count = B->num_threads;
        B->sense = B->mysense[index];
        } 
    else 
        {
        while(B->sense != B->mysense[index]);
        }
    B->mysense[index] = 1 - B->mysense[index];
    }
