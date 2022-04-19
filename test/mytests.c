#include "ctest.h"
#include <geom.h>
#include <math.h>
#include <stdlib.h>

CTEST(suite1, isinto)
{
    CTEST_LOG("checking isInto() function\n");
    ASSERT_EQUAL(1, isInto(0, -1, -1));
    ASSERT_EQUAL(0, isInto(-1, 1, 1));
    ASSERT_EQUAL(0, isInto(1, -1, 1));
    ASSERT_EQUAL(0, isInto(3, -1, 1));
    ASSERT_EQUAL(0, isInto(-50, -1, 1));

    ASSERT_EQUAL(0, isInto(1, 1, 1));
    ASSERT_EQUAL(0, isInto(1.2, 1.2, 2.2));
    ASSERT_EQUAL(0, isInto(2.2, 1.2, 2.2));
    ASSERT_EQUAL(1, isInto(1.21, 1.2, 2.2));
}

CTEST(suite1, segments)
{
    CTEST_LOG("checking isIntersectSegments() function..");

    Point t1 = {-1, -1};
    Point t2 = {1, 1};
    Point p1 = {-1, 1};
    Point p2 = {1, -1};

    ASSERT_EQUAL(1, isIntersectSegments(&t1, &t2, &p1, &p2));

    p1.x = 1;
    p1.y = -1;
    p2.x = 1;
    p2.y = 1;

    ASSERT_EQUAL(0, isIntersectSegments(&t1, &t2, &p1, &p2));

    p1.x = 1;
    p1.y = -1;
    p2.x = 0;
    p2.y = 0;

    ASSERT_EQUAL(0, isIntersectSegments(&t1, &t2, &p1, &p2));

    p1.x = 1;
    p1.y = -1;
    p2.x = 0;
    p2.y = 0.01;

    ASSERT_EQUAL(1, isIntersectSegments(&t1, &t2, &p1, &p2));

    p1.x = -1;
    p1.y = 1;
    p2.x = -1;
    p2.y = -1;

    ASSERT_EQUAL(0, isIntersectSegments(&t1, &t2, &p1, &p2));

    p1.x = 1;
    p1.y = -1;
    p2.x = 0;
    p2.y = 0;

    ASSERT_EQUAL(0, isIntersectSegments(&t1, &t2, &p1, &p2));

    p1.x = 1;
    p1.y = -1;
    p2.x = -0.01;
    p2.y = 0;

    ASSERT_EQUAL(1, isIntersectSegments(&t1, &t2, &p1, &p2));

    p1.x = -2;
    p1.y = 0;
    p2.x = 0;
    p2.y = -2;

    ASSERT_EQUAL(0, isIntersectSegments(&t1, &t2, &p1, &p2));

    p1.x = 2;
    p1.y = 0;
    p2.x = 0;
    p2.y = 2;

    ASSERT_EQUAL(0, isIntersectSegments(&t1, &t2, &p1, &p2));

    p1.x = 0;
    p1.y = 2;
    p2.x = 2;
    p2.y = -0.2;

    ASSERT_EQUAL(1, isIntersectSegments(&t1, &t2, &p1, &p2));

    p1.x = -1;
    p1.y = -1;
    p2.x = 1;
    p2.y = 1;

    ASSERT_EQUAL(0, isIntersectSegments(&t1, &t2, &p1, &p2));

    p1.x = -1;
    p1.y = -1;
    p2.x = 0;
    p2.y = 0;

    ASSERT_EQUAL(0, isIntersectSegments(&t1, &t2, &p1, &p2));

    p1.x = 3;
    p1.y = 3;
    p2.x = 3;
    p2.y = 6;

    ASSERT_EQUAL(0, isIntersectSegments(&t1, &t2, &p1, &p2));

    CTEST_LOG("checking problem cases..\n");

    p1.x = -2;
    p1.y = -1;
    p2.x = 1;
    p2.y = 1;
    ASSERT_EQUAL(0, isIntersectSegments(&t1, &t2, &p1, &p2));

    p1.x = -1;
    p1.y = 1;
    p2.x = -1;
    p2.y = -1;
    ASSERT_EQUAL(0, isIntersectSegments(&t1, &t2, &p1, &p2));

    p1.x = -1;
    p1.y = 1;
    p2.x = -1;
    p2.y = -1;
    t1.x = -1;
    t1.y = -1;
    t2.x = -1;
    t2.y = -2;
    ASSERT_EQUAL(0, isIntersectSegments(&t1, &t2, &p1, &p2));

    p1.x = -1;
    p1.y = -1;
    p2.x = 1;
    p2.y = -1;
    t1.x = 1;
    t1.y = -1;
    t2.x = 2;
    t2.y = -1;
    ASSERT_EQUAL(0, isIntersectSegments(&t1, &t2, &p1, &p2));
}

CTEST(suite1, tsegment)
{
    CTEST_LOG("checking isIntersectTSegment() function..");

    Triangle t;
    t.t1.x = -1;
    t.t1.y = -1;
    t.t2.x = 1;
    t.t2.y = 1;
    t.t3.x = 1;
    t.t3.y = -1;

    Point p1;
    Point p2;

    CTEST_LOG("checking line matching..");

    p1.x = 1;
    p1.y = -1;
    p2.x = 1;
    p2.y = 2;
    ASSERT_EQUAL(0, isIntersectTSegment(&p1, &p2, &t));

    p1.x = 1;
    p1.y = -1;
    p2.x = -1;
    p2.y = -1;
    ASSERT_EQUAL(0, isIntersectTSegment(&p1, &p2, &t));

    p1.x = 1;
    p1.y = -1;
    p2.x = 1;
    p2.y = 1;
    ASSERT_EQUAL(0, isIntersectTSegment(&p1, &p2, &t));

    CTEST_LOG("checking touchings..");
    p1.x = 1;
    p1.y = -1;
    p2.x = 0;
    p2.y = 0;
    ASSERT_EQUAL(0, isIntersectTSegment(&p1, &p2, &t));

    p1.x = 1;
    p1.y = -1;
    p2.x = 0.5;
    p2.y = -0.5;
    ASSERT_EQUAL(0, isIntersectTSegment(&p1, &p2, &t));

    p1.x = 0;
    p1.y = -2;
    p2.x = 2;
    p2.y = 0;
    ASSERT_EQUAL(0, isIntersectTSegment(&p1, &p2, &t));

    p1.x = 0;
    p1.y = 1;
    p2.x = 1;
    p2.y = 1;
    ASSERT_EQUAL(0, isIntersectTSegment(&p1, &p2, &t));

    p1.x = 0;
    p1.y = 1;
    p2.x = -1;
    p2.y = -1;
    ASSERT_EQUAL(0, isIntersectTSegment(&p1, &p2, &t));

    p1.x = 1;
    p1.y = 1;
    p2.x = -2;
    p2.y = -1;
    ASSERT_EQUAL(0, isIntersectTSegment(&p1, &p2, &t));

    CTEST_LOG("checking intersections..\n");

    p1.x = 1;
    p1.y = -1;
    p2.x = -1;
    p2.y = 1;
    ASSERT_EQUAL(1, isIntersectTSegment(&p1, &p2, &t));

    p1.x = -1;
    p1.y = 1;
    p2.x = 1;
    p2.y = 0;
    ASSERT_EQUAL(1, isIntersectTSegment(&p1, &p2, &t));

    p1.x = -1;
    p1.y = 1;
    p2.x = 0.5;
    p2.y = -0.5;
    ASSERT_EQUAL(1, isIntersectTSegment(&p1, &p2, &t));
}

CTEST(suite1, csegment)
{
    CTEST_LOG("checking isIntersectionCSegment() function..");

    Circle c;
    c.x = 0;
    c.y = 0;
    c.radius = 1;

    Point p1;
    Point p2;

    CTEST_LOG("checking touchings..");
    p1.x = -2;
    p1.y = 1;
    p2.x = 1;
    p2.y = 1;
    ASSERT_EQUAL(0, isIntersectCSegment(&p1, &p2, &c));

    p1.x = 0;
    p1.y = 1;
    p2.x = 1;
    p2.y = 0;
    ASSERT_EQUAL(0, isIntersectCSegment(&p1, &p2, &c));

    CTEST_LOG("checking intersections..\n");

    p1.x = -2;
    p1.y = 1;
    p2.x = 1;
    p2.y = -1;
    ASSERT_EQUAL(1, isIntersectCSegment(&p1, &p2, &c));

    p1.x = 0;
    p1.y = 0;
    p2.x = 1;
    p2.y = -1;
    ASSERT_EQUAL(1, isIntersectCSegment(&p1, &p2, &c));
}

CTEST(suite1, tt)
{
    CTEST_LOG("checking isIntersectTT() function..");

    Triangle t;
    t.t1.x = -1;
    t.t1.y = -1;
    t.t2.x = 1;
    t.t2.y = 1;
    t.t3.x = 1;
    t.t3.y = -1;

    Triangle p;

    CTEST_LOG("checking line matching..");

    p.t1.x = -1;
    p.t1.y = -1;
    p.t2.x = 1;
    p.t2.y = 1;
    p.t3.x = -1;
    p.t3.y = 1;
    ASSERT_EQUAL(0, isIntersectTT(&t, &p));

    CTEST_LOG("checking touchings..");

    p.t1.x = 2;
    p.t1.y = -1;
    p.t2.x = 1;
    p.t2.y = -2;
    p.t3.x = 1;
    p.t3.y = -1;
    ASSERT_FALSE(isIntersectTT(&t, &p));

    p.t1.x = -2;
    p.t1.y = -1;
    p.t2.x = 1;
    p.t2.y = 2;
    p.t3.x = -1;
    p.t3.y = 1;
    ASSERT_EQUAL(0, isIntersectTT(&t, &p));

    CTEST_LOG("checking intersection..\n");

    p.t1.x = 2;
    p.t1.y = -1;
    p.t2.x = 1;
    p.t2.y = -2;
    p.t3.x = -1;
    p.t3.y = 1;
    ASSERT_TRUE(isIntersectTT(&t, &p));
}

CTEST(suite1, tc)
{
    CTEST_LOG("checking isIntersectTC() function..");

    Triangle t;
    t.t1.x = -1;
    t.t1.y = -1;
    t.t2.x = 1;
    t.t2.y = 1;
    t.t3.x = 1;
    t.t3.y = -1;

    Circle c;
    c.radius = 1;

    CTEST_LOG("checking touching..");
    c.x = 2;
    c.y = 0;
    c.radius = 1;
    ASSERT_FALSE(isIntersectTC(&t, &c));

    CTEST_LOG("checking intersection..\n");
    c.x = 0;
    c.y = 0;
    ASSERT_TRUE(isIntersectTC(&t, &c));
}

CTEST(suite1, cc)
{
    CTEST_LOG("checking isIntersectCC() function..");

    Circle c1, c2;
    CTEST_LOG("checking isIntersectCC() function..");
    c1.x = 0;
    c1.y = 0;
    c1.radius = 1;

    CTEST_LOG("checking touching..");
    c2.x = 2;
    c2.y = 0;
    c2.radius = 1;
    ASSERT_FALSE(isIntersectCC(&c1, &c2));

    CTEST_LOG("checking concentration..");
    c2.x = 0;
    c2.y = 0;
    c2.radius = 2;
    ASSERT_FALSE(isIntersectCC(&c1, &c2));

    CTEST_LOG("checking intersection..\n");
    c2.x = 1;
    c2.y = 1;
    c2.radius = 1;
    ASSERT_TRUE(isIntersectCC(&c1, &c2));
}

CTEST(suite2, tolowercase)
{
    CTEST_LOG("checking lower case tranformation..\n");
    char str[100] = "LoReM iPsuM doLor sIt aMet    1234567890 ,;.!!!";
    toLowerCase(str, 100);
    ASSERT_STR("lorem ipsum dolor sit amet    1234567890 ,;.!!!", str);
}

CTEST(suite3, createtest)
{
    CTEST_LOG("checking create & initializing arrays functions..");
    CircleArray* c_array = createCircleArray();
    TriangleArray* t_array = createTriangleArray();

    ASSERT_NULL(c_array->elems);
    ASSERT_NULL(t_array->elems);

    ASSERT_EQUAL(0, c_array->num);
    ASSERT_EQUAL(0, t_array->num);

    CTEST_LOG("checking adding and removing figures functions..");

    addCircle(c_array);
    addCircle(c_array);
    ASSERT_EQUAL(2, c_array->num);

    addTriangle(t_array);
    addTriangle(t_array);
    ASSERT_EQUAL(2, t_array->num);

    removeCircle(c_array);
    ASSERT_EQUAL(1, c_array->num);
    removeCircle(c_array);
    removeCircle(c_array);
    ASSERT_NULL(c_array->elems);
    ASSERT_EQUAL(0, c_array->num);
    addCircle(c_array);
    ASSERT_EQUAL(1, c_array->num);

    removeTriangle(t_array);
    removeTriangle(t_array);
    ASSERT_NULL(t_array->elems);
    ASSERT_EQUAL(0, t_array->num);

    CTEST_LOG("checking deleting structure functions..\n");

    deleteTriangleArray(t_array);
    deleteCircleArray(c_array);
}

CTEST(suite4, segmentlength)
{
    CTEST_LOG("checking segmentLength() function..\n");

    double x1 = 0, y1 = 0;
    double x2 = 0, y2 = 1;
    ASSERT_DBL_NEAR(1, segmentLength(x1, y1, x2, y2));

    x1 = 0;
    y1 = 0;
    x2 = 1;
    y2 = 1;
    ASSERT_DBL_NEAR(sqrt(2), segmentLength(x1, x2, y1, y2));
}

CTEST(suite4, circlePerimeter)
{
    CTEST_LOG("checking circlePerimeter() function..\n");

    double r = 0;
    ASSERT_DBL_NEAR(0, circlePerimeter(r));

    r = 1;
    ASSERT_DBL_NEAR(6.2832, circlePerimeter(r));
}

CTEST(suite4, trianglePerimeter)
{
    CTEST_LOG("checking trianglePerimeter() function..\n");
    double a = 0, b = 0, c = 0;

    ASSERT_DBL_NEAR(0, trianglePerimeter(a, b, c));

    a = 0;
    b = 1;
    c = 0;

    ASSERT_DBL_NEAR(1, trianglePerimeter(a, b, c));

    a = 3;
    b = 4;
    c = 5;

    ASSERT_DBL_NEAR(12, trianglePerimeter(a, b, c));
}

CTEST(suite4, circleArea)
{
    CTEST_LOG("checking circleArea() function..\n");

    double r = 0;

    ASSERT_DBL_NEAR(0, circleArea(r));

    r = 1;
    ASSERT_DBL_NEAR(3.1416, circleArea(r));
}

CTEST(suite4, triangleArea)
{
    CTEST_LOG("checking circleArea() function..\n");

    double a, b, c;

    a = 0;
    b = 0;
    c = 0;
    ASSERT_DBL_NEAR(0, triangleArea(a, b, c));

    a = 3;
    b = 4;
    c = 5;
    ASSERT_DBL_NEAR(6, triangleArea(a, b, c));
}

CTEST(suite4, calculatefunctions)
{
    CTEST_LOG("checking calculate functions..");

    CTEST_LOG("checking calculateTriangle() function..");

    Triangle t;

    t.t1.x = 0;
    t.t1.y = 4;
    t.t2.x = 3;
    t.t2.y = 0;
    t.t3.x = 0;
    t.t3.y = 0;

    calculateTriangle(&t);
    ASSERT_DBL_NEAR(12, t.perimeter);
    ASSERT_DBL_NEAR(6, t.area);

    CTEST_LOG("checking calculateCircle() function..\n");

    Circle c;

    c.radius = 1;
    calculateCircle(&c);

    ASSERT_DBL_NEAR(6.2832, c.perimeter);
    ASSERT_DBL_NEAR(3.1416, c.area);
}

CTEST(suite5, lineend)
{
    CTEST_LOG("checking checkLineEnd() function..\n");
    char* space_str = "                     ";
    char* empty_str = "";
    char* muddy_str = "       ,";

    ASSERT_EQUAL(-1, checkLineEnd(&space_str));
    ASSERT_EQUAL(-1, checkLineEnd(&empty_str));
    ASSERT_NOT_EQUAL(-1, checkLineEnd(&muddy_str));
}

CTEST(suite5, notspaces)
{
    CTEST_LOG("checking notOnlySpace() function..\n");
    char* str = "     five spaces before";

    ASSERT_EQUAL(-1, notOnlySpaces(str, str + 5));
    ASSERT_NOT_EQUAL(-1, notOnlySpaces(str, str + 6));
}

CTEST(suite5, setdouble)
{
    CTEST_LOG("checking setDouble() function..\n");

    char* good = "3.54";
    char* bad = " ,3.54";

    double d;

    ASSERT_EQUAL(-1, setDouble(&good, &d));
    ASSERT_DBL_NEAR(3.54, d);

    ASSERT_NOT_EQUAL(-1, setDouble(&bad, &d));
}

CTEST(suite5, setxy)
{
    CTEST_LOG("checking setXY() function..\n");
    char* good = "  10 3";
    char* bad1 = " 10  ";
    char* bad2 = " 10 asdfa -3";

    double x, y;

    ASSERT_EQUAL(-1, setXY(&good, &x, &y));
    ASSERT_DBL_NEAR(10, x);
    ASSERT_DBL_NEAR(3, y);

    ASSERT_NOT_EQUAL(-1, setXY(&bad1, &x, &y));

    ASSERT_NOT_EQUAL(-1, setXY(&bad2, &x, &y));
}

CTEST(suite5, setradius)
{
    CTEST_LOG("checking setRadius() function..\n");

    char* good = "3.54";
    char* bad1 = " ,3.54";
    char* bad2 = "-3.5";

    double d;

    ASSERT_EQUAL(-1, setRadius(&good, &d));
    ASSERT_DBL_NEAR(3.54, d);

    ASSERT_NOT_EQUAL(-1, setRadius(&bad1, &d));
    ASSERT_NOT_EQUAL(-1, setRadius(&bad2, &d));
}

CTEST(suite6, findword)
{
    CTEST_LOG("checking findWord() function..\n");

    char* str = "  WORD we will wind this word)";
    char* str2 = " we will find this word: WORD";
    char word[5];

    ASSERT_EQUAL(-1, findWord(&str, word, "WORD"));
    ASSERT_STR("WORD", word);
    ASSERT_EQUAL(-1, findWord(&str, word, "we"));
    ASSERT_STR("we", word);

    ASSERT_NOT_EQUAL(-1, findWord(&str2, word, "WORD"));
}

CTEST(suite6, findsimbol)
{
    CTEST_LOG("checking findSymbol() function..\n");

    char* str = "  ( we will wind this symbol (()";
    ASSERT_EQUAL(-1, findSymbol(&str, '('));
    ASSERT_EQUAL(-1, findSymbol(&str, 'w'));

    ASSERT_NOT_EQUAL(-1, findSymbol(&str, 'l'));
}
