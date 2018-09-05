#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <unistd.h>
using namespace std;
int x1=0,x2=0,y_1=3,y2=3;
void Inicio()
{
	glMatrixMode(GL_PROJECTION);
	glClearColor(1.0,1.0,1.0,0.0);
	gluOrtho2D(-100,100,-100,100);
}
void setPixel(GLfloat x,GLfloat y){
	glColor3f(1.0,0.0,0.0);
	glPointSize(2);
	glBegin(GL_POINTS);
	glVertex2f(x,y);
	glEnd();
}
void incremental_basico(GLfloat x0,GLfloat y0, GLfloat xf, GLfloat yf){
	float x=0,y=0,valor;
	float a;
	a=(yf-y0)/(xf-x0);
	for(x=x0;x<=xf;x+=0.001){
		y=( y0 + a * ( x - x0) );
		setPixel(x,y);
	}
}
void punto_medio(GLfloat x0,GLfloat y0, GLfloat xf, GLfloat yf){
	float dx,dy,incE,incNE,d,x,y;
	
	dx=xf-x0;
	dy=yf-y0;
	d=2*dy-dx; /* Valor inicial de d*/
	incE=2*dy; /*Incremento de E*/
	incNE=2*(dy-dx);/* Incremento de NE*/
	x=x0;
	y=y0;
	setPixel(x,y);
	while (x < xf){
		if(d <=0){
			d=d+incE;
			x=x+0.001;
		}else{
		   d=d+incNE;
		   x=x+0.001;
		   y=y+0.001;
		}
		setPixel(x,y);
	}
}
void dibujar_linea(GLfloat x0,GLfloat y0, GLfloat xf, GLfloat yf)
{
	glBegin(GL_LINES);
	glColor3f(0.0,0.0,0.0);
	glVertex2i(x0,y0);
	glVertex2i(xf,yf);
	glEnd();
}

void draw(){
	glClear(GL_COLOR_BUFFER_BIT);
	GLfloat x0=-300,y0=-300,xf=300,yf=300;
	//dibujar_linea(x0,y0,xf,yf);
	//incremental_basico( x0, y0,  xf,  yf);
	punto_medio( x0, y0,  xf,  yf);
	glFlush();
}

int main ( int argc , char ** argv )
{
glutInit (&argc,argv ) ;
glutInitDisplayMode( GLUT_SINGLE | GLUT_RGB ) ;
glutInitWindowPosition (300 , 100) ;
glutInitWindowSize ( 600,600 ) ;
glutCreateWindow ( " Algoritmos Lineas " ) ;
Inicio() ;
glutDisplayFunc (draw) ;
glutMainLoop () ;

}
