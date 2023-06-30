#include <vector>
#include "CSCIx229.h"
int width  = 1; //  Window width
int height = 1; //  Window height
double asp = 1; //  Window aspect ratio

#define NMIN  3 //  Minimum number of vertecies to render house
#define NMAX  8 //  Maximum number of vertecies to render house
#define PLOTMIN 20 // Minimum land plot size
#define PLOTMAX 70 // Maxium land plot size
int n=0;        //  Number of nodes making up land plot
int move=-1;    //  Point2d getting moved
Point2d P[NMAX];  //  Data Point2ds
//Display mode 0: edit plot of house
//Display mode 1: View house from exterior
//Display mode 2: First person view mode
//Display mode 4: Outline mode
int mode=0;     //  Display mode
int dimMin = 20;//  Minimum plot size
int dimMax = 200;// Maxiumum plot size
int dim = 20;     //  Scale of world
//3D House construction components


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
   double dx = p.x - P[k].x;
   double dy = p.y - P[k].y;
   return sqrt(dx*dx+dy*dy);
}
/*
 *  GLUT calls this routine when a mouse is moved
 */
void motion(int x,int y)
{
   if (move<0) return;
   //  Update Point2d
   P[move] = Mouse2World(x,y);
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
    glutPostRedisplay();
}
void key(unsigned char ch, int x, int y){
    if(mode == 0){
        switch (ch)
        {
        case 'd':
            n = 0;
            break;
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
void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glRotatef(-90,1,0,0);
    if(mode==0){
        glColor3f(.3,.3,.3);
        glBegin(GL_LINES);
        for(int i=-dim;i<=dim;i++) {
            glVertex3f(i,0,-dim);
            glVertex3f(i,0,dim);
            glVertex3f(-dim,0,i);
            glVertex3f(dim,0,i);
        };
        glEnd();
        glColor3f(0,1,0);
        if(move == -1){
            for(int i = 0; i < n-1; i++){
                for(int j = 0; j < n-1; j++){
                    if(i==j) continue;
                    if(linesIntersect(P[i],P[i+1],P[j],P[j+1]) == 1){ 
                        glColor3f(1,0,0);
                        Point2d intersection = pointOfIntersection(P[i],P[i+1],P[j],P[j+1]);
                        glPointSize(7);
                        glBegin(GL_POINTS);
                        glVertex3f(intersection.x, 0, intersection.y);
                        glEnd();
                        glPointSize(1);
                    }
                }
            }
        }
        printf("\n");
        glBegin(GL_LINE_LOOP);
        for(int i = 0; i < n; i++){
            glVertex3d(P[i].x,0 ,P[i].y);
        }
        glEnd();
        glColor3f(1,1,1);
        glPointSize(7);
        glBegin(GL_POINTS);
        for(int i = 0; i < n; i++){
            glVertex3d(P[i].x, 0, P[i].y);
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
    glutKeyboardFunc(key);
    glutSpecialFunc(special);
    //  Pass control to GLUT so it can interact with the user
    ErrCheck("init");
    glutMainLoop();
    return 0;
}
