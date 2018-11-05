#ifdef __APPLE__
# include <OpenGL/gl.h>
# include <OpenGL/glu.h>
# include <GLUT/glut.h>
#else
# include <GL/gl.h>
# include <GL/glu.h>
# include <GL/glut.h>
#endif
#include "camera.h"
#include <cstdio>
#include "vector_tools.h"

static camera *LOCAL_MyCamera;
static int old_x, old_y;
int current_mode=0;

void Examinar(int x, int y){
	float rot_x, rot_y;
	rot_y = (float)(old_y - y);
	rot_x = (float)(x - old_x);
	Rotar_Latitud( LOCAL_MyCamera, rot_y * DEGREE_TO_RAD );
	Rotar_Longitud( LOCAL_MyCamera, rot_x * DEGREE_TO_RAD );
	old_y = y;
	old_x = x;
	glutPostRedisplay();
}

void MouseMotion(int x, int y){
	old_y = y;
	old_x = x;
}

void keyboard(unsigned char key, int x, int y) {
	//GLint params[2];
		switch (key) {
		case 'h':
			printf("help\n\n");
			printf("c            -  Toggle culling\n");
			printf("q/escape     -  Quit\n\n");
			break;
		case 'c':
			if (glIsEnabled(GL_CULL_FACE))
				glDisable(GL_CULL_FACE);
			else
				glEnable(GL_CULL_FACE);
			break;
		case 'd':
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
			break;
		case '1':
			glRotatef(1.0,1.,0.,0.);
			break;
		case '2':
			glRotatef(1.0,0.,1.,0.);
			break;
		case 'q':
		case 27:
			DestroyCamera(&LOCAL_MyCamera);
			exit(0);
			break;
		default:
			;	
		}
	
	glutPostRedisplay();
}


static void SpecialKey ( int key, int x, int y ){
	switch(key) {
	case GLUT_KEY_F1:
		glutPassiveMotionFunc(MouseMotion);
		LOCAL_MyCamera->camMovimiento = CAM_STOP;
		break;
	case GLUT_KEY_F2:
		glutPassiveMotionFunc(Examinar);
		LOCAL_MyCamera->camMovimiento = CAM_EXAMINAR;
		break;
	case GLUT_KEY_F3:
		glutPassiveMotionFunc(MouseMotion);
		LOCAL_MyCamera->camMovimiento = CAM_PASEAR;
		LOCAL_MyCamera->camAtY = 0;
		LOCAL_MyCamera->camViewY = 0;
		SetDependentParametersCamera( LOCAL_MyCamera );
		break;
	case GLUT_KEY_HOME: //Reset Camera
		LOCAL_MyCamera->camAtX =0;
		LOCAL_MyCamera->camAtY =0;
		LOCAL_MyCamera->camAtZ =0;
		LOCAL_MyCamera->camViewX = 0;
		LOCAL_MyCamera->camViewY = 1;
		LOCAL_MyCamera->camViewZ = -3;
		SetDependentParametersCamera( LOCAL_MyCamera );
		break;
	case GLUT_KEY_F4:
		if(LOCAL_MyCamera->camProjection == CAM_CONIC){
			LOCAL_MyCamera->x1=-3;
			LOCAL_MyCamera->x2=3;
			LOCAL_MyCamera->y1=-3;
			LOCAL_MyCamera->y2=3;
			LOCAL_MyCamera->z1=-5;
			LOCAL_MyCamera->z2=5;
			LOCAL_MyCamera->camProjection = CAM_PARALLEL;
		} else LOCAL_MyCamera->camProjection = CAM_CONIC;
		break;
	case GLUT_KEY_F5:
		if (current_mode != 0) break;
		current_mode = 3;
		LOCAL_MyCamera->camMovimiento = CAM_TRIPODE;
		break;
	default:
		printf("key %d %c X %d Y %d\n", key, key, x, y );
	}
	glutPostRedisplay();
}


void Zoom(int x, int y){
	float zoom;
	zoom = (float) ((y - old_y) * DEGREE_TO_RAD);
	old_y = y;
	switch(LOCAL_MyCamera->camMovimiento){
	case CAM_EXAMINAR:
		if (LOCAL_MyCamera->camAperture + zoom > (5 * DEGREE_TO_RAD) &&
			LOCAL_MyCamera->camAperture + zoom < 175 * DEGREE_TO_RAD)
			LOCAL_MyCamera->camAperture=LOCAL_MyCamera->camAperture + zoom;
		break;
	}
	glutPostRedisplay();
}
void Andar(int x, int y){
	float rotacion_x, avance_y;
	avance_y = (float)(y - old_y) / 10;
	rotacion_x = (float)(old_x - x) * DEGREE_TO_RAD / 5;
	YawCamera( LOCAL_MyCamera, rotacion_x );
	AvanceFreeCamera( LOCAL_MyCamera, avance_y);
	old_y = y;
	old_x = x;
	glutPostRedisplay();
}
void Tripode(int x, int y){
	float rotacion_x, rotacion_y;
	rotacion_x = (float)(old_x - x) * DEGREE_TO_RAD / 5;
	rotacion_y = (float)(old_y - y) * DEGREE_TO_RAD / 5;
	YawCamera( LOCAL_MyCamera, rotacion_x );
	PitchCamera( LOCAL_MyCamera, rotacion_y );
	old_y = y;
	old_x = x;
	glutPostRedisplay();
}
void Pan(int x, int y){
	float avance_x, avance_y;
	avance_x = (float)(old_x - x) / 10;
	avance_y = (float)(y - old_y) / 10;
	PanCamera( LOCAL_MyCamera, avance_x, avance_y);
	old_y = y;
	old_x = x;
	glutPostRedisplay();
}
void mouse(int button, int state, int x, int y){
	old_x = x;
	old_y = y;
	switch(button){
	case GLUT_LEFT_BUTTON:
		switch(LOCAL_MyCamera->camMovimiento){
		case CAM_EXAMINAR:
			if (state == GLUT_DOWN) glutMotionFunc(Zoom);
			if (state == GLUT_UP){
				glutPassiveMotionFunc(Examinar);
				glutMotionFunc(NULL);
			}
			break;
		case CAM_PASEAR:
			if (state == GLUT_DOWN) glutMotionFunc(Andar);
			if (state == GLUT_UP) glutMotionFunc(NULL);
			break;
		}
		case CAM_TRIPODE:
			if (state == GLUT_DOWN) glutMotionFunc(Tripode);
			if (state == GLUT_UP) glutMotionFunc(NULL);
			break;
		case CAM_PAN:
			if (state == GLUT_DOWN) glutMotionFunc(Pan);
			if (state == GLUT_UP) glutMotionFunc(NULL);
			break;
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN) ;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void drawTurtle() {
	int npoints;
	double x[] = {0.,   .3,  .4,  .7,   .9, 1.0,  .6, .9, .9,  .7,  .95,1.1, 1.2,  1.0,  .65,  .6,  .3, .3,   .5,     .65,  .45,  .1};
	double z[] = {1.15, 1.2, 1.5, 1.55, 1.3, 0.95, .8, .5, .1, -.3, -.4, -.4, -.9, -1.1, -.95, -.8, -.8, -1.1, -1.2, -1.6, -1.8, -2.0};
	
	int i;
	npoints = 22;
	glBegin(GL_LINE_LOOP);
	//	glBegin(GL_POLYGON);
	for (i=0; i < npoints; i++){
		glVertex3d(x[i],0.0,-z[i]);
	}
	for (i=npoints -1; i > -1; i--){
		glVertex3d(-x[i],0.0,-z[i]);
	}
	glEnd();
}

void display()
{
	glClearColor(1.5,1.5,1.0,1.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	SetGLCamera( LOCAL_MyCamera );
	glColor3f(0.0,0.0,0.0);
	drawTurtle();
	glutSwapBuffers();
}
void reshape(int width,int height)
{
	glViewport(0,0,width,height);
	SetGLAspectRatioCamera( LOCAL_MyCamera );
}



int main(int argc,char** argv)
{
	
	
	LOCAL_MyCamera = CreatePositionCamera(0.0f, 1.0f, -3.0f);
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_DEPTH|GLUT_DOUBLE);
	glutInitWindowSize(800,600);
	glutInitWindowPosition(20,20);
	glutCreateWindow("Tortuga");
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(SpecialKey);
	glutPassiveMotionFunc(MouseMotion);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}

