#include <geom.h>
#include <stdio.h>

int main()
{
    char str[100] = {'\0'};
    int size = 100;

    while (fgets(str, size, stdin)) {
        if (str[0] == '\n')
            break;

        str[size - 1] = '\0';
        char* strPtr = str;
        printf("%s", strPtr);
        int err = -1;

        Circle circle;
        Triangle triangle;
        char figureName[9];

        //уменьшаем регистр всех букв в строке
        toLowerCase(str, size);

        if ((err = findWord(&strPtr, figureName, "circle")) == -1) {
            //Удачно прочли слово "circle"
            err = strPtr - str;
            err = setCircle(&strPtr, &circle, err);
            if (err != -1) {
                //Не удалось считать строку
                continue;
            }
            //Рассчитываем площадь и периметр
            calculateCircle(&circle);
            //выводим данные фигуры
            printf("%s:\n"
                   "x=%lf\n"
                   "y=%lf\n"
                   "radius=%lf\n"
                   "perimetr=%lf\n"
                   "area=%lf\n\n",
                   figureName,
                   circle.x,
                   circle.y,
                   circle.radius,
                   circle.perimeter,
                   circle.area);
        } else if ((err = findWord(&strPtr, figureName, "triangle")) == -1) {
            //Удачно прочли слово "triangle"
            err = strPtr - str;
            err = setTriangle(&strPtr, &triangle, err);
            if (err != -1) {
                //Не удалось считать строку
                continue;
            }
            //Рассчитываем площадь и периметр
            calculateTriangle(&triangle);
            //выводим данные фигуры
            printf("%s:\n"
                   "x1=%lf, y1=%lf\n"
                   "x2=%lf, y2=%lf\n"
                   "x3=%lf, y3=%lf\n"
                   "perimetr=%lf\n"
                   "area=%lf\n\n",
                   figureName,
                   triangle.t1.x,
                   triangle.t1.y,
                   triangle.t2.x,
                   triangle.t2.y,
                   triangle.t3.x,
                   triangle.t3.y,
                   triangle.perimeter,
                   triangle.area);

        } else {
            //Не смогли распознать первое слово
            printErrorMessage(err, "\"Circle\" or \"Triangle\"", "begining");
            continue;
        }
    }

    return 0;
}
