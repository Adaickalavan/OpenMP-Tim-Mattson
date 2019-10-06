
//**********************************************************
// Pseudo random number generator:
//     double random
//     void seed (lower_limit, higher_limit)
//**********************************************************
//
// A simple linear congruential random number generator
// (Numerical Recipies chapter 7, 1st ed.) with parameters
// from the table on page 198j.
//
//  Uses a linear congruential generator to return a value between
//  0 and 1, then scales and shifts it to fill the desired range.  This
//  range is set when the random number generator seed is called.
// 
// USAGE:
//
//      pseudo random sequence is seeded with a range
//
//            void seed(lower_limit, higher_limit)
//   
//      and then subsequent calls to the random number generator generates values
//      in the sequence:
//
//            double drandom()
//
// History: 
//      Written by Tim Mattson, 9/2007.
//      changed to drandom() to avoid collision with standard libraries, 11/2011

#include <omp.h>
#include <stdio.h>

static unsigned long long MULTIPLIER  = 764261123;
static unsigned long long PMOD        = 2147483647;
static unsigned long long mult_n;
double random_low, random_hi;

#define MAX_THREADS 128
static unsigned long long pseed[MAX_THREADS][4]; //[4] to padd to cache line
                                                 //size to avoid false sharing

unsigned long long random_last = 0;
#pragma omp threadprivate(random_last)

double drandom()
{
    unsigned long long random_next;
    double ret_val;

// 
// compute an integer random number from zero to mod
//
    random_next = (unsigned long long)((mult_n  * random_last)% PMOD);
    random_last = random_next;

//
// shift into preset range
//
    ret_val = ((double)random_next/(double)PMOD)*(random_hi-random_low)+random_low;
    return ret_val;
}
//
// set the seed and the range
//
void seed(double low_in, double hi_in)
{
    int id = omp_get_thread_num();

    #pragma omp single
    {
        if(low_in < hi_in)
        { 
            random_low = low_in;
            random_hi  = hi_in;
        }
        else
        {
            random_low = hi_in;
            random_hi  = low_in;
        }

        int nthreads = omp_get_num_threads();
        unsigned long long iseed = PMOD/MULTIPLIER;     // just pick a reasonable seed
        pseed[0][0] = iseed;
        mult_n = MULTIPLIER;
        for (int i = 1; i < nthreads; ++i)
        {
            iseed = (unsigned long long)((MULTIPLIER * iseed) % PMOD);
            pseed[i][0] = iseed;
            mult_n = (mult_n * MULTIPLIER) % PMOD;
        }
    }

   random_last = (unsigned long long) pseed[id][0];

}
//**********************************************************
// end of pseudo random generator code.
//**********************************************************

