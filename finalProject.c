#include "CSCIx229.h"
int width  = 1; //  Window width
int height = 1; //  Window height
double asp = 1; //  Window aspect ratio

#define NMIN  3 //  Minimum number of vertecies to render house
#define NMAX  8 //  Maximum number of vertecies to render house
int n=0;        //  Number of nodes making up land plot
int move=-1;    //  Point getting moved
typedef struct {double x,y,z;} Point;
Point P[NMAX];  //  Data points
//Display mode 0: edit plot of house
//Display mode 1: View house from exterior
//Display mode 2: First person view mode
//Display mode 4: Outline mode
int mode=0;     //  Display mode
int dimMin = 20;//  Minimum plot size
int dimMax = 200;// Maxiumum plot size
int dim = 20;     //  Scale of world


/*
 *  Translate mouse (x,y) to world coordinates
 */
Point Mouse2World(int x,int y)
{
   Point p;
   p.x = dim*(2*asp*x       /(float)(width -1) - asp);
   p.z = dim*(2*(height-1-y)/(float)(height-1) - 1);
   p.y = 0;
   return p;
}
/*
 *  Distance to point
 */
double Distance(Point p,int k)
{
   double dx = p.x - P[k].x;
   double dy = p.y - P[k].y;
   double dz = p.z - P[k].z;
   return sqrt(dx*dx+dy*dy+dz*dz);
}
/*
 *  GLUT calls this routine when a mouse is moved
 */
void motion(int x,int y)
{
   if (move<0) return;
   //  Update point
   P[move] = Mouse2World(x,y);
   //  Redisplay
   glutPostRedisplay();
}
/*
*   Glut mouse onclick event
*/
void mouse(int button, int state, int x, int y){
    if(mode == 0){
        Point p = Mouse2World(x,y);
        //Add a point or edit close point
        if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
            int nodeToEdit = -1;
            double dmin = .2*dim;
            for(int i = 0; i < n; i++){
                double dist = Distance(p,i);
                if(dist < dmin){
                    nodeToEdit = i;
                    dmin = dist;
                }
            }
            if(n < NMAX && nodeToEdit == -1){
                P[n] = p;
                move = n;
                n++;
            }else if(nodeToEdit != -1){
                move = nodeToEdit;
            }
        }else if(button == GLUT_RIGHT_BUTTON && state == GLUT_UP){
            int nodeToDelete = -1;
            double dmin = .05*dim;
            for(int i = 0; i < n; i++){
                double dist = Distance(p,i);
                if(dist < dmin){
                    nodeToDelete = i;
                    dmin = dist;
                }
            }
            if(nodeToDelete != -1){
                for(int i = nodeToDelete; i < n-1; i++){
                    P[i] = P[i+1];
                }
                n--;
            }
        }else if(button == GLUT_LEFT_BUTTON && state == GLUT_UP){
            P[move] = p;
            move = -1;
        }
        glutPostRedisplay();
    }
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
void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glRotatef(-90,1,0,0);
    if(mode==0){
        glColor3f(0,1,0);
        glBegin(GL_LINE_LOOP);
        for(int i = 0; i < n; i++){
            glVertex3d(P[i].x, P[i].y, P[i].z);
        }
        glEnd();
        glColor3f(1,1,1);
        glPointSize(7);
        glBegin(GL_POINTS);
        for(int i = 0; i < n; i++){
            glVertex3d(P[i].x, P[i].y, P[i].z);
        }
        glEnd();
        glPointSize(1);
    }
    ErrCheck("display");
    glFlush();
    glutSwapBuffers();
}
int main(int argc, char *argv[])
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(600,600);
    glutCreateWindow("Carter Andrew | Final Project | Procedural House Generation");
#ifdef USEGLEW
   //  Initialize GLEW
   if (glewInit()!=GLEW_OK) Fatal("Error initializing GLEW\n");
#endif
    //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutMouseFunc(mouse);
   glutMotionFunc(motion);
   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
