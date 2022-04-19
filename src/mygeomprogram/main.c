#include <geom.h>
#include <stdio.h>

int main()
{
    char str[100] = {'\0'};
    int size = 100;

    CircleArray* c_array = createCircleArray();
    TriangleArray* t_array = createTriangleArray();

    while (fgets(str, size, stdin)) {
        if (str[0] == '\n')
            break;

        str[size - 1] = '\0';
        char* strPtr = str;

        int err = -1;

        char figureName[9];

        //уменьшаем регистр всех букв в строке
        toLowerCase(str, size);

        if ((findWord(&strPtr, figureName, "circle")) == -1) {
            //Удачно прочли слово "circle"
            err = strPtr - str;
            addCircle(c_array);
            err = setCircle(&strPtr, &(c_array->elems[c_array->num - 1]), err);
            if (err != -1) {
                //Не удалось считать строку
                removeCircle(c_array);
                continue;
            }
            //Рассчитываем площадь и периметр
            calculateCircle(&(c_array->elems[c_array->num - 1]));

        } else if ((err = findWord(&strPtr, figureName, "triangle")) == -1) {
            //Удачно прочли слово "triangle"
            err = strPtr - str;
            addTriangle(t_array);
            err = setTriangle(
                    &strPtr, &(t_array->elems[t_array->num - 1]), err);
            if (err != -1) {
                //Не удалось считать строку
                removeTriangle(t_array);
                continue;
            }
            //Рассчитываем площадь и периметр
            calculateTriangle(&(t_array->elems[t_array->num - 1]));

        } else {
            //Не смогли распознать первое слово
            printErrorMessage(err, "\"Circle\" or \"Triangle\"", "begining");
            continue;
        }
    }

    printAll(c_array, t_array);
    deleteCircleArray(c_array);
    deleteTriangleArray(t_array);
    printf("The End\n");
    return 0;
}
