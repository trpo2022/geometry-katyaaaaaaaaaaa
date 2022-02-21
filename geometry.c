#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_error_position(const uint8_t space_count)
{
    for (uint8_t i = 0; i < space_count; i++)
        putc(' ', stdout);
    printf("^\n");
}

int main()
{
    char str[100] = {'\0'};
    while (fgets(str, 100, stdin)) {
        if (str[0] == '\n')
            break;

        printf("%s", str);

        //уменьшаем регистр всех букв в строке
        for (uint8_t i = 0; str[i] != '\0'; i++) {
            if (str[i] >= 65 && str[i] <= 90)
                str[i] = str[i] + 32;
        }
        //поиск слова cirlce
        char* figure = strstr(str, "circle");
        if (figure == NULL) {
            print_error_position(0);
            printf("Error at column 0: expected 'circle'\n\n");
            continue;
        }

        //поиск левой круглой скобки после cirlce
        char* l_bracket = strchr(str, '(');
        if (l_bracket == NULL || figure > l_bracket) {
            print_error_position((figure + 6) - str);
            printf("Error at column %ld: expected \'(\'\n\n",
                   (figure + 6) - str);
            continue;
        }

        //выделяем координату окружности x
        char* end = NULL;
        double x_center = strtod(l_bracket + 1, &end);
        if (*end != ' ') {
            print_error_position(end - str);
            printf("Error at column %ld: expected '<double>'\n\n", end - str);
            continue;
        }

        //выделяем координату y окружности
        double y_center = strtod(end, &end);
        if (*end != ' ' && *end != ',') {
            print_error_position(end - str);
            printf("Error at column %ld: expected '<double>'\n\n", end - str);
            continue;
        }

        //поиск запятой,отделяющий центр окружности от радиуса
        if (*end != ',') {
            char* comma = strchr(end + 1, ',');
            if (comma == NULL) {
                print_error_position(end - str);
                printf("Error at column %ld: expected \',\'\n\n", end - str);
                continue;
            }
            end = comma;
        }

        //выделяем радиус окружности
        double radius = strtod(end + 1, &end);
        if (*end == '\n' || *end == '\0') {
            print_error_position(end - str);
            printf("Error at column %ld: expected \')\'\n\n", end - str);
            continue;
        }
        if ((*end != ' ' && *end != ')')
            || ((*end == ' ' || *end == ')') && *(end - 1) == ',')) {
            print_error_position(end - str);
            printf("Error at column %ld: expected '<double>'\n\n", end - str);
            continue;
        }

        //проверка,что есть правая круглая скобка
        char* r_bracket = end;
        if (*end != ')') {
            r_bracket = strchr(end + 1, ')');
            if (r_bracket == NULL) {
                print_error_position(end - str);
                printf("Error at column %ld: expected \')\'\n\n", end - str);
                continue;
            }
        }

        //проверка,что нет никаких символов кроме пробела после правой круглой
        //скобки
        for (r_bracket++; *r_bracket != '\0' && *r_bracket != '\n'; r_bracket++)
            if (*r_bracket != ' ') {
                print_error_position(r_bracket - str);
                printf("Error at column %ld: unexpected token\n\n",
                       r_bracket - str);
                continue;
            }

        //выводим данные фигуры
        printf("x=%lf\ny=%lf\nradius=%lf\n\n", x_center, y_center, radius);
    }

    return 0;
}
