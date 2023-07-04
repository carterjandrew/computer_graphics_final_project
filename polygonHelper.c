#include "CSCIx229.h"
Point2d getOrthonorm(Point2d p1, Point2d p2, int loopDir){
    double dx, dy, dim;
    dy = p2.x-p1.x;
    dx = p2.y-p1.y;
    dim = sqrtf(dx*dx+dy*dy);
    dx/=-dim;
    dy/=dim;
    if(loopDir){
        dx=-dx;
        dy=-dy;
    }
    return make2dPoint(dx,dy);
}
int linesIntersect(Point2d s1, Point2d e1, Point2d s2, Point2d e2){
    float d1, d2;
    float a1, a2, b1, b2, c1, c2;

    if(e1.x == s2.x && e1.y == s2.y){
        return 0;
    }
    if(e2.x == s1.x && e2.y == s1.y){
        return 0;
    }
    a1 = e1.y - s1.y;
    b1 = s1.x - e1.x;
    c1 = (e1.x * s1.y) - (s1.x * e1.y);
    d1 = (a1 * s2.x) + (b1 * s2.y) + c1;
    d2 = (a1 * e2.x) + (b1 * e2.y) + c1;
    if (d1 > 0 && d2 > 0) return 0;
    if (d1 < 0 && d2 < 0) return 0;
    a2 = e2.y - s2.y;
    b2 = s2.x - e2.x;
    c2 = (e2.x * s2.y) - (s2.x * e2.y);
    d1 = (a2 * s1.x) + (b2 * s1.y) + c2;
    d2 = (a2 * e1.x) + (b2 * e1.y) + c2;
    // 0 intersection is possible.
    if (d1 > 0 && d2 > 0) return 0;
    if (d1 < 0 && d2 < 0) return 0;
    if ((a1 * b2) - (a2 * b1) == 0.0f) return -1;
    return 1;
}
Point2d make2dPoint(double x, double y){
    Point2d point;
    point.x = x;
    point.y = y;
    return point;
}
Point2d pointOfIntersection(Point2d s1, Point2d e1, Point2d s2, Point2d e2){
    double a1,a2,b1,b2,c1,c2,x,y;
    a1 = s1.x - e1.x;
    b1 = s1.y - e1.y;
    a2 = s2.x - e2.x;
    b2 = s2.y - e2.y;
    c1 = s1.x*e1.y - s1.y*e1.x;
    c2 = s2.x*e2.y - s2.y*e2.x;
    x = (c1*(a2) - c2*a1 )/( (a1*(b2))- ((b1)*(a2)));
    y = (c1*(b2) - c2*(b1) )/( (a1*(b2))- ((b1)*(a2)));
    return make2dPoint(x,y);
}


int raycastPolygon(Point2d start, Point2d dest, Polygon polygon){
    int collisionCount = 0;
    for(int i = 0; i < polygon.count; i++){
        if(linesIntersect(start, dest, polygon.points[i], polygon.points[(i+1)%polygon.count])) collisionCount++;
    }
    return collisionCount;
}
void insertPoint(Polygon* p, Point2d point, int index){
    for(int i = p->count-1; i >=index; i--){
        p->points[i+1] = p->points[i];
    }
    p->points[index] = point;
}
Point2d getDir(Point2d p1, Point2d p2){
    return make2dPoint(p1.x-p2.x, p1.y-p2.y);
}
void addPoint(Polygon* p, Point2d point, int index){
    for(int i = 0; i < p->count; i++){
        if(i >= index) p->points[i+1] = p->points[i];
    }
    p->points[index] = point;
    p->count++;
}
Polygon makeSpace(Point2d p1, Point2d p2, int loopDir, int depth){
    Point2d orth = getOrthonorm(p1,p2,loopDir);
    Polygon ret;
    ret.points = (Point2d*)malloc(sizeof(Point2d) * 4);
    ret.points[0] = p1;
    ret.points[1] = make2dPoint(p1.x+orth.x*depth, p1.y+orth.y*depth);
    ret.points[2] = make2dPoint(p2.x+orth.x*depth, p2.y+orth.y*depth);
    ret.points[3] = p2;
    ret.count = 4;
    return ret;
}
FloorPlan buildHouse(Polygon space, double maxRoomDim, int loopDir, int floors){
    FloorPlan plan;
    plan.spaces = malloc(sizeof* plan.spaces * floors * 5);
    for(int i = 0; i < floors; i++){
        if(1){
            int line = rand()%space.count;
            float r1 = (float)rand()/(float)(RAND_MAX);
            float r2 = (float)rand()/(float)(RAND_MAX);
            Point2d dir1 = getDir(space.points[line], space.points[(line+1)%space.count]);
            Point2d dir2 = getDir(space.points[(line+1)%space.count], space.points[(line+2)%space.count]);
            Point2d p1 = make2dPoint(space.points[line].x + (dir1.y * r1), space.points[line].y + (dir1.y * r1));
            Point2d p3 = make2dPoint(space.points[(line+1)%space.count].x + (dir2.x * r2), space.points[(line+1)%space.count].y + (dir2.y * r2));
            Point2d orth1 = getOrthonorm(space.points[line], space.points[(line+1)%space.count], loopDir);
            Point2d orth2 = getOrthonorm(space.points[(line+1)%space.count], space.points[(line+2)%space.count], loopDir);
            Point2d p2 = make2dPoint(p1.x + (orth1.x * 200), p1.y + (orth1.y * 200));
            Point2d p4 = make2dPoint(p2.x + (orth2.x * 200), p2.y + (orth2.y * 200));
            Point2d intersect = pointOfIntersection(p1,p2,p3,p4);
            plan.spaces[plan.length].count = 4;
            plan.spaces[plan.length].points = malloc(sizeof * plan.spaces[plan.length].points * 4);
            plan.spaces[plan.length].type = 'c';
            plan.spaces[plan.length].points[0] = p1;
            plan.spaces[plan.length].points[1] = space.points[(line+1)%space.count];
            plan.spaces[plan.length].points[2] = p2;
            plan.spaces[plan.length].points[3] = intersect;
            plan.length++;
        }
        plan.spaces[plan.length] = space;
        plan.spaces[plan.length].type='s';
        plan.spaces[plan.length].floor=i;
        plan.length++;
    }
    return plan;
}