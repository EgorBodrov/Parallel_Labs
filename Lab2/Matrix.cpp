#include "Matrix.h"
#include "mpi.h"

double* get_matrix(int rows, int cols)
{   
    double* matrix = (double*)malloc(rows * cols * sizeof(double));

    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {
            double value = (double)rand() / RAND_MAX;
            matrix[row * cols + col] = MIN_VALUE + value * (MAX_VALUE - MIN_VALUE);
        }
    }

    return matrix;
}

// Answer
//3833.25 6816.4
//6442.79 13780.5
//12527.7 17121
//6861.43 5452.58
double* return_first()
{
    double* matrix = (double*)malloc(4 * 3 * sizeof(double));
    double arr[] = { 8.04773, 34.7636, 39.2743, 20.5481, 95.9777, 50.9598, 75.1732, 86.1721, 98.9227, 83.5322, 30.784, 26.7464 };

    for (int i = 0; i < 12; i++)
        matrix[i] = arr[i];

    return matrix;
}

double* return_second()
{
    double* matrix = (double*)malloc(3 * 2 * sizeof(double));
    double arr[] = { 52.7879, 1.57476, 18.3905, 96.7162, 70.5069, 87.6278 };

    for (int i = 0; i < 6; i++)
        matrix[i] = arr[i];

    return matrix;
}

void print_matrix(double* matrix, int rows, int cols)
{
    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {
            std::cout << matrix[row * cols + col] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void multiply_by_rows(int tasks, int rank)
{
    // Calculate amount of rows for each process
    int* sendcounts = (int*)malloc(tasks * sizeof(int));
    int* displs = (int*)malloc(tasks * sizeof(int));
    int sum = 0;
    int rem = F_ROWS % tasks;

    for (int i = 0; i < tasks; i++) 
    {
        sendcounts[i] = F_ROWS / tasks;
        if (rem > 0) 
        {
            sendcounts[i]++;
            rem--;
        }

        displs[i] = sum;
        sum += sendcounts[i] * F_COLUMNS;
    }
    for (int i = 0; i < tasks; i++)
        sendcounts[i] *= F_COLUMNS;

    int n_rows = sendcounts[rank] / F_COLUMNS;
    double* f_matrix = NULL;
    double* s_matrix = NULL;
    double* result = NULL;
    double* f_buffer = (double*)malloc(sendcounts[rank] * sizeof(double));
    double* local_result = (double*)malloc(n_rows * S_COLUMNS * sizeof(double));

    if (rank == 0)
    {
        f_matrix = get_matrix(F_ROWS, F_COLUMNS);
        s_matrix = get_matrix(S_ROWS, S_COLUMNS);
        //f_matrix = return_first();
        //s_matrix = return_second();
        result = (double*)malloc(F_ROWS * S_COLUMNS * sizeof(double));
        
        std::cout << "First matrix (vector)" << std::endl;
        print_matrix(f_matrix, F_ROWS, F_COLUMNS);
        std::cout << "Second matrix (vector)" << std::endl;
        print_matrix(s_matrix, S_ROWS, S_COLUMNS);
    }
    else 
    {
        s_matrix = (double*)malloc(S_ROWS * S_COLUMNS * sizeof(double));
    }
    MPI_Barrier(MPI_COMM_WORLD);

    // Distribute by rows, Scatterv for cases when rows are undivisible by number of processes
    MPI_Scatterv(f_matrix, sendcounts, displs, MPI_DOUBLE, f_buffer, sendcounts[rank] * F_COLUMNS, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Broadcast second matrix to all processes
    MPI_Bcast(s_matrix, S_ROWS * S_COLUMNS, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);

    double local_sum = 0;
    for (int i = 0; i < n_rows; i++)
    {
        for (int j = 0; j < S_COLUMNS; j++)
        {
            for (int k = 0; k < F_COLUMNS; k++)
            {
                local_sum += f_buffer[i * F_COLUMNS + k] * s_matrix[k * S_COLUMNS + j];
            }

            local_result[i * S_COLUMNS + j] = local_sum;
            local_sum = 0;
        }
    }

    // Calculations for Gatherv
    sum = 0;
    for (int i = 0; i < tasks; i++) {
        sendcounts[i] = sendcounts[i] / F_COLUMNS * S_COLUMNS;
        displs[i] = sum;
        sum += sendcounts[i];
    }

    MPI_Gatherv(local_result, n_rows * S_COLUMNS, MPI_DOUBLE, result, sendcounts, displs, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0)
    {
        std::cout << "Result matrix (vector)" << std::endl;
        print_matrix(result, F_ROWS, S_COLUMNS);
    }

    if (f_matrix)
        free(f_matrix);
    if (s_matrix)
        free(s_matrix);
    if (result)
        free(result);

    free(f_buffer);
    free(sendcounts);
    free(displs);
    free(local_result);
}
