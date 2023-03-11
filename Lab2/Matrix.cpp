#include "Matrix.h"
#include "mpi.h"

double** get_matrix(int rows, int columns)
{
    double** matrix = new double*[rows];
    for (int i = 0; i < rows; i++)
        matrix[i] = new double[columns];
    
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < columns; col++) {
            double value = (double)rand() / RAND_MAX;
            matrix[row][col] = MIN_VALUE + value * (MAX_VALUE - MIN_VALUE);
        }
    }

    return matrix;
}

void print_matrix(double** matrix, int rows, int columns)
{
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < columns; col++) {
            std::cout << matrix[row][col] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void clear_memory(double** matrix, int rows, int columns)
{
    for (int i = 0; i < rows; i++)
        delete[] matrix[i];

    delete[] matrix;
}
