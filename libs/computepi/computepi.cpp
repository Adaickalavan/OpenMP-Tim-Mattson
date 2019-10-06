#include <iostream>
#include <omp.h>
#include "computepi.h"

computepi::computepi(){
}

// Serial computation
double computepi::runPiSerial(int numSteps){
    double x, pi, sum = 0.0;
    double step = 1.0/double(numSteps);

    double startTime = omp_get_wtime();
    for (int ii = 0; ii < numSteps; ii++){
        x = (ii + 0.5)*step;
        sum = sum + 4.0/(1.0+x*x);
    }
    double totalTime = omp_get_wtime() - startTime;
    pi = step * sum;
    
    printf("computePi.runPiSerial(). Pi = %f. Time = %f sec\n", pi, totalTime);
    return pi;
}

// Use of array and cyclic for loop. But incurs false sharing due to shared cache line.
double computepi::runPiParallel(int numSteps, int nThreadsInput){
    double x;
    double step = 1.0/double(numSteps);
    double sum[nThreadsInput];
    double sumTotal;
    int nThreadsMaster;

    omp_set_num_threads(nThreadsInput);
    double startTime = omp_get_wtime();
    #pragma omp parallel
    {
        int id = omp_get_thread_num(); //Thread ID
        int nThreadsSlave = omp_get_num_threads();
        if (id == 0){
            nThreadsMaster = nThreadsSlave;
        }
        int ii;
        for (ii = id, sum[id]=0; ii < numSteps; ii=ii+nThreadsSlave){
            x = (ii + 0.5)*step;
            sum[id] = sum[id] + 4.0/(1.0+x*x);
        }
    }
    for (int ii = 0, pi=0.0; ii<nThreadsMaster; ii++){
        sumTotal = sumTotal + sum[ii];
    }
    double totalTime = omp_get_wtime() - startTime;
    double pi = sumTotal*step;
    
    printf("computePi.runPiParallel(). Pi = %f. Time = %f sec\n", pi, totalTime);
    return pi;
}

// Use of array and cyclic for loop. Array is padded to avoid false sharing.
double computepi::runPiParallelPad(int numSteps, int nThreadsInput){
    double x;
    int pad = 8;
    double step = 1.0/double(numSteps);
    double sum[nThreadsInput][pad];
    double sumTotal;
    int nThreadsMaster;

    omp_set_num_threads(nThreadsInput);
    double startTime = omp_get_wtime();
    #pragma omp parallel
    {
        int id = omp_get_thread_num(); //Thread ID
        int nThreadsSlave = omp_get_num_threads();
        if (id == 0){
            nThreadsMaster = nThreadsSlave;
        }
        int ii;
        for (ii = id, sum[id][0]=0; ii < numSteps; ii=ii+nThreadsSlave){
            x = (ii + 0.5)*step;
            sum[id][0] = sum[id][0] + 4.0/(1.0+x*x);
        }
    }
    for (int ii = 0, pi=0.0; ii<nThreadsMaster; ii++){
        sumTotal = sumTotal + sum[ii][0];
    }
    double totalTime = omp_get_wtime() - startTime;
    double pi = sumTotal*step;
    
    printf("computePi.runPiParallelPad(). Pi = %f. Time = %f sec\n", pi, totalTime);
    return pi;
}

// Use of cyclic for loop with `#pragma omp critical` for synchronisation
double computepi::runPiParallelSync(int numSteps, int nThreadsInput){
    double x;
    double step = 1.0/double(numSteps);
    double sumMaster = 0;
    int nThreadsMaster;

    omp_set_num_threads(nThreadsInput);
    double startTime = omp_get_wtime();
    #pragma omp parallel
    {
        int id = omp_get_thread_num(); //Thread ID
        int nThreadsSlave = omp_get_num_threads();
        if (id == 0){
            nThreadsMaster = nThreadsSlave;
        }
        int ii;
        double sumSlave = 0;
        for (ii = id; ii < numSteps; ii=ii+nThreadsSlave){
            x = (ii + 0.5)*step;
            sumSlave = sumSlave + 4.0/(1.0+x*x);
        }
        #pragma omp critical
        {
            sumMaster += sumSlave;
        }
    }

    double totalTime = omp_get_wtime() - startTime;
    double pi = sumMaster*step;
    
    printf("computePi.runPiParallelSync(). Pi = %f. Time = %f sec\n", pi, totalTime);
    return pi;
}

// Work sharing construct using `#pragma omp for reduction`
double computepi::runPiWorkSharing(int numSteps){
    double pi, sum = 0.0;
    double step = 1.0/double(numSteps);

    double startTime = omp_get_wtime();
    #pragma omp parallel 
    {
        double x;
        #pragma omp for reduction(+:sum)
        for (int ii = 0; ii < numSteps; ii++){
            x = (ii + 0.5)*step;
            sum = sum + 4.0/(1.0+x*x);
        }

    }
    double totalTime = omp_get_wtime() - startTime;
    pi = step * sum;
    
    printf("computePi.runPiWorkSharing(). Pi = %f. Time = %f sec\n", pi, totalTime);
    return pi;
}

double computepi::runPiTest(int numSteps){
    double pi, sum = 0.0;
    double step = 1.0/double(numSteps);

    double startTime = omp_get_wtime();
    #pragma omp parallel
    {
        double x;
        #pragma omp for lastprivate(sum) 
        for (int ii = 0; ii < numSteps; ii++){
            x = (ii + 0.5)*step;
            sum = sum + 4.0/(1.0+x*x);
        }
    }
    double totalTime = omp_get_wtime() - startTime;
    pi = step * sum;
    
    printf("computePi.runPiTest(). Pi = %f. Time = %f sec\n", pi, totalTime);
    return pi;
}