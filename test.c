#include <stdio.h>
#include "pchip.h"

int main()
{
    double x[38], y[38], new_x[377], new_y[377];
    FILE *fp_x = fopen("../x.txt", "r");
    FILE *fp_y = fopen("../y.txt", "r");
    FILE *fp_x1 = fopen("../x1.txt", "r");
    FILE *fp_y1 = fopen("../y1.txt", "w");

    if (fp_x == 0 || fp_x1 == 0 | fp_y == 0)
    {
        printf("Not find data file!\n");
        return -1;
    }
    int i = 0;
    while (!feof(fp_x))
    {
        fscanf(fp_x, "%lf", &x[i]);
        i++;
    }
    i = 0;
    while (!feof(fp_y))
    {
        fscanf(fp_y, "%lf", &y[i]);
        i++;
    }
    i = 0;
    while (!feof(fp_x1))
    {
        fscanf(fp_x1, "%lf", &new_x[i]);
        i++;
    }

    pchip(x, y, 38, new_x, 377, new_y);

    for (int i = 0; i < 377; i++)
    {
        printf("%lf \n", new_y[i]);
        fprintf(fp_y1, "%lf \n", new_y[i]);
    }

    fclose(fp_x);
    fclose(fp_x1);
    fclose(fp_y);
    fclose(fp_y1);

    return 0;
}