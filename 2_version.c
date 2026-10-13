#include <stdio.h>

int second_main(void)
{
    double x, y;

    printf("Enter x: ");
    scanf("%lf", &x);

    if (x > -15 && x <= 3)
    {
        y = 4 * x * x + 2;
        printf("y = %.2f\n", y);
    }
    else if (x <= -30 || x > 20)
    {
        y = 3 * x * x * x / 4 - 5;
        printf("y = %.2f\n", y);
    }
    else
    {
        printf("Function is not defined\n");
    }

    return 0;
}