# Dotar al programa de una tecla que permita cambiar el modo de proyección entre ORTOGONAL y PERSPECTIVA.
Tecla f4 cambio de vista

```c
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
```
# Programar otros modos de movimiento de cámara como son el MODO PAN o el MODO TRÍPODE.

## Para lograr el MODO TRÍPODE he definido una nueva función en el header camera.h
void PitchCamera( camera *thisCamera, float angle ); // local axis Z camera
### Ahora en camera.cpp

```c
void PitchCamera( camera *thisCamera, float angle )
{
    float vIn[3];
    vIn[0]= thisCamera->camAtX - thisCamera->camViewX;
    vIn[1]= thisCamera->camAtY - thisCamera->camViewY;
    vIn[2]= thisCamera->camAtZ - thisCamera->camViewZ;
    VectorRotXZ( vIn, angle, TRUE );
    thisCamera->camAtX = thisCamera->camViewX + vIn[0];
    thisCamera->camAtY = thisCamera->camViewY + vIn[1];
    thisCamera->camAtZ = thisCamera->camViewZ + vIn[2];
    SetDependentParametersCamera( thisCamera );
}
```
### Ahora en main.cpp - SpecialKey(key, x, y) la captura de la tecla F5:
```c
case GLUT_KEY_F5:
      if (current_mode != 0) break;
      current_mode = 3;
      LOCAL_MyCamera->camMovimiento = CAM_TRIPODE;
break;
```
### En main.cpp  función mouse(button, state, x, y) dentro del switch
captamos el estado CAM_TRIPODE
```c
case CAM_TRIPODE:
    if (state == GLUT_DOWN) glutMotionFunc(Tripode);
    if (state == GLUT_UP) glutMotionFunc(NULL);
break;
```
### Que llama a la nueva función Trípode(x, y):
```c
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
```
## Para lograr el MODO PAN también se añade  función que declaramos en camera.h
```c
void PanCamera( camera *thisCamera, float stepX, float stepY );
```
### Ahora en camera.cpp

```c
void PanCamera( camera *thisCamera, float stepX, float stepY )
{
float vaX, vaY, vaZ;
vaX= stepX * thisCamera->camIX + stepY * thisCamera->camJX;
vaY= stepX * thisCamera->camIY + stepY * thisCamera->camJY;
vaZ= stepX * thisCamera->camIZ + stepY * thisCamera->camJZ;

// Set V & A
thisCamera->camViewX=thisCamera->camViewX+ vaX;
thisCamera->camViewY=thisCamera->camViewY+ vaY;
thisCamera->camViewZ=thisCamera->camViewZ+ vaZ;
thisCamera->camAtX=thisCamera->camAtX+vaX;
thisCamera->camAtY=thisCamera->camAtY+vaY;
thisCamera->camAtZ=thisCamera->camAtZ+vaZ;

SetDependentParametersCamera( thisCamera );
}
```

### Activamos el modo PAN en main.cpp la función mouse 
```c
case CAM_PAN:
if (state == GLUT_DOWN) glutMotionFunc(Pan);
if (state == GLUT_UP) glutMotionFunc(NULL);
break;
```

### En main.cpp se declara la función Pan
```c
void Pan(int x, int y){
  float avance_x, avance_y;
  avance_x = (float)(old_x - x) / 10;
  avance_y = (float)(y - old_y) / 10;
  PanCamera( LOCAL_MyCamera, avance_x, avance_y);
  old_y = y;
  old_x = x;
  glutPostRedisplay();
}
```
