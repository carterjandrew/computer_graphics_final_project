/*
 */
#include "CSCIx229.h"
#define GL_FRAGMENT_PRECISION_HIGH 1
int shader = 0;
int screen[] = {600,600};
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
float dim = 20;
int mode = 2;
int dMode = 0;
int time = 1;
int fun = 0;
int maxDist = 100;
int maxSteps = 100;
float surfDist = .05;
const char* modes[] = {"Repitition", "Subtraction", "Smooth Union"};

/*
 *  Read text file
 */
char* ReadText(char *file)
{
   char* buffer;
   //  Open file
   FILE* f = fopen(file,"rt");
   if (!f) Fatal("Cannot open text file %s\n",file);
   //  Seek to end to determine size, then rewind
   fseek(f,0,SEEK_END);
   int n = ftell(f);
   rewind(f);
   //  Allocate memory for the whole file
   buffer = (char*)malloc(n+1);
   if (!buffer) Fatal("Cannot allocate %d bytes for text file %s\n",n+1,file);
   //  Snarf the file
   if (fread(buffer,n,1,f)!=1) Fatal("Cannot read %d bytes for text file %s\n",n,file);
   buffer[n] = 0;
   //  Close and return
   fclose(f);
   return buffer;
}

/*
 *  Print Shader Log
 */
void PrintShaderLog(int obj,char* file)
{
   int len=0;
   glGetShaderiv(obj,GL_INFO_LOG_LENGTH,&len);
   if (len>1)
   {
      int n=0;
      char* buffer = (char *)malloc(len);
      if (!buffer) Fatal("Cannot allocate %d bytes of text for shader log\n",len);
      glGetShaderInfoLog(obj,len,&n,buffer);
      fprintf(stderr,"%s:\n%s\n",file,buffer);
      free(buffer);
   }
   glGetShaderiv(obj,GL_COMPILE_STATUS,&len);
   if (!len) Fatal("Error compiling %s\n",file);
}

/*
 *  Print Program Log
 */
void PrintProgramLog(int obj)
{
   int len=0;
   glGetProgramiv(obj,GL_INFO_LOG_LENGTH,&len);
   if (len>1)
   {
      int n=0;
      char* buffer = (char *)malloc(len);
      if (!buffer) Fatal("Cannot allocate %d bytes of text for program log\n",len);
      glGetProgramInfoLog(obj,len,&n,buffer);
      fprintf(stderr,"%s\n",buffer);
   }
   glGetProgramiv(obj,GL_LINK_STATUS,&len);
   if (!len) Fatal("Error linking program\n");
}

/*
 *  Create Shader
 */
int CreateShader(GLenum type,char* file)
{
   //  Create the shader
   int shader = glCreateShader(type);
   //  Load source code from file
   char* source = ReadText(file);
   glShaderSource(shader,1,(const char**)&source,NULL);
   free(source);
   //  Compile the shader
   fprintf(stderr,"Compile %s\n",file);
   glCompileShader(shader);
   //  Check for errors
   PrintShaderLog(shader,file);
   //  Return name
   return shader;
}

/*
 *  Create Shader Program
 */
int CreateShaderProg(char* VertFile,char* FragFile)
{
   //  Create program
   int prog = glCreateProgram();
   //  Create and compile vertex shader
   int vert = CreateShader(GL_VERTEX_SHADER,VertFile);
   //  Create and compile fragment shader
   int frag = CreateShader(GL_FRAGMENT_SHADER,FragFile);
   //  Attach vertex shader
   glAttachShader(prog,vert);
   //  Attach fragment shader
   glAttachShader(prog,frag);
   //  Link program
   glLinkProgram(prog);
   //  Check for errors
   PrintProgramLog(prog);
   //  Return name
   return prog;
}

void reshape(int width,int height)
{
   glViewport(0,0, RES*width,RES*height);
   screen[0] = width;
   screen[1] = height;
}

void special(int key,int x,int y)
{
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
   else if (key == GLUT_KEY_PAGE_DOWN)
      dim += 1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_UP && dim>1)
      dim -= 1;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}
void key(unsigned char ch,int x,int y){
   switch (ch)
   {
   case 'm':
      mode = (mode+1)%3;
      break;
   case 'n':
      dMode = !dMode;
      break;
   case 'p':
      time = !time;
      break;
   case 'f':
      fun = !fun;
      break;
   default:
      break;
   }
}

void display(){
   glClear(GL_COLOR_BUFFER_BIT);
   float Ex = +2*dim*Sin(th)*Cos(ph);
   float Ey = +2*dim        *Sin(ph);
   float Ez = -2*dim*Cos(th)*Cos(ph);
   glUseProgram(shader);
   glUniform2f(glGetUniformLocation(shader,"u_resolution"), (float)screen[0],(float)screen[1]);
   if(time)glUniform1f(glGetUniformLocation(shader, "u_time"), .0005 * glutGet(GLUT_ELAPSED_TIME));
   glUniform3f(glGetUniformLocation(shader, "u_cam"), Ex, Ey + 1, Ez);
   glUniform1i(glGetUniformLocation(shader, "u_mode"), mode);
   glUniform1i(glGetUniformLocation(shader, "u_display"), dMode);
   glUniform1i(glGetUniformLocation(shader, "u_fun"), fun);
   glUniform1i(glGetUniformLocation(shader, "u_maxDist"), maxDist);
   glUniform1i(glGetUniformLocation(shader, "u_maxSteps"), maxSteps);
   glUniform1i(glGetUniformLocation(shader, "u_surfDist"), surfDist);
   glColor3f(1,1,1);
   glBegin(GL_QUADS);
   glVertex2d(-1,-1);
   glVertex2d(1,-1);
   glVertex2d(1,1);
   glVertex2d(-1,1);
   glEnd();
   glUseProgram(0);
   glWindowPos2i(5,85);
   Print("Angle=%d,%d  Dim=%.1f", th,ph,dim);
   glWindowPos2i(5,45);
   Print("View dist=%.1f   View steps=%.1f   Surface detection=%.3f", maxDist,maxSteps,surfDist);
   glWindowPos2i(5,5);
   Print("View mode %s  Display mode %s", modes[mode], dMode?"Normals":"Lit and Shaded");
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}
void idle(){
   glutPostRedisplay();
}
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
   glutInitWindowSize(800,600);
   screen[0] = 800;
   screen[1] = 600;
   glutCreateWindow("Andrew_Carter_Final_Project_RayMarching");
#ifdef USEGLEW
   //  Initialize GLEW
   if (glewInit()!=GLEW_OK) Fatal("Error initializing GLEW\n");
#endif
   //  Set callbacks
   glutReshapeFunc(reshape);
   glutDisplayFunc(display);
   glutIdleFunc(idle);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   //  Create Shader Programs
   shader = CreateShaderProg("Shaders/raymarch.vert", "Shaders/raymarch.frag");
   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
