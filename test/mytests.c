#include "ctest.h"
#include <geom.h>
#include <stdlib.h>

CTEST(suite1, isinto)
{
    CTEST_LOG("checking isInto() function\n");
    ASSERT_EQUAL(1, isInto(0, -1, 1));
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

CTEST(suite3, findword)
{
}
