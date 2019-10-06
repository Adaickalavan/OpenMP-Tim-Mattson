#include <iostream>
#include "helloworld.h"
#include "computepi.h"

int main(int argc, char* argv[]){
   
    const char* s = getenv("ENVVAR");
    printf("%s\n",s);

    // Slide 38 - Parallel 'Hello World' program
    helloWorld helloWorldInst;
    helloWorldInst.run(); 

    // Slide 48 - Compute Pi in Serial
    computepi computePiInst;
    computePiInst.runPiSerial(1e8); 

    // Slide 52 - Compute Pi in Parallel with false sharing
    computePiInst.runPiParallel(1e8, 1); 
    computePiInst.runPiParallel(1e8, 2); 
    computePiInst.runPiParallel(1e8, 3); 
    computePiInst.runPiParallel(1e8, 4); 

    // Slide 57 - Compute Pi in Parallel with padding
    computePiInst.runPiParallelPad(1e8, 1); 
    computePiInst.runPiParallelPad(1e8, 2); 
    computePiInst.runPiParallelPad(1e8, 3); 
    computePiInst.runPiParallelPad(1e8, 4); 

    // Slide 69 - Compute Pi in Parallel with synchronisation
    computePiInst.runPiParallelSync(1e8, 3); 

    // Slide 88 - Compute Pi with WorkSharing
    computePiInst.runPiWorkSharing(1e8); 

    return 0;
}
