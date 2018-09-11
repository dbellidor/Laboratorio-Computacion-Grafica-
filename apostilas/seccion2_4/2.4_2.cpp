#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>

int width=600;
int height=600;

using namespace std;
double posX=0.0;
double posY=0.0;
int radio;
int lados;
struct dot
{
	double _x;
	double _y;
	dot()
		:_x(0.0),_y(0.0){}
	dot(double nx,double ny)
		:_x(nx),_y(ny){}
};	

void init()
{
	glMatrixMode(GL_PROJECTION);
	glClearColor(0.0,0.0,0.0,0.0);
	gluOrtho2D(-600,600,-600,600);//la escala creo
}
void display()
{
	int tamVector=lados+1;//tamaño del vector de puntos
	dot * puntos=new dot[tamVector];
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0,1.0,1.0);
	glBegin(GL_POINTS);
	for(double i=0.0;i<360.0;i++)
	{
		posX=radio*cos((i*M_PI)/180.0);
		posY=radio*sin((i*M_PI)/180.0);
		glVertex2f(posX,posY);
	}
	glEnd();
	dot d;
	double i;
	int j;
	if(lados==3)
	{
		for(i=90.0,j=0;i<360.0;i+=360/lados*1.0,j++)
		{
			d._x=radio*cos((i*M_PI)/180.0);
			d._y=radio*sin((i*M_PI)/180.0);
			puntos[j]=d;
		}
		puntos[j]=puntos[0];
		for(int i=0;i<tamVector-1;i++)
		{
			glBegin(GL_LINES);
				glVertex2f(puntos[i+1]._x,puntos[i+1]._y);
				glVertex2f(puntos[i]._x,puntos[i]._y);
			glEnd();
		}
	}
	
	if(lados>3)
	{
		for(i=0.0,j=0;i<360.0;i+=360.0/lados*1.0,j++)
		{
			d._x=radio*cos((i*M_PI)/180.0);
			d._y=radio*sin((i*M_PI)/180.0);
			puntos[j]=d;
		}
		puntos[j]=puntos[0];
		for(int i=0;i<tamVector-1;i++)
		{
			glBegin(GL_LINES);
			glVertex2f(puntos[i+1]._x,puntos[i+1]._y);
			glVertex2f(puntos[i]._x,puntos[i]._y);
			glEnd();
		}
	}
	glFlush();
	
}	
int main(int argc,char **argv)
{
	cout<<"Ingrese el número de aristas: ";
	cin>>lados;
	cout<<"Ingrese el radio: ";
	cin>>radio;
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowPosition(10,10);
	glutInitWindowSize(width,height);
	glutCreateWindow("Poligonos");
	init();
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}	

