#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "intro_creator.cpp"

#define QUANTITY 100

int main()
{
    create_document(QUANTITY, "intro.txt");
    int ** mat_points = new int *;
    mat_points = get_mat_points("intro.txt");
    for (int row_index=0; row_index<QUANTITY; row_index++)
    {
        for (int col_index=0; col_index<7; col_index++)
            printf("%d ", mat_points[row_index][col_index]);
        printf("\n");
    }
    return 0;
}