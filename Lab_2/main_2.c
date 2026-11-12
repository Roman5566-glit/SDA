#include <stdio.h>
#include <math.h>

#ifdef _WIN32
#include <windows.h>
#endif

#define PI 3.14159265358979323846

int main(void)
{
#ifdef _WIN32
    SetConsoleOutputCP(65001);
#endif
    int n;
    unsigned long long ops = 0;

    printf("Введіть натуральне n: ");
    if (scanf("%d", &n) != 1 || n < 1) {
        printf("Помилка: n має бути натуральним числом.\n");
        return 1;
    }

    double x = 1.0 / 3.0;                  ops += 2;
    double c = x * x / (PI * PI);          ops += 4;
    double P = 1.0;                        ops += 1;

    ops += 1;
    for (int i = 1; i <= n; i++) {
        ops += 2;

        long long S = 0;                   ops += 1;

        ops += 1;
        for (int j = 1; j <= i; j++) {
            ops += 2;
            S += 2 * j - 1;                ops += 4;
        }
        ops += 1;

        P *= 1.0 - c / S;                  ops += 4;
    }
    ops += 1;

    printf("Спосіб 1 (вкладені цикли), n = %d\n", n);
    printf("P = %.7f\n", P);
    printf("Кількість операцій: %llu\n", ops);
    printf("Еталон sin(x)/x = %.7f, похибка = %.3e\n", sin(x) / x, fabs(P - sin(x) / x));
    return 0;
}
