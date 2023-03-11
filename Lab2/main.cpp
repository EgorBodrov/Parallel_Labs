#include <cstdio>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <mpi.h>

#include "Matrix.h"

int main(int* argc, char** argv)
{
    int tasks, rank;

    MPI_Init(argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &tasks);
    
    srand(time(NULL));

    switch (METHOD)
    {
    case 1: multiply_by_rows(tasks, rank); break;
    case 2: break;
    case 3: break;
    }

    MPI_Finalize();

    return 0;
}
