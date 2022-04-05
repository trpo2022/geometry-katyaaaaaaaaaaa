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
            printCircle(&circle, figureName);
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
            printTriangle(&triangle, figureName);

        } else {
            //Не смогли распознать первое слово
            printErrorMessage(err, "\"Circle\" or \"Triangle\"", "begining");
            continue;
        }
    }

    return 0;
}
