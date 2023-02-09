#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <cmath>
#include "document_worker.cpp"


MatPoint mat_points[QUANTITY];

void getForces(MatPoint *mat_points)
{
    for (int i = 0; i < QUANTITY; i++)
    {
        for (int j = 0; j < QUANTITY; j++)
        {
            if (i == j)
                continue;

            double x_dif = mat_points[j].x - mat_points[i].x;
            double y_dif = mat_points[j].y - mat_points[i].y;
            double r = x_dif * x_dif + y_dif * y_dif;
            double sqrt_r = sqrt(r);
            double f = GRAVITY_CONSTANT * mat_points[i].mass * mat_points[j].mass / r;
            mat_points[i].Fx = mat_points[i].Fx + f * x_dif * sqrt_r;
            mat_points[i].Fy = mat_points[i].Fy + f * y_dif * sqrt_r;
        }
    }
}

void moveMatPoints(MatPoint *mat_points)
{
    for (int i = 0; i < QUANTITY; i++)
    {
        double dvx = mat_points[i].Fx / mat_points[i].mass * dt;
        double dvy = mat_points[i].Fy / mat_points[i].mass * dt;

        mat_points[i].vx = mat_points[i].vx + dvx;
        mat_points[i].vy = mat_points[i].vy + dvy;

        mat_points[i].x = mat_points[i].x + (mat_points[i].vx) / 2 * dt;
        mat_points[i].y = mat_points[i].y + (mat_points[i].vy) / 2 * dt;

        mat_points[i].Fx = 0;
        mat_points[i].Fy = 0;
    }
}

int main()
{

    create_document(QUANTITY, "intro.txt");
    get_mat_points(mat_points, "intro.txt");
    for (int row_index = 0; row_index < QUANTITY; row_index++)
    {
        mat_points[row_index].Fx = 0;
        mat_points[row_index].Fy = 0;
    }

    // print_mat_points(mat_points);

    clock_t begin = clock();


    char * document_with_coordinates = "coordinates.txt";
    fopen(document_with_coordinates, "w");

    for (int step = 0; step < STEPS; step++)
    {
        getForces(mat_points);
        moveMatPoints(mat_points);
        // print_mat_points(mat_points);
        print_mat_points_in_file(mat_points, document_with_coordinates, step);
    }

    clock_t end = clock();
    
    printf("The elapsed time is %f seconds", (double)(end - begin) / CLOCKS_PER_SEC);

    return 0;
}