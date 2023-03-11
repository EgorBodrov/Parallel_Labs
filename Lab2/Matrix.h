#pragma once
#include <iostream>
#include <stdlib.h>

#define MIN_VALUE 0.0
#define MAX_VALUE 100.0
#define MAX_RAND 1000.0

#define F_ROWS 3
#define F_COLUMNS 3
#define S_ROWS 3
#define S_COLUMNS 2
#define METHOD 1    // methods: 1 - by row algorithm, 2 - by columns, 3 - block

double* get_matrix(int rows, int cols);
void print_matrix(double* matrix, int rows, int cols);

void multiply_by_rows(int tasks, int rank);
