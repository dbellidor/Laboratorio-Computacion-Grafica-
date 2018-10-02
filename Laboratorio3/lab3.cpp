#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <cmath>
using namespace std;

int np;
float px[10000];
float py[10000];
float pz[10000];

float trnsX=0.0;
float trnsY=0.0;
float trnsZ=0.0;

float rotaX=0.0;
float rotaY=0.0;
float rotaZ=0.0;



GLdouble mModel[16];

void drawTurtle(	)
{
    glClear(GL_COLOR_BUFFER_BIT);

    double x[] = {0, -.4, -.37, -.2, -.2, -.45, -.45, -.6, -.75, -.77, -.4, -.5, -.52, -.4, -.75, -.77, -.6, -.4, -.2, .0};
    double y[] = {1, .9, .73, .6, .45, .45, .5, .6, .5, .2, .2, .06, -.25, -.4, -.5, -.8, -.9, -.9, -.7, -.7};
    glColor3f(0.0f, 1.0f, 0.0f);
    glLineWidth(1.f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 20; i++) {
        glVertex3d(x[i], y[i], 0);
    }
    for (int i = 19; i > -1; i--) {
        glVertex3d(-1 * x[i], y[i], 0);

    }
    glEnd();
  
  
    glLineWidth(5);
    glBegin (GL_LINES);
		glColor3f (0,1,0); // X axis is red.
		glVertex3f(0,0,0);
		glVertex3f(0.5,0,0 ); 
		
		glColor3f (1,0,0); // Y axis is green.
		glVertex3f(0,0,0);
		glVertex3f(0,0.5,0);
		
		/*glColor3f (0,0,1); // z axis is blue.
		glVertex3f(0,0,0);
		glVertex3f(0,0,0.5 ); */
	glEnd();
	glLineWidth(1);
  
    glFlush();
}



//Draw Trace------------------------------------

void displayTrace()
{
	int i;
	glColor3f(0.5,0.5,1.0) ;
	glBegin(GL_LINE_STRIP);
	for (i = 0; i < np; i++) 
		glVertex3f(px[i],py[i],pz[i]);
	glEnd();
}


void addPointToTrace() {
	int i;
	GLdouble m[16];
	glGetDoublev (GL_MODELVIEW_MATRIX, m);

	if (np == 0) 
	{ 
		px [0] = 0.0;
		py [0] = 0.0;
		pz [0] = 0.0;
		np++;
	}
	
	px [np] = (m[0] * px [0] + m[4] * py [0] + m[8] * pz [0] + m[12])*0.5;
	py [np] = (m[1] * px [0] + m[5] * py [0] + m[9] * pz [0] + m[13])*0.5;
	pz [np] = (m[2] * px [0] + m[6] * py [0] + m[10] * pz [0] + m[14])*0.5;
	
	np++;
}



void display() 
{
    glClearColor(1.0, 1.0, 1.0, 0.0);

    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
		glRotatef( rotaX, 1.0, 0.0, 0.0 );
		glRotatef( rotaY, 0.0, 1.0, 0.0 );
		glRotatef( rotaZ, 0.0, 0.0, 1.0 );

		glTranslatef(trnsX, trnsY, trnsZ);		
		addPointToTrace();
		glMultMatrixd(mModel);
		glColor3f(1.0,0.0,0.0);
		drawTurtle();


	glPopMatrix();
	
	displayTrace(); 


    glutSwapBuffers();
}

void reshape(int width, int height) 
{

	glGetDoublev (GL_MODELVIEW_MATRIX, mModel);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)height / (GLfloat)width, 1.6, 128.0);
    glMatrixMode(GL_MODELVIEW);

    gluLookAt(0.0, 1.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

}


void teclado(unsigned char key, int x, int y) 
{

    switch (key) {
    case 'q':
		rotaZ+=3;
    	break;
    case 'e':
	    rotaZ+=-3;
    	break;    
    case 'w':
		rotaX+=3;
        break;
    case 's':
    	rotaX+=-3;
        break;
    case 'a':
	    rotaY+=-3;
        break;
    case 'd':
	    rotaY+=3;
        break;
	case '-':
        glScalef(0.5,0.5,0.5);
        break;
    case '+':
	    glScalef(2,2,2);
	    break;


    case 27:
        exit(0);
        break;
    }
    
    glutPostRedisplay();
}


void catchKey(int key, int x, int y)
{
    if(key == GLUT_KEY_LEFT)    
    {
		trnsX+= -.1;
    }
    
    else if(key == GLUT_KEY_RIGHT)
    {
		trnsX+= .1;
    }

    else if(key == GLUT_KEY_DOWN)
    {
		trnsY+= -.1;
	}
    else if(key == GLUT_KEY_UP)
    {
		trnsY+= .1;
    }

    glutPostRedisplay();
}

int main(int argc, char** argv) 
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB |  GLUT_DOUBLE);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(20, 20);
    glutCreateWindow("Tortuga");
    glutKeyboardFunc(teclado);
    glutSpecialFunc(catchKey);
    glutDisplayFunc(display);
    
    glutReshapeFunc(reshape);


    glutMainLoop();
    return 0;
}
