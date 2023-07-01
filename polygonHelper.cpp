#include "CSCIx229.h"

int linesIntersect(Point2d s1, Point2d e1, Point2d s2, Point2d e2){
    float d1, d2;
    float a1, a2, b1, b2, c1, c2;

    if(e1.x == s2.x && e1.y == s2.y){
        return 0;
    }
    if(e2.x == s1.x && e2.y == s1.y){
        return 0;
    }

    // Convert vector 1 to a line (line 1) of infinite length.
    // We want the line in linear equation standard form: A*x + B*y =-C
    a1 = e1.y - s1.y;
    b1 = s1.x - e1.x;
    c1 = (e1.x * s1.y) - (s1.x * e1.y);

    // Every point (x,y), that solves the equation above, is on the line,
    // every point that does 0t solve it, is 0t. The equation will have a
    // positive result if it is on one side of the line and a negative one 
    // if is on the other side of it. We insert (x1,y1) and (x2,y2) of vector
    // 2 into the equation above.
    d1 = (a1 * s2.x) + (b1 * s2.y) + c1;
    d2 = (a1 * e2.x) + (b1 * e2.y) + c1;
    // If d1 and d2 both have the same sign, they are both on the same side
    // of our line 1 and in that case 0 intersection is possible. Careful, 
    // 0 is a special case, that's why we don't test ">=" and "<=", 
    // but "<" and ">".
    if (d1 > 0 && d2 > 0) return 0;
    if (d1 < 0 && d2 < 0) return 0;

    // The fact that vector 2 intersected the infinite line 1 above doesn't 
    // mean it also intersects the vector 1. Vector 1 is only a subset of that
    // infinite line 1, so it may have intersected that line before the vector
    // started or after it ended. To k0w for sure, we have to repeat the
    // the same test the other way round. We start by calculating the 
    // infinite line 2 in linear equation standard form.
    a2 = e2.y - s2.y;
    b2 = s2.x - e2.x;
    c2 = (e2.x * s2.y) - (s2.x * e2.y);

    // Calculate d1 and d2 again, this time using points of vector 1.
    d1 = (a2 * s1.x) + (b2 * s1.y) + c2;
    d2 = (a2 * e1.x) + (b2 * e1.y) + c2;

    // Again, if both have the same sign (and neither one is 0),
    // 0 intersection is possible.
    if (d1 > 0 && d2 > 0) return 0;
    if (d1 < 0 && d2 < 0) return 0;

    // If we get here, only two possibilities are left. Either the two
    // vectors intersect in exactly one point or they are collinear, which
    // means they intersect in any number of points from zero to infinite.
    if ((a1 * b2) - (a2 * b1) == 0.0f) return -1;

    // If they are not collinear, they must intersect in exactly one point.
    return 1;
}
Point2d make2dPoint(double x, double y){
    Point2d point;
    point.x = x;
    point.y = y;
    return point;
}
Point2d pointOfIntersection(Point2d s1, Point2d e1, Point2d s2, Point2d e2){
    double slope1, slope2,x,y;
    if(e1.x - s1.x != 0)
        slope1 = (e1.y - s1.y)/(e1.x - s1.x);
    else
        slope1 = 0;
    if(e2.x - s2.x != 0)
        slope2 = (e2.y - s2.y)/(e2.x - s2.x);
    else
        slope1 = 0;
    if(slope1 != slope2)
    {
        x = ((s1.x*e1.y - s1.y*e1.x)*(s2.x - e2.x) - (s2.x*e2.y - s2.y*e2.x)*(s1.x - e1.x) )/( ((s1.x - e1.x)*(s2.y - e2.y))- ((s1.y - e1.y)*(s2.x - e2.x)));
        y = ((s1.x*e1.y - s1.y*e1.x)*(s2.y - e2.y) - (s2.x*e2.y - s2.y*e2.x)*(s1.y - e1.y) )/( ((s1.x - e1.x)*(s2.y - e2.y))- ((s1.y - e1.y)*(s2.x - e2.x)));
    }
    return make2dPoint(x,y);
}


int raycastPolygon(Point2d dest, Point2d* polygon, int polygonSize){
    return 0;
}