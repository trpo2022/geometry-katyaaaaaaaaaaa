#include <ctype.h>
#include <geom.h>
#include <inttypes.h>
#include <malloc.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void toLowerCase(char* str, int size)
{
    //уменьшаем регистр всех букв в строке
    for (uint8_t i = 0; str[i] != '\0' && i < size; i++) {
        if (isupper(str[i]) != 0)
            str[i] = tolower(str[i]);
    }
}

void underline_error_position(const uint8_t space_count)
{
    for (uint8_t i = 0; i < space_count; i++)
        putc(' ', stdout);
    printf("^\n");
}

int findWord(char** pointer, char* name, const char* word)
{
    //Коды возврата: -1 -- всё хорошо, n=>0 -- позиция ошибки
    char* start = *pointer;
    char* end = strstr(start, word);
    strcpy(name, word);

    //Проверка: искомого слова нет в строке
    if (end == NULL) {
        return 0;
    }

    //Проверка: перед искомым словом не только пробелы
    int errorPosition = notOnlySpaces(start, end);
    if (errorPosition > -1) {
        return errorPosition;
    }

    //Хороший исход: перемещаем указатель на символ после искомого слова
    *pointer = end + strlen(word);
    return -1;
}

int findSymbol(char** pointer, char symbol)
{
    //Коды возврата: -1 -- всё хорошо, n>=0 -- позиция ошибки
    char* start = *pointer;
    char* end = strchr(start, symbol);

    //Проверка: искомый символ не найден в строке
    if (end == NULL) {
        return 0;
    }

    //Проверка: перед искомым символом не только пробелы
    int errorPosition = notOnlySpaces(start, end);
    if (errorPosition > -1) {
        return errorPosition;
    }

    //Хороший исход: переместили указатель на следующий символ после искомого
    *pointer = end + 1;
    return -1;
}

int setDouble(char** pointer, double* doublePtr)
{
    //Коды возврата: -1 -- всё хорошо, n=>0 -- позиция ошибки
    char* start = *pointer;
    char* end = NULL;
    int errorPosition = 0;

    //Проверка: есть ли вообще цифры дальше в строке
    end = strpbrk(start, "+-0123456789");
    if (end == NULL) {
        return 0;
    }

    //Проверка: перед цифрой не только пробелы
    errorPosition = notOnlySpaces(start, end);
    if (errorPosition > -1) {
        return errorPosition;
    }

    //Считывание числа
    double tmp = strtod(start, &end);

    //Проверка: введен только минус или только плюс (либо сразу после чисел -,
    //либо +)
    if (*end == '+' || *end == '-') {
        errorPosition = end - start;
        return errorPosition;
    }

    //Всё хорошо: перемещаем указатель, устанавливаем переменную double
    *pointer = end;
    *doublePtr = tmp;
    return -1;
}

int setXY(char** pointer, double* x, double* y)
{
    //Коды возврата: -1 -- всё хорошо, n>=0 -- позиция ошибки
    //выделяем координату окружности x
    char* start = *pointer;
    int localErrPosition = -1;
    int globalErrPosition = -1;

    //Ищем x
    localErrPosition = setDouble(pointer, x);
    if (localErrPosition > -1) {
        return localErrPosition;
    }

    //Увеличиваем счётчик позиции на всякий случай
    globalErrPosition = *pointer - start;

    //Ищем y
    localErrPosition = setDouble(pointer, y);
    if (localErrPosition > -1) {
        globalErrPosition += localErrPosition;
        return globalErrPosition;
    }

    //Всё хорошо: указатель не переставляем, так как он уже переставлен внутри
    //функций setDouble()
    return -1;
}

int setRadius(char** pointer, double* radius)
{
    char* start = *pointer;
    int errorPosition = -1;

    //Считываем радиус
    errorPosition = setDouble(pointer, radius);
    if (errorPosition > -1) {
        return errorPosition;
    }

    errorPosition = strcspn(start, "0123456789");

    //Проверка: радиус положительный
    if (*radius <= 0) {
        return errorPosition - 1;
    }

    //Всё хорошо: указатель не переставляем, он переставлен внутри setDouble()
    return -1;
}

int checkLineEnd(char** pointer)
{
    char* start;
    int errorPosition = 0;
    for (start = *pointer; *start != '\0' && *start != '\n';
         start++, errorPosition++) {
        if (*start != ' ') {
            return errorPosition;
        }
    }
    return -1;
}

int notOnlySpaces(char* const start, char* const end)
{
    char* tmpPtr = start;
    int errorPosition = 0;

    while (tmpPtr < end) {
        if (*tmpPtr != ' ') {
            return errorPosition;
        }

        tmpPtr++;
        errorPosition++;
    }

    //Всё хорошо
    return -1;
}

void printErrorMessage(
        int errorPosition, const char* expected, const char* after)
{
    underline_error_position(errorPosition);
    if (strstr(after, "begining") != NULL) {
        printf("Error in column %d: expected \"%s\" in the begining of "
               "line\n\n",
               errorPosition,
               expected);
    } else if (strstr(after, "ending") != NULL) {
        printf("Error in column %d: unexpected token in the ending of line\n\n",
               errorPosition);

    } else {
        printf("Error in column %d: expected \"%s\" after \"%s\"\n\n",
               errorPosition,
               expected,
               after);
    }
}

int setCircle(char** pointer, Circle* circle, int errorPosition)
{
    int err = -1;
    char* start = *pointer - errorPosition;

    //Считываем левую скобку
    if ((err = findSymbol(pointer, '(')) != -1) {
        errorPosition += err;
        printErrorMessage(errorPosition, "(", "circle");
        return errorPosition;
    }
    errorPosition = (*pointer) - start;

    //Считываем координаты точки
    if ((err = setXY(pointer, &(circle->x), &(circle->y))) != -1) {
        errorPosition += err;
        printErrorMessage(errorPosition, "<double> <double>", "(");
        return errorPosition;
    }
    errorPosition = (*pointer) - start;

    //Считываем запятую
    if ((err = findSymbol(pointer, ',')) != -1) {
        errorPosition += err;
        printErrorMessage(errorPosition, ",", "<double> <double>");
        return errorPosition;
    }
    errorPosition = (*pointer) - start;

    //Считываем радиус
    if ((err = setRadius(pointer, &(circle->radius))) != -1) {
        errorPosition += err;
        printErrorMessage(errorPosition, "non-negative <double>", ",");
        return errorPosition;
    }
    errorPosition = (*pointer) - start;

    //Считываем скобку
    if ((err = findSymbol(pointer, ')')) != -1) {
        errorPosition += err;
        printErrorMessage(errorPosition, ")", "<double>");
        return errorPosition;
    }
    errorPosition = (*pointer) - start;

    //Проверяем, что после скобки ничего нет
    if ((err = checkLineEnd(pointer)) != -1) {
        errorPosition += err;
        printErrorMessage(errorPosition, ")", "ending");
        return errorPosition;
    }

    return -1;
}

double circlePerimeter(double r)
{
    return M_PI * 2 * r;
}

double trianglePerimeter(double a, double b, double c)
{
    return a + b + c;
}

double circleArea(double r)
{
    return M_PI * r * r;
}
double triangleArea(double a, double b, double c)
{
    double hp = (a + b + c) / 2;
    return sqrt(hp * (hp - a) * (hp - b) * (hp - c));
}

void calculateCircle(Circle* circle)
{
    circle->perimeter = circlePerimeter(circle->radius);
    circle->area = circleArea(circle->radius);
}

double segmentLength(double x1, double x2, double y1, double y2)
{
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

void calculateTriangle(Triangle* triangle)
{
    double a = segmentLength(
            triangle->t1.x, triangle->t2.x, triangle->t1.y, triangle->t2.y);
    double b = segmentLength(
            triangle->t2.x, triangle->t3.x, triangle->t2.y, triangle->t3.y);
    double c = segmentLength(
            triangle->t3.x, triangle->t1.x, triangle->t3.y, triangle->t1.y);

    triangle->perimeter = trianglePerimeter(a, b, c);
    triangle->area = triangleArea(a, b, c);
}

int setTriangle(char** pointer, Triangle* triangle, int errorPosition)
{
    int err = -1;
    char* start = *pointer - errorPosition;

    //Считываем левую скобку
    if ((err = findSymbol(pointer, '(')) != -1) {
        errorPosition += err;
        printErrorMessage(errorPosition, "(", "triangle");
        return errorPosition;
    }
    errorPosition = (*pointer) - start;
    //Считываем вторую левую скобку
    if ((err = findSymbol(pointer, '(')) != -1) {
        errorPosition += err;
        printErrorMessage(errorPosition, "(", "triangle");
        return errorPosition;
    }
    errorPosition = (*pointer) - start;
    //Считываем координаты 1й точки
    if ((err = setXY(pointer, &(triangle->t1.x), &(triangle->t1.y))) != -1) {
        errorPosition += err;
        printErrorMessage(errorPosition, "<double> <double>", "(");
        return errorPosition;
    }
    errorPosition = (*pointer) - start;

    //Считываем запятую
    if ((err = findSymbol(pointer, ',')) != -1) {
        errorPosition += err;
        printErrorMessage(errorPosition, ",", "<double> <double>");
        return errorPosition;
    }
    errorPosition = (*pointer) - start;

    //Считываем координаты 2й точки
    if ((err = setXY(pointer, &(triangle->t2.x), &(triangle->t2.y))) != -1) {
        errorPosition += err;
        printErrorMessage(
                errorPosition, "<double> <double>", "<double> <double>");
        return errorPosition;
    }
    errorPosition = (*pointer) - start;

    //Считываем запятую
    if ((err = findSymbol(pointer, ',')) != -1) {
        errorPosition += err;
        printErrorMessage(errorPosition, ",", "<double> <double>");
        return errorPosition;
    }
    errorPosition = (*pointer) - start;

    //Считываем координаты 3й точки
    if ((err = setXY(pointer, &(triangle->t3.x), &(triangle->t3.y))) != -1) {
        errorPosition += err;
        printErrorMessage(
                errorPosition, "<double> <double>", "<double> <double>");
        return errorPosition;
    }
    errorPosition = (*pointer) - start;

    //Считываем запятую
    if ((err = findSymbol(pointer, ',')) != -1) {
        errorPosition += err;
        printErrorMessage(errorPosition, ",", "<double> <double>");
        return errorPosition;
    }
    errorPosition = (*pointer) - start;

    //Считываем координаты 4й точки отдельно и проверяем, что она соответствует
    //первой точке
    double x, y;
    if ((err = setXY(pointer, &x, &y)) != -1) {
        errorPosition += err;
        printErrorMessage(
                errorPosition, "<double> <double>", "<double> <double>");
        return errorPosition;
    }
    if ((x != triangle->t1.x) || (y != triangle->t1.y)) {
        printErrorMessage(
                errorPosition,
                "the same <double> <double> point as the first point",
                ",");
        return errorPosition;
    }
    errorPosition = (*pointer) - start;

    //Считываем скобку
    if ((err = findSymbol(pointer, ')')) != -1) {
        errorPosition += err;
        printErrorMessage(errorPosition, ")", "<double>");
        return errorPosition;
    }
    errorPosition = (*pointer) - start;
    //Считываем вторую скобку
    if ((err = findSymbol(pointer, ')')) != -1) {
        errorPosition += err;
        printErrorMessage(errorPosition, ")", "<double>");
        return errorPosition;
    }
    errorPosition = (*pointer) - start;

    //Проверяем, что после скобки ничего нет
    if ((err = checkLineEnd(pointer)) != -1) {
        errorPosition += err;
        printErrorMessage(errorPosition, ")", "ending");
        return errorPosition;
    }

    return -1;
}

void printCircle(Circle* circle, char* figureName)
{
    printf("%s(%.2lf %.2lf, %.2lf)\n"
           "  perimetr=%lf\n"
           "  area=%lf\n",
           figureName,
           circle->x,
           circle->y,
           circle->radius,
           circle->perimeter,
           circle->area);
}

void printTriangle(Triangle* triangle, char* figureName)
{
    printf("%s((%.2lf %.2lf, %.2lf %.2lf, %.2lf %.2lf, %.2lf %.2lf))\n"
           "  perimetr=%lf\n"
           "  area=%lf\n",
           figureName,
           triangle->t1.x,
           triangle->t1.y,
           triangle->t2.x,
           triangle->t2.y,
           triangle->t3.x,
           triangle->t3.y,
           triangle->t1.x,
           triangle->t1.y,
           triangle->perimeter,
           triangle->area);
}

CircleArray* createCircleArray()
{
    CircleArray* array = (CircleArray*)malloc(sizeof(CircleArray));
    array->elems = NULL;
    array->num = 0;
    return array;
}

void addCircle(CircleArray* array)
{
    array->num++;
    array->elems = realloc(array->elems, array->num * sizeof(Circle));
}

void removeCircle(CircleArray* array)
{
    if (array->num == 0) {
        return;
    }

    if (array->num == 1) {
        array->num--;
        free(array->elems);
        array->elems = NULL;
        return;
    }

    array->num--;
    array->elems = realloc(array->elems, array->num * sizeof(Circle));
}

void deleteCircleArray(CircleArray* array)
{
    if (array->elems != NULL) {
        free(array->elems);
    }
    if (array != NULL) {
        free(array);
    }
}

void printCircleArray(CircleArray* array)
{
    for (int i = 0; i < array->num; ++i) {
        printCircle(&(array->elems[i]), "Circle");
    }
}

TriangleArray* createTriangleArray()
{
    TriangleArray* array = (TriangleArray*)malloc(sizeof(TriangleArray));
    array->elems = NULL;
    array->num = 0;
    return array;
}

void addTriangle(TriangleArray* array)
{
    array->num++;
    array->elems = realloc(array->elems, array->num * sizeof(Triangle));
}

void removeTriangle(TriangleArray* array)
{
    if (array->num == 0) {
        return;
    }

    if (array->num == 1) {
        array->num--;
        free(array->elems);
        array->elems = NULL;
        return;
    }

    array->num--;
    array->elems = realloc(array->elems, array->num * sizeof(Triangle));
}

void deleteTriangleArray(TriangleArray* array)
{
    if (array->elems != NULL) {
        free(array->elems);
    }
    if (array != NULL) {
        free(array);
    }
}

void printTriangleArray(TriangleArray* array)
{
    for (int i = 0; i < array->num; ++i) {
        printTriangle(&(array->elems[i]), "Triangle");
    }
}

int isIntersectCC(Circle* c1, Circle* c2)
{
    //Окружности -- полые фигуры.
    //Будем считать пересечением наличие ровно 2 общих точек.
    //Это выполняется, если выполняется правило треугольника для радиусов и
    //расстояния

    double l = segmentLength(c1->x, c2->x, c1->y, c2->y);
    if (c1->radius + c2->radius <= l) {
        return 0;
    }

    if (c1->radius + l <= c2->radius) {
        return 0;
    }

    if (l + c2->radius <= c2->radius) {
        return 0;
    }

    return 1;
}

int isInto(double x, double left, double right)
{
    //Проверяет, принадлежит ли x интервалу (left, right)
    if (fabs(x - left) < 0.0001 || fabs(x - right) < 0.0001) {
        return 0;
    }
    if (x > left && x < right) {
        return 1;
    }
    return 0;
}

int isIntersectSegments(Point* t1, Point* t2, Point* p1, Point* p2)
{
    //Будем считать, что отрезки пересекаются, если при делении плоскости
    //прямой, проходящей через один отрезок, второй лежил в обоих
    //полуплоскостях.

    //!ПЕРЕД РАСЧЕТОМ Предусматриваем ситуацию, когда это не функция f(x) (x =
    //! const)
    int p_constflag = 0;
    int t_constflag = 0;
    if (p2->x == p1->x) {
        p_constflag = 1;
    }

    if (t2->x == t1->x) {
        t_constflag = 1;
    }

    // Уравнение прямой по двум точкам: (x - x1)/(x2 - x1) = (y - y1)/(y2 - y1)
    // После преобразований получаем: y = x*[(y2 - y1)/(x2 - x1)] + y1 - x1[(y2
    // - y1)/(x2 - x1)] "Школьный" вид уравнения:  y = kx + b Получается, что k
    // = (y2 - y1)/(x2 - x1) b = y1 - k*x1

    //Рассчёт k и b для прямых t и p:
    double k_p = 0;
    if (!p_constflag) {
        k_p = (p2->y - p1->y) / (p2->x - p1->x);
    }
    double b_p = p1->y - (p1->x * k_p);

    double k_t = 0;
    if (!t_constflag) {
        k_t = (t2->y - t1->y) / (t2->x - t1->x);
    }
    double b_t = t1->y - (t1->x * k_t);

    //Если обе линии вертикальны..
    if (t_constflag == 1 && p_constflag == 1) {
        return 0;
    }

    // Если k равны (но вертикальных линий нет),  то прямые, на которых лежат
    // отрезки, параллельны.
    if ((k_t == k_p) && !(t_constflag == 1 || p_constflag == 1)) {
        return 0;
    }

    //Если мы здесь, то прямые, на которых лежат отрезки пересекаются.
    //Надём точку пересечения.
    //В случае, если есть одна вертикальная линия, точка пересеченя по x
    //автоматически найдена
    double x = 0;
    if (p_constflag) {
        x = p1->x;
    } else if (t_constflag) {
        x = t1->x;
    } else {
        // Cистема:
        // y = x * k_t + b_t
        // y = x * k_p + b_p
        // ==>
        // x*kt + bt = x*kp + bp ==>  x = (bp - bt)/(kt - kp)
        x = (b_p - b_t) / (k_t - k_p);
    }

    //Если точка пересечения лежит внутри обоих отрезков, значит, пересекаются
    //не только прямые, но и отрезки. Вычисления достаточно провести для одной
    //координаты. Для простоты "упорядочим" x-овые координаты каждого отрезка:
    double t_left = t1->x;
    double t_right = t2->x;
    if (t_left > t_right) {
        double tmp = t_left;
        t_left = t_right;
        t_right = tmp;
    }

    double p_left = p1->x;
    double p_right = p2->x;
    if (p_left > p_right) {
        double tmp = p_left;
        p_left = p_right;
        p_right = tmp;
    }
    //Проверяем, не лежит ли точка пересечения внутри
    if (isInto(x, t_left, t_right) && isInto(x, p_left, p_right)) {
        //Пересекаются
        return 1;
    }

    return 0;
}

int isIntersectTSegment(Point* p1, Point* p2, Triangle* t)
{
    //Проверяем пересечение прямой со всеми прямыми треугольника.
    int result = 0;
    result += isIntersectSegments(p1, p2, &(t->t1), &(t->t2));
    result += isIntersectSegments(p1, p2, &(t->t2), &(t->t3));
    result += isIntersectSegments(p1, p2, &(t->t3), &(t->t1));

    return result;
}

int isIntersectTT(Triangle* triangle1, Triangle* triangle2)
{
    //Каждый треугольник -- это 3 отрезка. Если какие-нибудь из них
    //пересекаются, то и треугольники пересекаются Проверяем пересечения
    //отрезков первого треугольника со всеми отрезками второго.
    double result = 0;
    result += isIntersectTSegment(
            &(triangle1->t1), &(triangle1->t2), triangle2);
    result += isIntersectTSegment(
            &(triangle1->t2), &(triangle1->t3), triangle2);
    result += isIntersectTSegment(
            &(triangle1->t3), &(triangle1->t1), triangle2);

    return result;
}

int isIntersectCSegment(Point* p1, Point* p2, Circle* circle)
{
    // Уравнение прямой: y = kx + c
    // Уравнение окружности: (y - a)^2 + (x - b)^2 = r^2
    // Подставляем первое во второе:
    // (kx + c - a)(kx + c - a) + x^2 - 2bx + b^2 = r^2
    //
    // (kx)^2 + 2kxc - 2kxa - 2ac + c^2 + a^2 + x^2 - 2bx + b^2 = r^2
    //
    // (k^2 + 1)x^2  + 2x(kc - ka - b) + [a^2 + b^2 + c^2 - 2ac - r^2] = 0
    //
    // Получили квадратное уравнение относительно x.
    // Если у него 2 решения -- прямая, на которой лежит отрезок, пересекает
    // окружность в 2 точках.

    //Параметры прямой
    double k = (p2->y - p1->y) / (p2->x - p1->x); //см. isIntersectSegments()
    double c = p1->y - (k * p1->x);

    // Параметры окружности
    // Даны:
    // a == circle->y
    // b == circle->x
    // r == circle->radius

    // Параметры квадратного уравнения
    // Ax^2 + Bx + C = 0;
    double A = k * k + 1;
    double B = 2 * (k * c - (k * circle->y) - circle->x);
    double C = (circle->x) * (circle->x) + (circle->y) * (circle->y) + c * c
            - (circle->radius) * (circle->radius);

    double discriminant = B * B - 4 * A * C;

    if (discriminant <= 0) {
        //Пересечения нет (максимум касание)
        return 0;
    }

    //Ищем точки пересечения
    double x1 = (-1 * B + sqrt(discriminant)) / (2 * A);
    double x2 = (-1 * B - sqrt(discriminant)) / (2 * A);

    //Проверяем, принадлежит ли хоть одна из них отрезку
    // 1. Упорядочиваем концы отрезка по координате x
    double p_left = p1->x;
    double p_right = p2->x;
    if (p_left > p_right) {
        double tmp = p_left;
        p_left = p_right;
        p_right = tmp;
    }
    // 2. Смотрим
    if (isInto(x1, p_left, p_right) || isInto(x2, p_left, p_right)) {
        //Лежит
        return 1;
    }

    return 0;
}

int isIntersectTC(Triangle* triangle, Circle* circle)
{
    int result = 0;
    result += isIntersectCSegment(&(triangle->t1), &(triangle->t2), circle);
    result += isIntersectCSegment(&(triangle->t2), &(triangle->t3), circle);
    result += isIntersectCSegment(&(triangle->t3), &(triangle->t1), circle);

    return result;
}

void printAll(CircleArray* c_array, TriangleArray* t_array)
{
    int c = 0, j = 0;
    int t = 0, k = 0;
    for (c = 0; c < c_array->num; ++c) {
        //Обходим все окружности.
        printf("%d.", c);
        printCircle(&(c_array->elems[c]), "Circle");
        printf("   Intersects:\n");
        for (j = 0; j < c_array->num; ++j) {
            if (j == c) {
                continue;
            }
            if (isIntersectCC(&(c_array->elems[j]), &(c_array->elems[c]))) {
                printf("    %d. circle\n", j);
            }
        }
        for (k = 0; k < t_array->num; ++k) {
            if (isIntersectTC(&(t_array->elems[k]), &(c_array->elems[c]))) {
                printf("    %d. triangle\n", k + j);
            }
        }
        printf("-------\n");

    } // for c_array
    for (t = 0; t < t_array->num; ++t) {
        //Обходим все треугольники.
        printf("%d.", t + c);
        printTriangle(&(t_array->elems[t]), "Triangle");
        printf(" Intersects:\n");
        for (j = 0; j < c_array->num; ++j) {
            if (isIntersectTC(&(t_array->elems[t]), &(c_array->elems[j]))) {
                printf("    %d. circle\n", j);
            }
        }
        for (k = 0; k < t_array->num; ++k) {
            if (k == t) {
                continue;
            }
            if (isIntersectTT(&(t_array->elems[k]), &(t_array->elems[t]))) {
                printf("    %d. triangle\n", k + j);
            }
        }
        printf("-------\n");

    } // for t_array
}
