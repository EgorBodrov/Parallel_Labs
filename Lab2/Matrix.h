#pragma once
#include <iostream>
#include <stdlib.h>

#define MIN_VALUE 0.0
#define MAX_VALUE 100.0
#define MAX_RAND 1000.0

#define F_ROWS 4
#define F_COLUMNS 3
#define S_ROWS 3
#define S_COLUMNS 2
#define METHOD 1    // methods: 1 - by row algorithm, 2 - by columns, 3 - block

double* get_matrix(int rows, int cols);
void print_matrix(double* matrix, int rows, int cols);

double* multiply_by_rows(double* f_matrix, double* s_matrix, int tasks, int rank);
void multiply_by_columns(int tasks, int rank);

double* return_first();
double* return_second();

// 3x3 * 3x2 //
//first
//49.2264 70.7022 4.03455
//55.6475 61.3483 66.4876
//22.7515 73.6351 7.81579
//second
//57.2375 65.7064
//82.1711 20.9815
//48.4848 58.5253
//answer
//8822.88 4954.04
//11449.8 8834.78
//7731.86 3497.32

// 2x5 * 5x1
//first
//51.4695 51.0453 70.3879 78.6126 43.8063
//62.7613 59.9048 63.6341 45.4268 35.6944
//second
//64.1499
//12.4149
//22.541
//9.94293
//96.6735
//answer
//10538.6
//10106.6