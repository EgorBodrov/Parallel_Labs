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
    //double arr[] = { 49.2264, 70.7022, 4.03455, 55.6475, 61.3483, 66.4876, 22.7515, 73.6351, 7.81579 };
    //double arr[] = { 51.4695, 51.0453, 70.3879, 78.6126, 43.8063, 62.7613, 59.9048, 63.6341, 45.4268, 35.6944 };
    for (int i = 0; i < 12; i++)
        matrix[i] = arr[i];

    return matrix;
}

double* return_second()
{
    double* matrix = (double*)malloc(3 * 2 * sizeof(double));
    double arr[] = { 52.7879, 1.57476, 18.3905, 96.7162, 70.5069, 87.6278 };
    //double arr[] = { 57.2375, 65.7064, 82.1711, 20.9815, 48.4848, 58.5253 };
    //double arr[] = { 64.1499, 12.4149, 22.541, 9.94293, 96.6735 };
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

void calculate_scatter_counts(int* sendcounts, int* displs, int tasks, int num_per_process, int vector_length)
{
    int sum = 0;
    int rem = num_per_process % tasks;

    for (int i = 0; i < tasks; i++)
    {
        sendcounts[i] = num_per_process / tasks;
        if (rem > 0)
        {
            sendcounts[i]++;
            rem--;
        }

        displs[i] = sum;
        sum += sendcounts[i] * vector_length;
    }

    for (int i = 0; i < tasks; i++)
        sendcounts[i] *= vector_length;
}

void calculate_gather_counts(int* sendcounts, int* displs, int tasks, int result_length, int vector_length)
{
    int sum = 0;

    for (int i = 0; i < tasks; i++) {
        sendcounts[i] = sendcounts[i] / vector_length * result_length;
        displs[i] = sum;
        sum += sendcounts[i];
    }
}

void transpose_matrix(double* matrix, int rows, int cols)
{
    double* transposed = (double*)malloc(rows * cols * sizeof(double));
    int indx = 0;

    for (int j = 0; j < cols; j++)
    {
        for (int i = 0; i < rows; i++)
        {
            transposed[indx++] = matrix[j + i * cols];
        }
    }

    memcpy(matrix, transposed, rows * cols * sizeof(double));
    free(transposed);
}

double* multiply_by_rows(double* f_matrix, double* s_matrix, int tasks, int rank)
{
    // Calculate amount of rows for each process
    int* sendcounts = (int*)malloc(tasks * sizeof(int));
    int* displs = (int*)malloc(tasks * sizeof(int));

    calculate_scatter_counts(sendcounts, displs, tasks, F_ROWS, F_COLUMNS);

    int n_rows = sendcounts[rank] / F_COLUMNS;
    double* result = NULL;
    double* f_buffer = (double*)malloc(sendcounts[rank] * sizeof(double));
    double* local_result = (double*)malloc(n_rows * S_COLUMNS * sizeof(double));

    if (rank == 0)
        result = (double*)malloc(F_ROWS * S_COLUMNS * sizeof(double));

    MPI_Barrier(MPI_COMM_WORLD);
    
    // Distribute by rows, Scatterv for cases when rows are undivisible by number of processes
    MPI_Scatterv(f_matrix, sendcounts, displs, MPI_DOUBLE, f_buffer, sendcounts[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Broadcast second matrix to all processes
    MPI_Bcast(s_matrix, S_ROWS * S_COLUMNS, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);

    double local_sum = 0;
    for (int i = 0; i < n_rows; i++)
    {
        for (int j = 0; j < S_COLUMNS; j++)
        {
            for (int k = 0; k < F_COLUMNS; k++)
                local_sum += f_buffer[i * F_COLUMNS + k] * s_matrix[k * S_COLUMNS + j];
            
            local_result[i * S_COLUMNS + j] = local_sum;
            local_sum = 0;
        }
    }

    calculate_gather_counts(sendcounts, displs, tasks, S_COLUMNS, F_COLUMNS);

    MPI_Gatherv(local_result, n_rows * S_COLUMNS, MPI_DOUBLE, result, sendcounts, displs, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);

    if (f_matrix)
        free(f_matrix);
    if (s_matrix)
        free(s_matrix);

    free(f_buffer);
    free(sendcounts);
    free(displs);
    free(local_result);

    return result;
}

double* multiply_by_columns(double* f_matrix, double* s_matrix, int tasks, int rank)
{
    // Calculate amount of rows for each process
    int* sendcounts = (int*)malloc(tasks * sizeof(int));
    int* displs = (int*)malloc(tasks * sizeof(int));

    calculate_scatter_counts(sendcounts, displs, tasks, F_COLUMNS, F_ROWS);

    /*if (rank == 0) {
        for (int i = 0; i < tasks; i++)
            printf("sendcounts[%d] = %d\tdispls[%d] = %d\n", i, sendcounts[i], i, displs[i]);
    }*/

    int n_cols = sendcounts[rank] / F_ROWS;
    double* result = NULL;
    double* f_buffer = (double*)malloc(sendcounts[rank] * sizeof(double));
    double* local_result = (double*)calloc(n_cols * F_ROWS * S_COLUMNS, sizeof(double));

    if (rank == 0)
    {
        result = (double*)malloc(F_ROWS * S_COLUMNS * sizeof(double));
        transpose_matrix(f_matrix, F_ROWS, F_COLUMNS);
    }
    MPI_Barrier(MPI_COMM_WORLD);

    // Distribute by columns, Scatterv for cases when rows are undivisible by number of processes
    MPI_Scatterv(f_matrix, sendcounts, displs, MPI_DOUBLE, f_buffer, sendcounts[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Broadcast second matrix to all processes
    MPI_Bcast(s_matrix, S_ROWS * S_COLUMNS, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    int local_sum = 0;
    int ind = displs[rank] / F_ROWS;
    for (int i = 0; i < n_cols; i++)
    {
        for (int j = 0; j < F_ROWS; j++)
        {
            for (int k = 0; k < S_COLUMNS; k++)
            {
                local_result[j * S_COLUMNS + k] += f_buffer[i * F_ROWS + j] * s_matrix[ind * S_COLUMNS + k];
            }
        }

        ind++;
    }

    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Reduce(local_result, result, F_ROWS * S_COLUMNS, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (f_matrix)
        free(f_matrix);
    if (s_matrix)
        free(s_matrix);

    free(f_buffer);
    free(sendcounts);
    free(displs);
    free(local_result);

    return result;
}
