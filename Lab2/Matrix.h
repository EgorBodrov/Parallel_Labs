#pragma once
#include <iostream>
#include <stdlib.h>

#define MIN_VALUE 0.0
#define MAX_VALUE 100.0
#define MAX_RAND 1000.0

double** get_matrix(int rows, int columns);
void print_matrix(double** matrix, int rows, int columns);
void clear_memory(double** matrix, int rows, int columns);
