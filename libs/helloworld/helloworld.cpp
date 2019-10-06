#include <iostream>
#include <omp.h>
#include "helloworld.h"

helloWorld::helloWorld(){
}

void helloWorld::run(){
    #pragma omp parallel
    {
        int ID = omp_get_thread_num();
        printf("hello(%d)",ID);
        printf("world(%d)\n",ID);
    }    
}