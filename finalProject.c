#include "CSCIx229.h"
#include <time.h>
int width  = 1; //  Window width
int height = 1; //  Window height
double asp = 1; //  Window aspect ratio

#define NMIN  3 //  Minimum number of vertecies to render house
#define NMAX  8 //  Maximum number of vertecies to render house
#define PLOTMIN 20 // Minimum land plot size
#define PLOTMAX 70 // Maxium land plot size

int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int move=-1;    //  Point2d getting moved
Polygon P;      //  To store the base shape
FloorPlan FP;   //  To store the currently generated house
int loopDir = 0;
int validShape = 0;
int floors = 3;
//Display mode 0: edit plot of house
//Display mode 1: View house from exterior
//Display mode 2: First person view mode
//Display mode 4: Outline mode
int mode=0;     //  Display mode
int dimMin = 20;//  Minimum plot size
int dimMax = 200;// Maxiumum plot size
int dim = 20;     //  Scale of world
int invalidGeneration = 1;
//3D House construction components



//Lighting attributes
int light=1;    //  Lighting
int units=1;    //  Scaling unit
int distance=10;//  Radius of light orbit
int inc=10;     //  Incriment of ball movement
int smooth=1;   //  Smooth shading switch
int emission=0; //  Emission intensity(%)
int ambient=10;  // Ambient intensity (%)
int diffuse=50;  // Diffuse intensity (%)
int specular=0;  // Specular intensity (%)
int shininess=0;  // Shininess (power of two)
float shiny=1;  // Shininess (value)
int zh=90;  // Light azimuth
float lr=1; //Light orbit radius
float ylight=10;  // Elevation of light

static void ball(double x,double y,double z,double r)
{
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball
   glColor3f(1,1,1);
   glutSolidSphere(1.0,16,16);
   //  Undo transofrmations
   glPopMatrix();
}

/*
 *  Translate mouse (x,y) to world coordinates
 */
Point2d Mouse2World(int x,int y)
{
   Point2d p;
   p.x = dim*(2*asp*x       /(float)(width -1) - asp);
   p.y = dim*(2*(height-1-y)/(float)(height-1) - 1);
   return p;
}
/*
 *  Distance to Point2d
 */
double Distance(Point2d p,int k)
{
   double dx = p.x - P.points[k].x;
   double dy = p.y - P.points[k].y;
   return sqrt(dx*dx+dy*dy);
}
/*
 *  GLUT calls this routine when a mouse is moved
 */
void motion(int x,int y)
{
   if (move<0) return;
   //  Update Point2d
   P.points[move] = Mouse2World(x,y);
   //  Redisplay
   glutPostRedisplay();
}
/*
*   Glut mouse onclick event
*/
void mouse(int button, int state, int x, int y){
    if(mode == 0){
        Point2d p = Mouse2World(x,y);
        //Add a Point2d or edit close Point2d
        if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
            int nodeToEdit = -1;
            double dmin = .2*dim;
            for(int i = 0; i < P.count; i++){
                double dist = Distance(p,i);
                if(dist < dmin){
                    nodeToEdit = i;
                    dmin = dist;
                }
            }
            if(P.count < NMAX && nodeToEdit == -1){
                P.points[P.count] = p;
                move = P.count;
                P.count++;
            }else if(nodeToEdit != -1){
                move = nodeToEdit;
            }
        }else if(button == GLUT_RIGHT_BUTTON && state == GLUT_UP){
            int nodeToDelete = -1;
            double dmin = .05*dim;
            for(int i = 0; i < P.count; i++){
                double dist = Distance(p,i);
                if(dist < dmin){
                    nodeToDelete = i;
                    dmin = dist;
                }
            }
            if(nodeToDelete != -1){
                for(int i = nodeToDelete; i < P.count-1; i++){
                    P.points[i] = P.points[i+1];
                }
                P.count--;
            }
        }else if(button == GLUT_LEFT_BUTTON && state == GLUT_UP){
            P.points[move] = p;
            move = -1;
        }
        glutPostRedisplay();
    }
}
void special(int key, int x, int y){
    if(mode == 0){
        switch (key)
        {
        case GLUT_KEY_PAGE_DOWN:
            if(dim-1 >= PLOTMIN)dim--;
            Project(0,asp,dim);
            break;
        case GLUT_KEY_PAGE_UP:
            if(dim+1 < PLOTMAX)dim++;
            Project(0,asp,dim);
        default:
            break;
        }
    }
    //  Right arrow key - increase angle by 5 degrees
    if (key == GLUT_KEY_RIGHT)
        th += 5;
    //  Left arrow key - decrease angle by 5 degrees
    else if (key == GLUT_KEY_LEFT)
        th -= 5;
    //  Up arrow key - increase elevation by 5 degrees
    else if (key == GLUT_KEY_UP)
        ph += 5;
    //  Down arrow key - decrease elevation by 5 degrees
    else if (key == GLUT_KEY_DOWN)
        ph -= 5;
    //  PageUp key - increase dim
    else if (key == GLUT_KEY_PAGE_UP)
        dim += 0.1;
    //  PageDown key - decrease dim
    else if (key == GLUT_KEY_PAGE_DOWN && dim>1)
        dim -= 0.1;
    //  Keep angles to +/-360 degrees
    th %= 360;
    ph %= 360;
    glutPostRedisplay();
}
void key(unsigned char ch, int x, int y){
    if(mode == 0){
        switch (ch)
        {
        case 'd':
           P.count = 0;
            break;
        case 13:
            mode = (mode+1)%3;
            if(mode == 1){
                if(!validShape){
                    mode = 0;
                    break;
                }
                FP = buildHouse(P,10,loopDir,floors);
                Project(45,asp,dim*2);
                lr=dim*1.2;
                ylight = floors*5;
            }
        default:
            break;
        }
    }else if(mode == 1){

    }else if(mode == 2){

    }
    glutPostRedisplay();
}
/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int w,int h)
{
   //  Remember dimensions
   width  = w;
   height = h;
   //  Ratio of the width to the height of the window
   asp = (h>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, RES*width,RES*height);
   //  Project
   Project(0,asp,dim);
}
void idle(){
    double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
    zh = fmod(90*t,360.0);
    glutPostRedisplay();
}
void display(){
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glLoadIdentity();
    if(mode==0){
        glRotatef(-90,1,0,0);
        validShape = 1;
        if(P.count < 3) validShape = 0;
        glColor3f(1,0,0);
        for(int i = 0; i < P.count; i++){
            for(int j = i; j < P.count; j++){
                if(i==j) continue;
                if(linesIntersect(P.points[i],P.points[(i+1)%P.count],P.points[j],P.points[(j+1)%P.count])){ 
                    validShape = 0;
                    glPointSize(9);
                    Point2d intersection = pointOfIntersection(P.points[i%P.count],P.points[(i+1)%P.count],P.points[(j%P.count)],P.points[(j+1)%P.count]);
                    glBegin(GL_POINTS);
                    glVertex3f(intersection.x,0,intersection.y);
                    glEnd();
                    glPointSize(1);
                }
            }
        }
        if(validShape) glColor3f(0,1,0);
        //Draw in and test the direction of the polygon loop
        if(P.count > 1){
            Point2d orth = getOrthonorm(P.points[0],P.points[1], loopDir);
            Point2d lineMean = make2dPoint((P.points[0].x + P.points[1].x)/2,(P.points[0].y + P.points[1].y)/2);
            Point2d testPoint = make2dPoint(orth.x + lineMean.x, orth.y + lineMean.y);
            if(raycastPolygon(make2dPoint(100,100), testPoint, P)%2==0)loopDir = !loopDir;
        }
        glBegin(GL_LINES);
        for(int i = 0; i < P.count; i++){
            Point2d orth = getOrthonorm(P.points[i],P.points[(i+1)%P.count], loopDir);
            Point2d lineMean = make2dPoint((P.points[i].x + P.points[(i+1)%P.count].x)/2,(P.points[i].y + P.points[(i+1)%P.count].y)/2);
            Point2d testPoint = make2dPoint(orth.x + lineMean.x, orth.y + lineMean.y);
            glVertex3d(lineMean.x,0,lineMean.y);
            glVertex3d(testPoint.x,0,testPoint.y);
        }
        glEnd();
        glBegin(GL_LINE_LOOP);
        for(int i = 0; i < P.count; i++){
            glVertex3d(P.points[i].x,0 ,P.points[i].y);
        }
        glEnd();
        glColor3f(1,1,1);
        glPointSize(7);
        glBegin(GL_POINTS);
        for(int i = 0; i < P.count; i++){
            glVertex3d(P.points[i].x, 0, P.points[i].y);
        }
        glEnd();
        glColor3f(0,0,1);
        glBegin(GL_LINES);
        for(int p = 0; p < FP.length; p++){
            for(int i = 0; i < FP.spaces[p].count; i++){
                glVertex3d(FP.spaces[p].points[i].x,0,FP.spaces[p].points[i].y);
                glVertex3d(FP.spaces[p].points[(i+1)%FP.spaces[p].count].x,0,FP.spaces[p].points[(i+1)%FP.spaces[p].count].y);
            }
        }
        glEnd();
        glPointSize(1);
    }
    else if(mode == 1){
        double Ex = -4*dim*Sin(th)*Cos(ph);
        double Ey = +4*dim        *Sin(ph);
        double Ez = +4*dim*Cos(th)*Cos(ph);
        gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
        //  Translate intensity to color vectors
        float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
        float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
        float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
        //  Light direction
        float Position[]  = {lr*Cos(zh), ylight, lr*Sin(zh),1};
        //  Draw light position as ball (still no lighting here)
        glColor3f(1,1,1);
        ball(Position[0],Position[1],Position[2] , 1);
        //  OpenGL should normalize normal vectors
        glEnable(GL_NORMALIZE);
        //  Enable lighting
        glEnable(GL_LIGHTING);
        //  glColor sets ambient and diffuse color materials
        glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);
        //  Enable light 0
        glEnable(GL_LIGHT0);
        //  Set ambient, diffuse, specular components and position of light 0
        glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
        glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
        glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
        glLightfv(GL_LIGHT0,GL_POSITION,Position);
        glColor3f(1,1,1);
        glBegin(GL_QUADS);
        for(int i = 0; i < FP.length; i++){
            for(int p = 0; p < FP.spaces[i].count; p++){
                if(FP.spaces[i].type == 's'){   
                    Point2d normal = getOrthonorm(FP.spaces[i].points[p], FP.spaces[i].points[(p+1)%FP.spaces[i].count], !loopDir);
                    glNormal3f(normal.x,0,normal.y);
                    glVertex3d(FP.spaces[i].points[p].x, FP.spaces[i].floor*5, FP.spaces[i].points[p].y);
                    glVertex3d(FP.spaces[i].points[p].x, 5+FP.spaces[i].floor*5, FP.spaces[i].points[p].y);
                    glVertex3d(FP.spaces[i].points[(p+1)%FP.spaces[i].count].x, 5+FP.spaces[i].floor*5, FP.spaces[i].points[(p+1)%FP.spaces[i].count].y);
                    glVertex3d(FP.spaces[i].points[(p+1)%FP.spaces[i].count].x, FP.spaces[i].floor*5, FP.spaces[i].points[(p+1)%FP.spaces[i].count].y);
                }
            }
        }
        glEnd();
        glDisable(GL_LIGHTING);
    }
    glColor3f(.3,.3,.3);
    glBegin(GL_LINES);
    for(int i=-dim;i<=dim;i++){
        glVertex3f(i,-.01,-dim);
        glVertex3f(i,-.01,dim);
        glVertex3f(-dim,-.01,i);
        glVertex3f(dim,-.01,i);
    }
    glEnd();
    ErrCheck("display");
    glFlush();
    glutSwapBuffers();
}
int main(int argc, char *argv[]){
    srand(time(NULL));
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(600,600);
    glutCreateWindow("Carter Andrew | Final Project | Procedural House Generation");
#ifdef USEGLEW
   //  Initialize GLEW
   if (glewInit()!=GLEW_OK) Fatal("Error initializing GLEW\n");
#endif
    Point2d mappingPoints[200];
    P.points = (Point2d*) &mappingPoints;
    //  Set callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(key);
    glutSpecialFunc(special);
    glutIdleFunc(idle);
    //  Pass control to GLUT so it can interact with the user
    ErrCheck("init");
    glutMainLoop();
    return 0;
}
