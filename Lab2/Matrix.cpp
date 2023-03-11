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
            matrix[row * rows + col] = MIN_VALUE + value * (MAX_VALUE - MIN_VALUE);
        }
    }

    return matrix;
}

void print_matrix(double* matrix, int rows, int cols)
{
    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {
            std::cout << matrix[row * rows + col] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void multiply_by_rows(int tasks, int rank)
{
    double* f_matrix = NULL;
    double* s_matrix = NULL;

    int rows_per_process = F_ROWS / tasks;
    double* f_buffer = (double*)malloc(rows_per_process * F_COLUMNS);

    if (rank == 0)
    {
        f_matrix = get_matrix(F_ROWS, F_COLUMNS);

        print_matrix(f_matrix, F_ROWS, F_COLUMNS);

        MPI_Scatter(f_matrix, rows_per_process * F_COLUMNS, MPI_DOUBLE, f_buffer, rows_per_process * F_COLUMNS, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        //MPI_Bcast(s_matrix, S_ROWS * S_COLUMNS, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        free(f_matrix);
    }
    else {
        MPI_Scatter(f_matrix, rows_per_process * F_COLUMNS, MPI_DOUBLE, f_buffer, rows_per_process * F_COLUMNS, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        //MPI_Bcast(s_matrix, S_ROWS * S_COLUMNS, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    std::cout << "HELLO, I AM " << rank << " process.\n";
    for (int i = 0; i < rows_per_process * F_COLUMNS; i++)
        std::cout << f_buffer[i] << " ";


    /*free(f_matrix);
    free(s_matrix);
    free(f_buffer);
    free(s_buffer);*/
}

//void multiply_by_rows(int tasks, int rank)
//{
//    double** m_1 {nullptr};
//    double** m_2{ nullptr };
//    double** result{ nullptr };
//
//    if (rank == 0)
//    {
//        m_1 = get_matrix(F_ROWS, F_COLUMNS);
//        m_2 = get_matrix(S_ROWS, S_COLUMNS);
//        result = get_matrix(F_ROWS, S_COLUMNS, true);
//
//        print_matrix(m_1, F_ROWS, F_COLUMNS);
//        print_matrix(m_2, S_ROWS, S_COLUMNS);
//
//        
//    }
//    
//    int sum{ 0 };
//    int remaining_elements{ (F_ROWS * F_COLUMNS) % tasks };
//    int* sendcounts = (int*)malloc(sizeof(int) * tasks);
//    int* displs = (int*)malloc(sizeof(int) * tasks);
//
//    for (int i = 0; i < tasks; i++) {
//        sendcounts[i] = (F_ROWS * F_COLUMNS) / tasks;
//        if (remaining_elements > 0) {
//            sendcounts[i]++;
//            remaining_elements--;
//        }
//
//        displs[i] = sum;
//        sum += sendcounts[i];
//    }
//
//    int count{ sendcounts[rank] };
//    double* recv = (double*)malloc(sizeof(double) * count);
//    MPI_Scatterv(&(m_1[0][0]), sendcounts, displs, MPI_DOUBLE, recv, count, MPI_DOUBLE, 0, MPI_COMM_WORLD);
//    
//    printf("%d: ", rank);
//    for (int i = 0; i < count; i++) {
//        std::cout << recv[i] << " ";
//    }
//    printf("\n");
//
//    MPI_Barrier(MPI_COMM_WORLD);
//    if (rank == 0)
//    {
//        clear_memory(m_1, F_ROWS, F_COLUMNS);
//        clear_memory(m_2, S_ROWS, S_COLUMNS);
//        clear_memory(result, F_ROWS, S_COLUMNS);
//
//        std::cout << "Cleared" << std::endl;
//    }
//
//}
