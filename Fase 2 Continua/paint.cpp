#include <unistd.h>
#include <time.h>
#include <math.h>
#include <GL/glui.h>
#include <GL/glut.h>
#define minPixThresh 6
//*************************************************************************
//  GLUT Declaraciones
//*************************************************************************

//  Initialization
void init ();

//  Callback functions
void display (void);
void reshape (int w, int h);
void mouse (int button, int state, int x, int y);

bool check=false;
int ClickCount = 0, X1 = 0, Y1 = 0, X2 = 0, Y2 = 0,xBegin = 0,yBegin = 0,statusComplete = 0;
int mousex,mousey;
//  definir la posicion en la ventana
int window_x;
int window_y;

//  variables tamanio de ventana 
int window_width = 512;
int window_height = 512;

//  titulo de la ventana
char *window_title = "GLUI Paint";

//  el id de la ventana
GLuint main_window;

int full_screen = 0;

//*************************************************************************
//  GLUI Declarations
//*************************************************************************

//  pointer to the GLUI window
GLUI * glui_window;

//  Declare live variables (related to GLUI)
int draw = 1;				//  Relacionado para Draw Check Box
int wireframe = 1;			//  Related to Wireframe Check Box
int listbox_item_id = 4;	//  Id de el seleccionado item en la list box
int radiogroup_item_id = 0; //  Id de el boton de radio
float rotation_matrix[16]	//  Matriz de rotacion
							= { 1.0, 0.0, 0.0, 0.0, 
								0.0, 1.0, 0.0, 0.0,
								0.0, 0.0, 1.0, 0.0, 
								0.0, 0.0, 0.0, 1.0 };	
float translate_xy[2]		//  Traslacion x,y
							= {0, 0};			
float scale = 1;			//  Spinner Scale Live Variable

// array rbg colores
float color[] = { 1.0, 1.0, 1.0 };

// Configurar GLUI
void setupGLUI ();

// ID del componente de la interfaz
void idle ();

//  llamado para la funciones 
void glui_callback (int arg);

//  ID de los componentes
enum
{
	COLOR_LISTBOX = 0,
	OBJECTYPE_RADIOGROUP,
	TRANSLATION_XY,
	ROTATION,
	SCALE_SPINNER,
	QUIT_BUTTON
};

//  The different GLUT shapes
enum GLUT_SHAPES
{
	LINEA = 0,
	CIRCULO,
	POLIGONO

};


void centerOnScreen ();
void drawObject ();

//*************************************************************************
//  GLUT Funciones.
//*************************************************************************


void init ()
{	
	
	glClearColor (1.0, 1.0, 1.0, 0.0);
	gluOrtho2D(0.0,window_width,0.0,window_height);
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
}
	

void centerOnScreen ()
{
	window_x = (glutGet (GLUT_SCREEN_WIDTH) - window_width)/2;
	window_y = (glutGet (GLUT_SCREEN_HEIGHT) - window_height)/2;
}



void display (void)
{
	//  Limpiar ventana
	glClear (GL_COLOR_BUFFER_BIT);
	//  Color
	glColor3fv (color);
	
	//  Dibujar objecto
	drawObject ();


}
void setPixel(GLfloat x,GLfloat y){
	
	glPointSize(4);
	glBegin(GL_POINTS);
	glVertex2f(x,y);
	glEnd();
}

void drawLineDDA(int x1,int y1,int x2,int y2)
{
	//finding slope
	float lineSlope,ytemp,xtemp;
	int xtempi, ytempi;
	
	
	ytemp=(float) (y2-y1);
	xtemp = (float) (x2-x1);
	lineSlope = ytemp/xtemp;
	
	glBegin(GL_POINTS);
	
	if((abs(lineSlope) < 1.000000)&&(abs(x2-x1)))
	{
          if(x1>x2)
          {
                   xtempi = x2;
                   x2=x1;
                   x1 = xtempi;
                   ytempi = y2;
                   y2 = y1;
                   y1 = ytempi;
          }
          xtempi = x1;//init
          ytemp = (float)y1;
          while( xtempi <= x2)
          {
              glVertex2i(xtempi, round(ytemp));   
               xtempi++;//increment x by 1 
              
               ytemp = ytemp+ lineSlope;
                
          }
                        
    }
    else
    {
          if(y1>y2)
          {
                   xtempi = x2;
                   x2=x1;
                   x1 = xtempi;
                   ytempi = y2;
                   y2 = y1;
                   y1 = ytempi;
          }
          xtemp = (float)x1;//init
          ytempi = y1;
          while( ytempi <= y2)
          {
              glVertex2i(round(xtemp), ytempi);   
               ytempi++;//increment x by 1 
              
               xtemp = xtemp+ (1.0/lineSlope);
                
          }
    }
	glEnd();

}
void draw_line(int button, int state, int x, int y) 
{
  if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
 {
	 ClickCount++;
    	if(ClickCount > 2)
	{ClickCount = 1;
	X1 = 0;
	Y1 = 0;
	X2 = 0;
	Y2 = 0;
	}
	
    	

	if(ClickCount == 1)
	{
	X1 = x;
	Y1 = window_width - y;
	
	printf("\nRegistered Linea 1st pt X = %d, Y = %d COUNT = %d",X1, Y1,ClickCount);
	glutSwapBuffers();
	
	
	}
        
	if(ClickCount == 2)
	{X2 = x;
	Y2 = window_width - y;
	
	
	drawLineDDA(X1,Y1,X2,Y2);
	printf("\nRegistered Linea 2st ptX = %d, Y = %d COUNT = %d",X2, Y2,ClickCount);
	glutSwapBuffers();
	
        
	}
	
    }
    
	
}

void shiftOriginToCenter(float *x, float *y,int cX,int CY)
{
	(*x) =  cX+(*x);
	(*y) = CY+(*y);
}
void dibujar_circulo(int cX,int CY,float radGlobal){

    float Pk, xK, yK, xtempi, ytempi;
    xK=0;
    yK=radGlobal;
    Pk = 1 - radGlobal;

    while(yK >= xK)
	{
		xtempi = xK;
		ytempi = yK;
		shiftOriginToCenter(&xtempi,&ytempi,cX,CY);
		setPixel(xtempi, ytempi);

		xtempi = yK;
		ytempi = xK;
		shiftOriginToCenter(&xtempi,&ytempi,cX,CY);
		setPixel(xtempi, ytempi);

		xtempi = -yK;
		ytempi = xK;
		shiftOriginToCenter(&xtempi,&ytempi,cX,CY);
		setPixel(xtempi, ytempi);

		xtempi = -xK;
		ytempi = yK;
		shiftOriginToCenter(&xtempi,&ytempi,cX,CY);
		setPixel(xtempi, ytempi);

		xtempi = -xK;
		ytempi = -yK;
		shiftOriginToCenter(&xtempi,&ytempi,cX,CY);
		setPixel(xtempi, ytempi);

		xtempi = -yK;
		ytempi = -xK;
		shiftOriginToCenter(&xtempi,&ytempi,cX,CY);
		setPixel(xtempi, ytempi);

		xtempi = yK;
		ytempi = -xK;
		shiftOriginToCenter(&xtempi,&ytempi,cX,CY);
		setPixel(xtempi, ytempi);

		xtempi = xK;
		ytempi = -yK;
		shiftOriginToCenter(&xtempi,&ytempi,cX,CY);
		setPixel(xtempi, ytempi);
		if(Pk <= 0)
		{
			Pk = Pk + (2*xK) + 3;
		}
		else
		{
			Pk = Pk + (2*xK) -(2*yK) + 5;
			yK = yK - 1;
		}
		xK = xK + 1;

	}
    
}



void draw_circulo(int button, int state, int x, int y)
{
  float distancia=0;
  glClear(GL_COLOR_BUFFER_BIT);
 if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
 {
	
     ClickCount++;
    	if(ClickCount > 2)
	{ClickCount = 1;
	X1 = 0;
	Y1 = 0;
	X2 = 0;
	Y2 = 0;
	}

	if(ClickCount == 1)
	{
	X1 = x;
	Y1 = window_width - y;
	
	printf("\nRegistered circulo 1st pt X = %d, Y = %d COUNT = %d",X1, Y1,ClickCount);
	glutSwapBuffers();
	
	
	}

	if(ClickCount == 2)
	{
	X2 = x;
	Y2 = window_width - y;
	
        distancia = sqrt((X1 - Y1)*(X1 - Y1) + (X2 - Y2)*(X2 - Y2));
	dibujar_circulo(X1,Y1,distancia);
	printf("\nRegistered circulo 2st ptX = %d, Y = %d COUNT = %d",X2, Y2,ClickCount);
	glutSwapBuffers();
	
        
	}
	
    }
	

   
}
void drawBitmapText(char *string,float x,float y,float z) 
{  
	char *c;
	glRasterPos3f(x, y,z);

	for (c=string; *c != '\0'; c++) 
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
	}
}
void draw_poligono(int button, int state, int x, int y) 
{
     //printf("clicks %d \n ",ClickCount);
   
     if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && statusComplete==0)
 	{
		 ClickCount++;
		 printf("clicks %d \n ",ClickCount);
		 
		 if(ClickCount > 2){
				ClickCount = 1;
				X1 = X2;
				Y1 = Y2;
				X2 = 0;
				Y2 = 0;
		}

		if(ClickCount == 1){
		        X1 = x;
			Y1 = window_width - y;
			xBegin = X1;
			yBegin = Y1;
			printf("\nRegistered poli 1st pt X = %d, Y = %d \n",X1, Y1);
		}
	
	
	
		if(ClickCount == 2)
		{
			X2 = x;
			Y2 = window_width - y;
			drawLineDDA(X1,Y1,X2,Y2);
			printf("\nRegistered poli 2pt X = %d, Y = %d \n",X2, Y2);
	
				if( ((X2-xBegin)*(X2-xBegin)) + ((Y2-yBegin)*(Y2-yBegin)) <= (minPixThresh*minPixThresh))
				{
					X1 = X2;
					Y1 = Y2;
					X2 = xBegin;
					Y2 = yBegin;
					drawLineDDA(X1,Y1,X2,Y2);
					printf("\nPolygon complete....");
					//drawBitmapText("Polygon complete....",0,3,0);
					//glFlush();
					statusComplete=1;
				}
		}
		
	
      }

	
}

void drawObject() 
{
 

    	    	int selected_object = (radiogroup_item_id) ;
		if (draw)
		{
			
			glPushMatrix ();
			glMultMatrixf (rotation_matrix);
           		glScalef (scale, scale, scale);
			switch (selected_object)
				{
				  
				    case LINEA:
									
					glutMouseFunc (draw_line);
					
					break;
				    
				    case CIRCULO:
				
					glutMouseFunc(draw_circulo);
					
					break;
				    case POLIGONO:
					
				        glutMouseFunc(draw_poligono);
				
				    default:
					break;
				}

			
			glPopMatrix ();
			
		}
	
    

	
}



/*void reshape (int w, int h)
{
	printf ("GLUT: ");
	window_width = w;
	window_height = h;
	glViewport(0, 0, window_width, window_height);
	printf ("Window Ancho: %d, Window Alto: %d.\n", window_width, window_height);
}*/

//*************************************************************************
//  GLUI Funciones.
//*************************************************************************

//-------------------------------------------------------------------------
//  Congiurar GLUI 
//-------------------------------------------------------------------------
void setupGLUI ()
{

	GLUI_Master.set_glutIdleFunc (idle);
	glui_window = GLUI_Master.create_glui ("Opciones", 0, window_x - 235, window_y);
		
	GLUI_Panel *op_panel = glui_window->add_panel ("Propiedades Objeto");

	//  Separador
	glui_window->add_separator_to_panel (op_panel);

	//  LIstbox colores
	GLUI_Listbox *color_listbox = glui_window->add_listbox_to_panel (op_panel, 
									"Color", &listbox_item_id, COLOR_LISTBOX, glui_callback);
	
	//  Add the items to the listbox
	color_listbox->add_item (1, "Negro");
	color_listbox->add_item (2, "Azul");
	color_listbox->add_item (3, "Cyan");
	color_listbox->add_item (4, "Gris Oscuro");
	color_listbox->add_item (5, "Gris");
	//  Select the White Color by default
	color_listbox->set_int_val (4);

	//---------------------------------------------------------------------
	// 'Object Type' Panel
	//---------------------------------------------------------------------
	
	//  Add the 'Object Type' Panel to the GLUI window
	GLUI_Rollout *ot_rollout = glui_window->add_rollout ("Tipo de FIgura");

	//  Create radio button group
	GLUI_RadioGroup *ot_group = glui_window->add_radiogroup_to_panel 
								(ot_rollout, &radiogroup_item_id, OBJECTYPE_RADIOGROUP, glui_callback);
 	
	//  Add the radio buttons to the radio group
	glui_window->add_radiobutton_to_group( ot_group, "Linea" );
	glui_window->add_radiobutton_to_group( ot_group, "Circulo" );
	glui_window->add_radiobutton_to_group( ot_group, "Poligono" );

	//---------------------------------------------------------------------
	// 'Transformacion' Panel
	//---------------------------------------------------------------------

	//  Aniadir el Panel de transformaciones
	GLUI_Panel *transformation_panel = glui_window->add_panel ("Transformacion");

	//  Crear el panel de transformacion
	GLUI_Panel *transformation_panel1 = glui_window->add_panel_to_panel (transformation_panel, "");

	//   xy  control de traslacion
 	GLUI_Translation *translation_xy = glui_window->add_translation_to_panel (transformation_panel1, "Traslacion XY", GLUI_TRANSLATION_XY, translate_xy, TRANSLATION_XY, glui_callback );
	
	//  Velocidad de traslacion translation 
	translation_xy->set_speed( 0.005 );


	glui_window->add_column_to_panel (transformation_panel1, false);


	//  transformacion panel 2 
	GLUI_Panel *transformation_panel2 = glui_window->add_panel_to_panel (transformation_panel, "");

	//   control de rotacion
	glui_window->add_rotation_to_panel (transformation_panel2, "Rotacion", rotation_matrix, ROTATION, glui_callback);
	
	//   separador
	glui_window->add_separator_to_panel (transformation_panel2);

	//   la escala 
	GLUI_Spinner *spinner = glui_window->add_spinner_to_panel (transformation_panel2, "Scala", GLUI_SPINNER_FLOAT, &scale, SCALE_SPINNER, glui_callback);
	
	//  Set the limits for the spinner
	spinner->set_float_limits ( -4.0, 4.0 );

	//---------------------------------------------------------------------
	// 'Salir' Boton
	//---------------------------------------------------------------------


	glui_window->add_button ("Salir", QUIT_BUTTON, glui_callback);


	glui_window->set_main_gfx_window( main_window );
}

//-------------------------------------------------------------------------
//  GLUI callback function.
//-------------------------------------------------------------------------
void glui_callback (int control_id)
{
	//  Notify that this is a GLUI Callback
	printf ("GLUI: ");

	//  Behave based on control ID
	switch (control_id)
	{
		//  Color Listbox item changed
		case COLOR_LISTBOX:

			switch (listbox_item_id)
			{
				//  Select black color
				case 1:
					color[0] = 0/255.0;
					color[1] = 0/255.0;
					color[2] = 0/255.0;
					break;
				//  Select blue color
				case 2:
					color[0] = 0/255.0;
					color[1] = 0/255.0;
					color[2] = 255/255.0;
					break;
				//  Select cyan color
				case 3:	
					color[0] = 0/255.0;
					color[1] = 255/255.0;
					color[2] = 255/255.0;
					break;
				//  Select dark grey color
				case 4:
					color[0] = 64/255.0;
					color[1] = 64/255.0;
					color[2] = 64/255.0;
					break;
				//  Select grey color
				case 5:
					color[0] = 128/255.0;
					color[1] = 128/255.0;
					color[2] = 128/255.0;
					break;

			}

			printf ("Item %d selected.\n", listbox_item_id);

		break;

		//  A Radio Button in the radio group is selected
		case OBJECTYPE_RADIOGROUP:
			
			printf ("Radio Button %d selected.\n", radiogroup_item_id);
			
		break;

		//  Translation XY control
		case TRANSLATION_XY:

			printf ("Trasladando X y Y Coordenadas: ");
			printf ("X: %f, Y: %f.\n", translate_xy[0], translate_xy[1]);

			break;

		//  rotation control is being rotated
		case ROTATION:

			printf ("Rotating archball!\n");
			//printMatrixf (rotation_matrix);

		break;

		//  Scaling
		case SCALE_SPINNER:

			printf ("Scaling Object: %f.\n", scale);

		break;

		//  Quit Button clicked
		case QUIT_BUTTON:
		
			printf ("Quit Button clicked... Exit!\n");

			exit (1);

		break;

	}
}


void Sleep(unsigned int mseconds)
{
    clock_t goal = mseconds + clock();
    while (goal > clock());
}
void idle ()
{
	glutSetWindow (main_window);
	glutPostRedisplay ();
	Sleep (5);
	
}



int main (int argc, char **argv)
{

	

	glutInit(&argc, argv);
	centerOnScreen ();
	glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE);	
	glutInitWindowSize (window_width, window_height);
	glutInitWindowPosition(300, 300);
	
	main_window = glutCreateWindow (window_title);

	//  
	//if (full_screen)
	//	glutFullScreen ();

	//   OpenGL Estado Inicial
	init();
	

	glutDisplayFunc (display);
	//glutReshapeFunc  (reshape);
	
	//  Iniciar  GLUI 
	setupGLUI ();

	//  Iniciar GLUT 
	glutMainLoop();
}

