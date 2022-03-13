#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//Прототипы функций
void toLowerCase(char* str, int size);
void underline_error_position(const uint8_t space_count);
void printErrorMessage(int errorPosition, const char* expected, const char* after);

int findWord(char** pointer, const char* word);
int findSymbol(char** pointer, char symbol);

int setDouble(char** pointer, double *doublePtr);
int setXY(char** pointer, double* x, double* y);
int setRadius(char** pointer, double* r);

int checkLineEnd(char** pointer);
int notOnlySpaces(char* const start, char* const end);

double circlePerimeter(double r);
double trianglePerimeter(double a, double b, double c);
double circleArea(double r);
double triangleArea(double a, double b, double c);
double segmentLength(double x1, double y1, double x2, double y2);



typedef struct Circle {
    double x;
    double y;
    double radius;
    double perimeter;
    double area;
} Circle;

typedef struct Point {
    double x;
    double y;
} Point;

typedef struct Triangle {
    Point t1;
    Point t2;
    Point t3;
    double perimeter;
    double area;
} Triangle;

int setCircle(char** pointer, Circle* circle, int errorPosition);
int setTriangle(char** pointer, Triangle* triangle, int errorPosition);
void calculateCircle(Circle* circle);
void calculateTriangle(Triangle* triangle);


int main()
{

    char str[100] = {'\0'};
    int size = 100;

    while (fgets(str, size, stdin)) {

        if (str[0] == '\n')
            break;


        str[size-1] = '\0';
        char* strPtr = str;
        printf("%s", strPtr);
        int err = -1;

        Circle circle;
        Triangle triangle;



        //уменьшаем регистр всех букв в строке
        toLowerCase(str, size);

        if((err = findWord(&strPtr, "circle")) == -1) {
            //Удачно прочли слово "circle"
            err = strPtr - str;
            err = setCircle(&strPtr, &circle, err);
            if(err != -1) {
                //Не удалось считать строку
                continue;
            }
            //Рассчитываем площадь и периметр
            calculateCircle(&circle);
            //выводим данные фигуры
            printf("x=%lf\n"
                   "y=%lf\n"
                   "radius=%lf\n"
                   "perimetr=%lf\n"
                   "area=%lf\n\n", circle.x, circle.y, circle.radius, circle.perimeter, circle.area);
        }
        else if((err = findWord(&strPtr, "triangle")) == -1){
            //Удачно прочли слово "triangle"
            err = strPtr - str;
            err = setTriangle(&strPtr, &triangle, err);
            if(err != -1) {
                //Не удалось считать строку
                continue;
            }
            //Рассчитываем площадь и периметр
            calculateTriangle(&triangle);
            //выводим данные фигуры
            printf("x1=%lf, y1=%lf\n"
                   "x2=%lf, y2=%lf\n"
                   "x3=%lf, y3=%lf\n"
                   "perimetr=%lf\n"
                   "area=%lf\n\n", triangle.t1.x, triangle.t1.y,
                   triangle.t2.x, triangle.t2.y,
                   triangle.t3.x, triangle.t3.y,
                   triangle.perimeter,
                   triangle.area);

        }
        else {
            //Не смогли распознать первое слово
            printErrorMessage(err, "\"Circle\" or \"Triangle\"", "begining");
            continue;
        }


    }

    return 0;
}


void toLowerCase(char* str, int size) {
    //уменьшаем регистр всех букв в строке
    for (uint8_t i = 0; str[i] != '\0' && i < size; i++) {
        if (str[i] >= 65 && str[i] <= 90)
            str[i] = str[i] + 32;
    }
}

void underline_error_position(const uint8_t space_count)
{
    for (uint8_t i = 0; i < space_count; i++)
        putc(' ', stdout);
    printf("^\n");
}


int findWord(char** pointer, const char* word) {
    //Коды возврата: -1 -- всё хорошо, n=>0 -- позиция ошибки
    char* start = *pointer;
    char* end = strstr(start, word);

//Проверка: искомого слова нет в строке
    if (end == NULL) {
        return 0;
    }

    //Проверка: перед искомым словом не только пробелы
    int errorPosition = notOnlySpaces(start, end);
    if(errorPosition > -1) {
        return errorPosition;
    }

    //Хороший исход: перемещаем указатель на символ после искомого слова
    *pointer = end + strlen(word);
    return -1;
}

int findSymbol(char** pointer, char symbol) {
    //Коды возврата: -1 -- всё хорошо, n>=0 -- позиция ошибки
    char* start = *pointer;
    char* end = strchr(start, symbol);

    //Проверка: искомый символ не найден в строке
    if (end == NULL) {
        return 0;
    }

    //Проверка: перед искомым символом не только пробелы
    int errorPosition = notOnlySpaces(start, end);
    if(errorPosition > -1) {
        return errorPosition;
    }

    //Хороший исход: переместили указатель на следующий символ после искомого
    *pointer = end + 1;
    return -1;

}


int setDouble(char** pointer, double* doublePtr) {
    //Коды возврата: -1 -- всё хорошо, n=>0 -- позиция ошибки
    char* start = *pointer;
    char* end = NULL;
    int errorPosition = 0;

    //Проверка: есть ли вообще цифры дальше в строке
    end = strpbrk(start, "+-0123456789");
    if(end == NULL) {
        return 0;
    }

    //Проверка: перед цифрой не только пробелы
    errorPosition = notOnlySpaces(start, end);
    if(errorPosition > -1) {
        return errorPosition;
    }

    //Считывание числа
    double tmp =  strtod(start, &end);

    //Проверка: введен только минус или только плюс (либо сразу после чисел -, либо +)
    if(*end == '+' || *end == '-') {
        errorPosition = end - start;
        return errorPosition;
    }



    //Всё хорошо: перемещаем указатель, устанавливаем переменную double
    *pointer = end;
    *doublePtr = tmp;
    return -1;

}

int setXY(char** pointer, double* x, double* y) {
    //Коды возврата: -1 -- всё хорошо, n>=0 -- позиция ошибки
    //выделяем координату окружности x
    char* start = *pointer;
    int localErrPosition = -1;
    int globalErrPosition = -1;

    //Ищем x
    localErrPosition = setDouble(pointer, x);
    if(localErrPosition > -1) {
        return localErrPosition;
    }

    //Увеличиваем счётчик позиции на всякий случай
    globalErrPosition = *pointer - start;

    //Ищем y
    localErrPosition = setDouble(pointer, y);
    if(localErrPosition > -1) {
        globalErrPosition += localErrPosition;
        return globalErrPosition;
    }

    //Всё хорошо: указатель не переставляем, так как он уже переставлен внутри функций setDouble()
    return -1;

}

int setRadius(char** pointer, double* radius) {
    char* start = *pointer;
    int errorPosition = -1;

    //Считываем радиус
    errorPosition = setDouble(pointer, radius);
    if(errorPosition > -1) {
        return errorPosition;
    }

    errorPosition = strcspn(start, "0123456789");

    //Проверка: радиус положительный
    if(*radius <= 0) {
        return errorPosition-1;
    }

    //Всё хорошо: указатель не переставляем, он переставлен внутри setDouble()
    return -1;
}

int checkLineEnd(char** pointer) {
    char* start;
    int errorPosition = 0;
    for (start = *pointer; *start != '\0' && *start != '\n'; start++, errorPosition++) {
        if (*start != ' ') {
            return errorPosition;
        }
    }
    return -1;
}

int notOnlySpaces(char* const start, char* const end) {
    char* tmpPtr = start;
    int errorPosition = 0;

    while(tmpPtr < end) {

        if(*tmpPtr != ' ') {
            return errorPosition;
        }

        tmpPtr++;
        errorPosition++;
    }

    //Всё хорошо
    return -1;
}

void printErrorMessage(int errorPosition, const char* expected, const char* after) {
    underline_error_position(errorPosition);
    if(strstr(after, "begining") != NULL) {
        printf("Error in column %d: expected \"%s\" in the begining of line\n\n", errorPosition, expected);
    }
    else if (strstr(after, "ending") != NULL) {
        printf("Error in column %d: unexpected token in the ending of line\n\n", errorPosition);

}
    else {
        printf("Error in column %d: expected \"%s\" after \"%s\"\n\n", errorPosition, expected, after);
    }
}

int setCircle(char** pointer, Circle* circle, int errorPosition)
{
    int err = -1;
    char* start = *pointer - errorPosition;

    //Считываем левую скобку
    if((err = findSymbol(pointer, '(')) != -1) {
        errorPosition += err;
        printErrorMessage(errorPosition, "(", "circle");
        return errorPosition;
    }
    errorPosition = (*pointer) - start;

    //Считываем координаты точки
    if((err = setXY(pointer, &(circle->x), &(circle->y))) != -1) {
        errorPosition += err;
        printErrorMessage(errorPosition, "<double> <double>", "(");
        return errorPosition;
    }
    errorPosition = (*pointer) - start;

    //Считываем запятую
    if((err = findSymbol(pointer, ',')) != -1) {
        errorPosition += err;
        printErrorMessage(errorPosition, ",", "<double> <double>");
        return errorPosition;
    }
    errorPosition = (*pointer) - start;

    //Считываем радиус
    if((err = setRadius(pointer, &(circle->radius))) != -1) {
        errorPosition += err;
        printErrorMessage(errorPosition, "non-negative <double>", ",");
        return errorPosition;
    }
    errorPosition = (*pointer) - start;

    //Считываем скобку
    if((err = findSymbol(pointer, ')')) != -1) {
        errorPosition += err;
        printErrorMessage(errorPosition, ")", "<double>");
        return errorPosition;
    }
    errorPosition = (*pointer) - start;

    //Проверяем, что после скобки ничего нет
    if((err = checkLineEnd(pointer)) != -1) {
        errorPosition += err;
        printErrorMessage(errorPosition, ")", "ending");
        return errorPosition;
    }

    return -1;
}

double circlePerimeter(double r) {
    double pi = 3.14159;
    return pi*2*r;
}

double trianglePerimeter(double a, double b, double c) {
    return a+b+c;
}

double circleArea(double r) {
    double pi = 3.14159;
    return pi*r*r;
}
double triangleArea(double a, double b, double c) {
    double hp =(a+b+c)/2;
    return sqrt(hp*(hp-a)*(hp-b)*(hp-c));
}

void calculateCircle(Circle* circle) {
    circle->perimeter = circlePerimeter(circle->radius);
    circle->area = circleArea(circle->radius);
}

double segmentLength(double x1, double y1, double x2, double y2) {
    return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}

void calculateTriangle(Triangle* triangle) {

    double a = segmentLength(triangle->t1.x, triangle->t2.x, triangle->t1.y, triangle->t2.y);
    double b = segmentLength(triangle->t2.x, triangle->t3.x, triangle->t2.y, triangle->t3.y);
    double c = segmentLength(triangle->t3.x, triangle->t1.x, triangle->t3.y, triangle->t1.y);

    triangle->perimeter = trianglePerimeter(a, b, c);
    triangle->area = triangleArea(a, b, c);
}

int setTriangle(char** pointer, Triangle* triangle, int errorPosition)
{
    int err = -1;
    char* start = *pointer - errorPosition;

    //Считываем левую скобку
    if((err = findSymbol(pointer, '(')) != -1) {
        errorPosition += err;
        printErrorMessage(errorPosition, "(", "triangle");
        return errorPosition;
    }
    errorPosition = (*pointer) - start;

    //Считываем координаты 1й точки
    if((err = setXY(pointer, &(triangle->t1.x), &(triangle->t1.y))) != -1) {
        errorPosition += err;
        printErrorMessage(errorPosition, "<double> <double>", "(");
        return errorPosition;
    }
    errorPosition = (*pointer) - start;

    //Считываем запятую
    if((err = findSymbol(pointer, ',')) != -1) {
        errorPosition += err;
        printErrorMessage(errorPosition, ",", "<double> <double>");
        return errorPosition;
    }
    errorPosition = (*pointer) - start;

    //Считываем координаты 2й точки
    if((err = setXY(pointer, &(triangle->t2.x), &(triangle->t2.y))) != -1) {
        errorPosition += err;
        printErrorMessage(errorPosition, "<double> <double>", "<double> <double>");
        return errorPosition;
    }
    errorPosition = (*pointer) - start;

//Считываем запятую
    if((err = findSymbol(pointer, ',')) != -1) {
        errorPosition += err;
        printErrorMessage(errorPosition, ",", "<double> <double>");
        return errorPosition;
    }
    errorPosition = (*pointer) - start;

    //Считываем координаты 3й точки
    if((err = setXY(pointer, &(triangle->t3.x), &(triangle->t3.y))) != -1) {
        errorPosition += err;
        printErrorMessage(errorPosition, "<double> <double>", "<double> <double>");
        return errorPosition;
    }
    errorPosition = (*pointer) - start;

    //Считываем запятую
    if((err = findSymbol(pointer, ',')) != -1) {
        errorPosition += err;
        printErrorMessage(errorPosition, ",", "<double> <double>");
        return errorPosition;
    }
    errorPosition = (*pointer) - start;

    //Считываем координаты 4й точки отдельно и проверяем, что она соответствует первой точке
    double x, y;
    if((err = setXY(pointer, &x, &y)) != -1) {
        errorPosition += err;
        printErrorMessage(errorPosition, "<double> <double>", "<double> <double>");
        return errorPosition;
    }
    if(x != triangle->t1.x || y != triangle->t1.y) {
        printErrorMessage(errorPosition, "the same <double> <double> point as the first point", ",");
        return errorPosition;
    }
    errorPosition = (*pointer) - start;

    //Считываем скобку
    if((err = findSymbol(pointer, ')')) != -1) {
        errorPosition += err;
        printErrorMessage(errorPosition, ")", "<double>");
        return errorPosition;
    }
    errorPosition = (*pointer) - start;

    //Проверяем, что после скобки ничего нет
    if((err = checkLineEnd(pointer)) != -1) {
        errorPosition += err;
        printErrorMessage(errorPosition, ")", "ending");
        return errorPosition;
    }

    return -1;
}
