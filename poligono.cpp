#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <unistd.h>
using namespace std;

double calx=0.0,caly=0.0;
int radio=10,lados=60;
class punto{
public:
	double x;
	double y;
	punto(){};
	punto(double x_,double y_);
};
punto::punto(double x_,double y_){
	x=x_;
	y=y_;
}
void Inicio()
{
	glMatrixMode(GL_PROJECTION);
	glClearColor(0.0,0.0,0.0,0.0);
	gluOrtho2D(-20,20,-20,20);
}

void display()
{
	int tam=lados+1;
	punto * pt =new punto[tam];
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0,1.0,1.0);
	glBegin(GL_POINTS);
	for(double i=0.0;i<360.0;i+=0.1)
	{
		calx=radio*cos((i*M_PI)/180);
		caly=radio*sin((i*M_PI)/180);
	}
	glEnd();

	punto p;
	double i,l;
	int j;
	if(lados==3){
		i=90.0;

	}
	if(lados==4){
		i=45.0;

	}
	if(lados>4){
		i=0.0;

	}
	for(l=i,j=0;l<360.0;l+=360.0/lados*1.0,j++){
			p.x=radio*cos((l*M_PI)/180);
			p.y=radio*sin((l*M_PI)/180);
			pt[j]=p;
		}
		pt[j]=pt[0];
		for(int k=0;k<tam-1;k++){
			glBegin ( GL_LINES ) ;
				glVertex2f ( pt[k+1].x,pt[k+1].y) ;
				glVertex2f ( pt[k].x,pt[k].y) ;
				
				
			glEnd ( ) ;
	}
	glFlush();
}


int main ( int argc , char ** argv )
{
glutInit (&argc,argv ) ;
glutInitDisplayMode( GLUT_SINGLE | GLUT_RGB ) ;
glutInitWindowPosition (10 , 10) ;
glutInitWindowSize ( 600,600 ) ;
glutCreateWindow ( " Poligono regular circunscrito " ) ;
Inicio() ;
glutDisplayFunc (display) ;
glutMainLoop () ;
return EXIT_SUCCESS;
}

