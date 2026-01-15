
#include <stdio.h>
#include <math.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <float.h>   

#define EPS 1e-6

// Прототипы функций
double f(double x);
int is_defined(double x);
double derivative(double x);
int find_min_and_max(double a, double b, double h,double* x_min_out, double* f_min_out, double* x_max_out, double* f_max_out);
double find_x_by_y(double Y, double a, double b);
void create_table(double a, double b, int points);
void save_calc_result(const char* type, double param1, double param2, double param3, double result1, double result2);

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

            if (is_defined(x)) {
                double val = f(x);
                printf("f(%.4lf) = %.2lf\n", x, val);
                save_calc_result("f(x)", x, 0, 0, val, 0);
            }
            else {
                printf("Функция не определена\n");
            }
            break;

        case 2: {
            printf("Введите начало интервала a: ");
            scanf("%lf", &a);

            printf("Введите конец интервала b: ");
            scanf("%lf", &b);

            int points;
            printf("Введите количество точек: ");
            scanf("%d", &points);
            int c;
            while ((c = getchar()) != '\n' && c != EOF);

            create_table(a, b, points);

            break;
        }

        case 3:
            printf("Введите начало отрезка : ");
            scanf("%lf", &a);

            printf("Введите конец отрезка : ");
            scanf("%lf", &b);

            printf("Введите шаг : ");
            scanf("%lf", &h);

            double xmin, fmin, xmax, fmax;

            if (find_min_and_max(a, b, h, &xmin, &fmin, &xmax, &fmax))
            {
                double x_ext, f_ext;

                if (fabs(fmin) >= fabs(fmax)) {
                    x_ext = xmin;
                    f_ext = fmin;
                }
                else {
                    x_ext = xmax;
                    f_ext = fmax;
                }

                printf("Экстремум с наибольшим модулем:\n");
                printf("x = %.4lf      |f(x)| = %.4lf\n",
                    x_ext, f_ext, fabs(f_ext));

                save_calc_result("extremum", a, b, h, x_ext, f_ext);
            }
            else
            {
                printf("Экстремумы не найдены (нет определённых точек на отрезке)\n");
            }

            break;

        case 4:
            printf("Введите значение Y: ");
            scanf("%lf", &y);

            printf("Введите начало интервала a : ");
            scanf("%lf", &a);

            printf("Введите конец интервала b: ");
            scanf("%lf", &b);

            x = find_x_by_y(y, a, b);

            if (!isnan(x)) {
                printf("x = %.4lf\n", x);
                save_calc_result("inverse", y, a, b, x, f(x));
            }
            else {
                printf("Значение x не найдено\n");
            }

            break;

        case 5:
            printf("Введите x: ");
            scanf("%lf", &x);

            if (is_defined(x)) {
                double val = derivative(x);
                printf("f'(%.2lf) = %.2lf\n", x, val);
                save_calc_result("derivative", x, 0, 0, val, 0);
            }
            else {
                printf("Функция не определена\n");
            }
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

// ────────────────────────────────────────────────
// Реализация функций
// ────────────────────────────────────────────────

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

// Переименовать функцию
int find_min_and_max(double a, double b, double h,
    double* x_min_out, double* f_min_out,
    double* x_max_out, double* f_max_out)
{
    if (a >= b || h <= 0) return 0;

    int found = 0;
    double fmin = INFINITY, fmax = -INFINITY;
    double xmin = a, xmax = a;

    // проверяем концы отрезка (очень важно!)
    if (is_defined(a)) {
        double fa = f(a);
        fmin = fmax = fa;
        xmin = xmax = a;
        found = 1;
    }
    if (is_defined(b)) {
        double fb = f(b);
        if (!found || fb < fmin) { fmin = fb; xmin = b; }
        if (!found || fb > fmax) { fmax = fb; xmax = b; }
        found = 1;
    }

    for (double x = a + h; x <= b + EPS; x += h)
    {
        if (!is_defined(x)) continue;
        double fx = f(x);
        if (fx < fmin) { fmin = fx; xmin = x; }
        if (fx > fmax) { fmax = fx; xmax = x; }
        found = 1;
    }

    if (!found) return 0;

    *x_min_out = xmin;  *f_min_out = fmin;
    *x_max_out = xmax;  *f_max_out = fmax;
    return 1;
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

void create_table(double a, double b, int points) {
    if (a >= b || points < 2) {
        printf("Ошибка: некорректные параметры\n");
        return;
    }

    double step = (b - a) / (points - 1);

    printf("\n+--------------+------------------+\n");
    printf("|      x       |       f(x)       |\n");
    printf("+--------------+------------------+\n");

    time_t now = time(NULL);
    char fname[64];
    strftime(fname, sizeof(fname), "table_%Y%m%d_%H%M%S.txt", localtime(&now));
    FILE* fp = fopen(fname, "w");
    if (!fp) {
        printf("Не удалось сохранить таблицу\n");
    }
    else {
        fprintf(fp, "Таблица для [%.4f ; %.4f] с %d точками\n", a, b, points);
    }

    // Удаляем вызов save_calc_result здесь - сохраняем все точки таблицы одним вызовом
    for (int i = 0; i < points; i++) {
        double x = a + i * step;

        if (is_defined(x)) {
            double val = f(x);
            printf("| %12.4lf | %16.2lf |\n", x, val);
            if (fp) fprintf(fp, "%.6lf\t%.8lf\n", x, val);
        }
        else {
            printf("| %12.4lf | %16s |\n", x, "не определена");
            if (fp) fprintf(fp, "%.6lf\tне определена\n", x);
        }
    }

    printf("+--------------+------------------+\n");

    if (fp) {
        fclose(fp);
        printf("Таблица сохранена в %s\n", fname);
    }

    // Сохраняем информацию о всей таблице одним вызовом
    save_calc_result("table", a, b, points, step, 0);
}

void save_calc_result(const char* type, double param1, double param2, double param3, double result1, double result2) {
    FILE* fp = fopen("calc_results.txt", "a");
    if (!fp) {
        printf("Не удалось открыть файл для сохранения результата\n");
        return;
    }

    time_t now = time(NULL);
    char time_str[32];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&now));

    fprintf(fp, "[%s] Тип: %s\n", time_str, type);

    if (strcmp(type, "f(x)") == 0 || strcmp(type, "derivative") == 0) {
        fprintf(fp, "x = %.6lf    значение = %.8lf\n", param1, result1);
    }
    else if (strcmp(type, "extremum") == 0) {
        fprintf(fp, "Интервал [%.4lf ; %.4lf] шаг %.4lf\n", param1, param2, param3);
        fprintf(fp, "x = %.6lf    f(x) = %.8lf    |f(x)| = %.8lf\n", result1, result2, fabs(result2));
    }
    else if (strcmp(type, "inverse") == 0) {
        fprintf(fp, "Y = %.6lf    интервал [%.4lf ; %.4lf]\n", param1, param2, param3);
        fprintf(fp, "x ≈ %.6lf    f(x) = %.8lf\n", result1, result2);
    }
    else if (strcmp(type, "table") == 0) {
        fprintf(fp, "Таблица на интервале [%.4lf ; %.4lf]\n", param1, param2);
        fprintf(fp, "Количество точек: %.0lf    Шаг: %.6lf\n", param3, result1);
        fprintf(fp, "Таблица сохранена в отдельном файле\n");
    }

    fprintf(fp, "----------------------------------------\n\n");
    fclose(fp);

    // Убираем вывод для каждого сохранения, оставляем только для отдельных операций
    if (strcmp(type, "table") != 0) {
        printf("Результат сохранён в calc_results.txt\n");
    }
}