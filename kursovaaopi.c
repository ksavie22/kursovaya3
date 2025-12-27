#include <stdio.h>
#include <math.h>
#include <locale.h>
#include <stdlib.h>
#define EPS 1e-6



double f(double x);
int is_defined(double x);
double derivative(double x);
int find_extremum(double a, double b, double h, double* x_ext, double* f_ext);
double find_x_by_y(double Y, double a, double b);
void create_table();



int main() {
    setlocale(LC_CTYPE, "rus");
    int choice;
    double x, y, a, b, h, result;
    printf("Автор: Гвардеев Станислав\n");
    printf("Группа: БИЦ-252\n");
    printf("Приветствую тебя пользователь, в этой программе ты можешь:\n");
    printf("\n |1.вычислить значение функции в точке\n |2.построить таблицу значений\n |3.найти экстремум\n |4.осуществить поиск аргумента по заданному значению функции \n |5.вычислить численное значение производной \n");
    do {
        printf("\n--- МЕНЮ ---\n");
        printf("1. Значение f(x)\n");
        printf("2. Таблица значений\n");
        printf("3. Экстремум на отрезке\n");
        printf("4. Найти x по f(x) ~ Y\n");
        printf("5. Производная в точке\n");
        printf("0. Выход\n");
        printf("Выбор: ");
        scanf("%d", &choice);

        switch (choice) {

        case 1:
            printf("Введите x: ");
            scanf("%lf", &x);

            if (is_defined(x))
                printf("f(%.4lf) = %.2lf\n", x, f(x));
            else
                printf("Функция не определена\n");
            break;

        case 2:
            create_table();
            break;

        case 3:
            printf("Введите начало отрезка : ");
            scanf("%lf", &a);

            printf("Введите конец отрезка : ");
            scanf("%lf", &b);

            printf("Введите шаг : ");
            scanf("%lf", &h);

            if (find_extremum(a, b, h, &x, &result))
                printf("Экстремум: x = %.2lf, f(x) = %.2lf\n", x, result);
            else
                printf("Экстремум не найден\n");
            break;

        case 4:
            printf("Введите значение Y: ");
            scanf("%lf", &y);

            printf("Введите начало интервала a : ");
            scanf("%lf", &a);

            printf("Введите конец интервала b: ");
            scanf("%lf", &b);

            x = find_x_by_y(y, a, b);

            if (!isnan(x))
                printf("x ≈ %.4lf\n", x);
            else
                printf("Значение x не найдено\n");

            break;

        case 5:
            printf("Введите x: ");
            scanf("%lf", &x);

            if (is_defined(x))
                printf("f'(%.2lf) = %.2lf\n", x, derivative(x));
            else
                printf("Функция не определена\n");
            break;

        case 0:
            printf("Выход.\n");
            break;

        default:
            printf("Неверный пункт меню\n");
        }

    } while (choice != 0);
    
    
   
    return 0;
}



int is_defined(double x) {
    if (x < -2)
        return fabs(x) > 1.0;
    return 1;
}

double f(double x) {
    double sum = 0.0;

    if (x < -2)
        return 1.0 / log(fabs(x));
    else if (x < 1)
        return cos(x * x) * exp(-x);
    else {
        for (int n = 0; n <= 5; n++)
            sum += pow(x, 2 * n + 1) / (2 * n + 1);
        return sum;
    }
}

double derivative(double x) {
    return (f(x + EPS) - f(x - EPS)) / (2 * EPS);
}

double find_x_by_y(double Y, double a, double b) {
    double h = 0.001;

    if (a >= b)
        return NAN;

    for (double x = a; x <= b + EPS; x += h) {
        if (is_defined(x) && fabs(f(x) - Y) < 1e-3) {
            return x;
        }
    }

    return NAN;
}

int find_extremum(double a, double b, double h,
    double* x_ext, double* f_ext) {
    int found = 0;
    double fmin = 0, fmax = 0, xmin = 0, xmax = 0;

    if (a >= b || h <= 0)
        return 0;

    for (double x = a; x <= b + EPS; x += h) {
        if (!is_defined(x)) continue;

        double fx = f(x);

        if (!found) {
            fmin = fmax = fx;
            xmin = xmax = x;
            found = 1;
        }

        if (fx < fmin) {
            fmin = fx;
            xmin = x;
        }

        if (fx > fmax) {
            fmax = fx;
            xmax = x;
        }
    }

    if (!found) return 0;

    if (fabs(fmin) >= fabs(fmax)) {
        *x_ext = xmin;
        *f_ext = fmin;
    }
    else {
        *x_ext = xmax;
        *f_ext = fmax;
    }

    return 1;
}

void create_table() {
    double a, b;
    int points;

    printf("Введите начало интервала a: ");
    scanf("%lf", &a);

    printf("Введите конец интервала b: ");
    scanf("%lf", &b);

    printf("Введите количество точек: ");
    scanf("%d", &points);

    if (a >= b || points < 2) {
        printf("Ошибка: некорректные параметры\n");
        return;
    }

    double step = (b - a) / (points - 1);

    printf("\n+--------------+------------------+\n");
    printf("|      x       |       f(x)       |\n");
    printf("+--------------+------------------+\n");

    for (int i = 0; i < points; i++) {
        double x = a + i * step;

        if (is_defined(x))
            printf("| %12.4lf | %16.2lf |\n", x, f(x));
        else
            printf("| %12.4lf | %16s |\n", x, "не определена");
    }

    printf("+--------------+------------------+\n");
}
