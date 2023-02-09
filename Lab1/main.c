#include "Filemanager.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define N 10

void print_array(int** arr)
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 5; j++)
            printf("%d ", arr[i][j]);

        printf("\n");
    }
}

int main()
{
    srand(time(NULL));

    //generate_file(N, "input.txt");
    int** points = read_file(N, "input.txt");

    print_array(points);

    return 0;
}
