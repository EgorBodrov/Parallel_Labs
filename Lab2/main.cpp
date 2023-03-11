#include <cstdio>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <mpi.h>

#include "Matrix.h"

#define F_ROWS 3
#define F_COLUMNS 3
#define S_ROWS 2
#define S_COLUMNS 2

int main(int* argc, char** argv)
{
    int tasks, rank;
    srand(time(NULL));

    double** f_matrix = get_matrix(F_ROWS, F_COLUMNS);
    double** s_matrix = get_matrix(S_ROWS, S_COLUMNS);

    print_matrix(f_matrix, F_ROWS, F_COLUMNS);
    print_matrix(s_matrix, S_ROWS, S_COLUMNS);

    MPI_Init(argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &tasks);

    

    MPI_Finalize();

    clear_memory(f_matrix, F_ROWS, F_COLUMNS);
    clear_memory(s_matrix, S_ROWS, S_COLUMNS);

    return 0;
}
