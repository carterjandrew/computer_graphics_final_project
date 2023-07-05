#ifndef CSCIx229
#define CSCIx229

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <float.h>

// GLEW _MUST_ be included first
#ifdef USEGLEW
#include <GL/glew.h>
#endif
//  Get all GL prototypes
#define GL_GLEXT_PROTOTYPES
//  Select SDL, SDL2, GLFW or GLUT
#if defined(SDL2)
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#elif defined(SDL)
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#elif defined(GLFW)
#include <GLFW/glfw3.h>
#elif defined(__APPLE__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
//  Make sure GLU and GL are included
#ifdef __APPLE__
#include <OpenGL/glu.h>
#include <OpenGL/gl.h>
// Tell Xcode IDE to not gripe about OpenGL deprecation
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#else
#include <GL/glu.h>
#include <GL/gl.h>
#endif
//  Default resolution
//  For Retina displays compile with -DRES=2
#ifndef RES
#define RES 1
#endif

//  cos and sin in degrees
#define Cos(th) cos(3.14159265/180*(th))
#define Sin(th) sin(3.14159265/180*(th))

#ifdef __cplusplus
extern "C" {
#endif

void Print(const char* format , ...);
void Fatal(const char* format , ...);
unsigned int LoadTexBMP(const char* file);
void Project(double fov,double asp,double dim);
void ErrCheck(const char* where);
int  LoadOBJ(const char* file);
typedef struct {double x,y,z;} Point3d;
typedef struct {double x,y;} Point2d;
typedef struct {int count; char type; int floor; Point2d* points;} Polygon;
typedef struct {Polygon polygon; int expansionDir;} Wall;
typedef struct {Point2d start, end;} Line;
typedef struct {Polygon* spaces; int length;} FloorPlan;
Point2d make2dPoint(double x, double y);
Point2d getOrthonorm(Point2d p1, Point2d p2, int loopDir);
FloorPlan buildHouse(Polygon space, double maxRoomDim, int loopDir, int floors);
Polygon buildRoom(Polygon cutout, int line, float r1, float r2, int loopDir);
int raycastPolygon(Point2d start, Point2d dest, Polygon polygon);
int linesIntersect(Point2d s1, Point2d e1, Point2d s2, Point2d e2);
Point2d pointOfIntersection(Point2d s1, Point2d e1, Point2d s2, Point2d e2);
#ifdef __cplusplus
}
#endif

#endif
