#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <stdio.h>
#include <stdlib.h>

void generate_file(int N, const char* filename);
int** read_file(int N, const char* filename);

#endif
