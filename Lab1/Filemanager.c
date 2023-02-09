#define _CRT_SECURE_NO_WARNINGS
#include "Filemanager.h"

#define MASS 1000       // mass
#define X_RADIUS 100    // x for radius vector
#define Y_RADIUS 100    // y for radius vector
#define X_VELOCITY 50   // x for velocity vector
#define Y_VELOCITY 50   // y for velocity vector


void generate_file(int N, const char* filename)
{
    FILE* fp = fopen(filename, "w");

    for (int i = 0; i < N; i++)
    {
        fprintf(fp, "%d %d %d %d %d\n",
            rand() % MASS + 1,

            rand() % X_RADIUS + 1,
            rand() % Y_RADIUS + 1,

            rand() % X_VELOCITY + 1,
            rand() % Y_VELOCITY + 1
        );
    }

    fclose(fp);
}

int** read_file(int N, const char* filename)
{
    FILE* fp = fopen(filename, "r");

    char buffer[1000];
    int** arr = (int**)malloc(N * sizeof(int*));
    for (int i = 0; i < N; i++)
        arr[i] = (int*)malloc(5 * sizeof(int));

    int ind = 0;
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        sscanf(buffer, "%d %d %d %d %d",
            &(arr[ind][0]),
            &(arr[ind][1]),
            &(arr[ind][2]),
            &(arr[ind][3]),
            &(arr[ind][4])
        );

        ind++;
    }

    fclose(fp);
    return arr;
}
