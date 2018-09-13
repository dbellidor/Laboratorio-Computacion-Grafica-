#include <stdio.h>
#include <stdlib.h>
#include "GL/freeglut.h"
#include "GL/gl.h"
#include <string.h>
bool command = false; /* command mode */
char strCommand[256];
int width=600;
int height=600;
GLfloat Y = 0, X = 1;
using namespace std;

void init()
{
	glMatrixMode(GL_PROJECTION);
	glClearColor(0.0,0.0,0.0,0.0);
	gluOrtho2D(-3,3,-3,3);//la escala
}
void draw_turtle() {

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


}
void display()
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBegin(GL_LINES);
    glColor3f(0.0f, 1.0f, 0.0f); // green
    glEnd();
    draw_turtle();
    glutSwapBuffers();
	
}
void parseCommand(char* strCommandParse) {
	char *strToken0;
	char *strToken1;
	double val;
	strToken0 = strtok(strCommandParse, " ");
	while ((strToken1 = strtok(NULL," ")) != NULL) {
		val = atof(strToken1);
		if (!strcmp("fd",strToken0)) { // FORWARD
			glTranslatef(0.0, 0.0, val);
		} else if (!strcmp("bk",strToken0)) { // BACK
			glTranslatef(0.0, 0.0, -val);
		} else if (!strcmp("rt",strToken0)) { // RIGHT
			glRotatef(-val,0.,1.,0.);
		} else if (!strcmp("lt",strToken0)) { // LEFT
			glRotatef(val,0.,1.,0.);
		} else if (!strcmp("up",strToken0)) { // UP
			glRotatef(val,1.,0.,0.);
		} else if (!strcmp("dn",strToken0)) { // DOWN
			glRotatef(-val,1.,0.,0.);
		}
		strToken0 = strtok(NULL, " ");
		display();
	}
	// EXIT COMMAND MODE
	if (strToken0 != NULL && strncmp(strToken0, "exit", 4) == 0) {
		command = false;
	// HOME
	} else if (strToken0 != NULL && !strcmp("home",strToken0)) {
		glLoadIdentity();
	}
}


void keyboard(unsigned char key, int x, int y) {
	if (command) {
		if (key == 13) {
			strcat(strCommand, " ");
			if (strlen(strCommand) == 1) command = false;
			parseCommand(strCommand);
			strcpy(strCommand, "");
		} else {
			char strKey[2] = " ";
			strKey[0] = key;
			printf(strKey);
			strcat(strCommand, strKey);
		}
	} else { // not in command mode
		switch (key) {
		case 'h':
			printf("help\n\n");
			printf("c 		- Toggle culling\n");
			printf("q/escape 		- Quit\n\n");
			break;
		case 'c':
			if (glIsEnabled(GL_CULL_FACE))
				glDisable(GL_CULL_FACE);
			else
				glEnable(GL_CULL_FACE);
			break;
		case '1':
			glRotatef(1.0,1.,0.,0.);
			break;
		case '2':
			glRotatef(1.0,0.,1.,0.);
			break;
		case 'i':
			command = true;
			break;
		case 'q':
		case 27:
			exit(0);
			break;
		}

	}	
	glutPostRedisplay();
}	


void reshape(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)height / (GLfloat)width, 1.0, 128.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 1.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

int main(int argc,char **argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowPosition(10,10);
	glutInitWindowSize(width,height);
	glutCreateWindow("Poligonos");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}	


