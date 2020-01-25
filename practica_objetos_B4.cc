//**************************************************************************
// Práctica 4 usando objetos
//**************************************************************************

#include <GL/glut.h>
#include <ctype.h>
#include <math.h>
#include <vector>
#include "objetos_B4.h"


using namespace std;

// tipos
typedef enum{CUBO, PIRAMIDE, OBJETO_PLY, ROTACION, ARTICULADO, ANDROID, CONO, CILINDRO, ESFERA} _tipo_objeto;
_tipo_objeto t_objeto=CUBO;
_modo   modo=POINTS;

// variables que definen la posicion de la camara en coordenadas polares
GLfloat Observer_distance;
GLfloat Observer_angle_x;
GLfloat Observer_angle_y;

// variables que controlan la ventana y la transformacion de perspectiva
GLfloat Size_x,Size_y,Front_plane,Back_plane;

// variables que determninan la posicion y tamaño de la ventana X
int Window_x=50,Window_y=50,Window_width=450,Window_high=450;

bool rotando_l1 = false, rotando_l2 = false, luz1_activada = false, 
	 luz2_activada = false;
int mi_alfa = 60, mi_beta = 60;

//Velocidad giro
float giro_brazos_speedup = 1.0, salir_laser_speedup = 0.1, giro_cabeza_speedup = 1.0;

int angulo_animacion_cabeza = 0, angulo_animacion_brazos = 0;
float angulo_animacion_laser = 0;

//Todo lo referente al material

vector<_material> materiales_posibles;
_material material_actual;
int pos_material = 0;


// objetos
_cubo cubo;
_piramide piramide(0.85,1.3);
_objeto_ply  ply; 
_rotacion rotacion; 
_tanque tanque;
_android android;
_cono cono;
_cilindro cilindro;
_esfera esfera;

// _objeto_ply *ply1;


//**************************************************************************
//
//***************************************************************************

void rellena_vector_materiales(){
	_vertex4f ambiental, difusa, especular;
	float brillo;
	_material mat;
	
	//Brass
	ambiental = _vertex4f(0.329412,0.223529,0.027451,1.0);
	difusa    = _vertex4f(0.780392,0.568627,0.113725,1.0);
	especular = _vertex4f(0.992157,0.941176,0.807843,1.0);
	brillo    = 27.8974;
	
	mat.setAmbiental(ambiental);
	mat.setDifusa(difusa);
	mat.setEspecular(especular);
	mat.setBrillo(brillo);
	
	materiales_posibles.push_back(mat);
	
	//Bronze
	
	ambiental = _vertex4f(0.2125, 0.1275, 0.054, 1.0);
	difusa    = _vertex4f(0.714, 0.4284, 0.18144, 1.0);
	especular = _vertex4f(0.393548, 0.271906, 0.166721, 1.0);
	brillo    = 25.6;
	
	mat.setAmbiental(ambiental);
	mat.setDifusa(difusa);
	mat.setEspecular(especular);
	mat.setBrillo(brillo);
	
	materiales_posibles.push_back(mat);
	
	//Chrome
	
	ambiental = _vertex4f(0.25, 0.25, 0.25, 1.0);
	difusa    = _vertex4f(0.4, 0.4, 0.4, 1.0);
	especular = _vertex4f(0.774597, 0.774597, 0.774597, 1.0);
	brillo    = 76.8;
	
	mat.setAmbiental(ambiental);
	mat.setDifusa(difusa);
	mat.setEspecular(especular);
	mat.setBrillo(brillo);
	
	materiales_posibles.push_back(mat);
	
	//Tin
	
	ambiental = _vertex4f(0.105882, 0.058824, 0.113725, 1.0);
	difusa    = _vertex4f(0.427451, 0.470588, 0.541176, 1.0);
	especular = _vertex4f(0.333333, 0.333333, 0.521569, 1.0);
	brillo    = 9.84615;
	
	mat.setAmbiental(ambiental);
	mat.setDifusa(difusa);
	mat.setEspecular(especular);
	mat.setBrillo(brillo);
	
	materiales_posibles.push_back(mat);
	
	//Emerald
	
	ambiental = _vertex4f(0.0215, 0.1745, 0.0215, 0.55);
	difusa    = _vertex4f(0.07568, 0.61424, 0.07568, 0.55);
	especular = _vertex4f(0.633, 0.727811, 0.633, 0.55);
	brillo    = 76.8;
	
	mat.setAmbiental(ambiental);
	mat.setDifusa(difusa);
	mat.setEspecular(especular);
	mat.setBrillo(brillo);
	
	materiales_posibles.push_back(mat);
	
	//Ruby
	
	ambiental = _vertex4f(0.1745, 0.01175, 0.01175, 0.55);
	difusa    = _vertex4f(0.61424, 0.04136, 0.04136, 0.55);
	especular = _vertex4f(0.727811, 0.626959, 0.626959, 0.55);
	brillo    = 76.8;
	
	mat.setAmbiental(ambiental);
	mat.setDifusa(difusa);
	mat.setEspecular(especular);
	mat.setBrillo(brillo);
	
	materiales_posibles.push_back(mat);
}
	

void clean_window()
{

glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}


//METODO PARA MODIFICAR EL ANGULO DE LA ILUMINACION

void luces(float alfa, float beta){
	GLfloat light1_position[4]={20,20,0,0},
			light1_ambient[4]={0.1,0.0,0.0,0.1},
			light1_intensity[4]={0.9,0.9,0.9,1};
			
	GLfloat light2_position[4]={20,30,0,0},
			light2_ambient[4]={0.1,0.0,0.0,0.1},
			light2_intensity[4]={0.9,0.9,0.9,1},
			light2_specular[4]={10.0,0.0,0.0,1};
			
	glEnable(GL_LIGHT0);
	
	//Primera luz
	
	glLightfv(GL_LIGHT1,GL_POSITION,light1_position);
	glLightfv(GL_LIGHT1,GL_AMBIENT,light1_ambient);
	glLightfv(GL_LIGHT1,GL_DIFFUSE,light1_intensity);
	glLightfv(GL_LIGHT1,GL_SPECULAR,light1_intensity);
	
	if (luz1_activada)
		glEnable(GL_LIGHT1);
	else
		glDisable(GL_LIGHT1);
	
	glPushMatrix();
	glLoadIdentity();
	glRotatef(alfa,0,1,0);
	glLightfv(GL_LIGHT1,GL_POSITION,light1_position);
	glPopMatrix();	
	
	
	//Segunda luz	
	glLightfv(GL_LIGHT2,GL_POSITION,light2_position);
	glLightfv(GL_LIGHT2,GL_AMBIENT,light2_ambient);
	glLightfv(GL_LIGHT2,GL_DIFFUSE,light2_intensity);
	glLightfv(GL_LIGHT2,GL_SPECULAR,light2_specular);
	
	if (luz2_activada)
		glEnable(GL_LIGHT2);
	else
		glDisable(GL_LIGHT2);
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glRotatef(beta,0,1,0);
	glLightfv(GL_LIGHT2,GL_POSITION,light2_position);
	glPopMatrix();
}

//**************************************************************************
// Metodo para rotar una de las luces
//***************************************************************************

void funcion_idle(){
	if (rotando_l1)
		mi_alfa += 10;
		
	if (rotando_l2)
		mi_beta += 10;
		
	glutPostRedisplay();
}

//**************************************************************************
// Funcion para definir la transformación de proyeccion
//***************************************************************************

void change_projection()
{

glMatrixMode(GL_PROJECTION);
glLoadIdentity();

// formato(x_minimo,x_maximo, y_minimo, y_maximo,plano_delantero, plano_traser)
//  plano_delantero>0  plano_trasero>PlanoDelantero)
glFrustum(-Size_x,Size_x,-Size_y,Size_y,Front_plane,Back_plane);
}

//**************************************************************************
// Funcion para definir la transformación*ply1 de vista (posicionar la camara)
//***************************************************************************

void change_observer()
{

// posicion del observador
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
glTranslatef(0,0,-Observer_distance);
glRotatef(Observer_angle_x,1,0,0);
glRotatef(Observer_angle_y,0,1,0);
}

//**************************************************************************
// Funcion que dibuja los ejes utilizando la primitiva grafica de lineas
//***************************************************************************

void draw_axis()
{
	
glDisable(GL_LIGHTING);
glLineWidth(2);
glBegin(GL_LINES);
// eje X, color rojo
glColor3f(1,0,0);
glVertex3f(-AXIS_SIZE,0,0);
glVertex3f(AXIS_SIZE,0,0);
// eje Y, color verde
glColor3f(0,1,0);
glVertex3f(0,-AXIS_SIZE,0);
glVertex3f(0,AXIS_SIZE,0);
// eje Z, color azul
glColor3f(0,0,1);
glVertex3f(0,0,-AXIS_SIZE);
glVertex3f(0,0,AXIS_SIZE);
glEnd();
}


//**************************************************************************
// Funcion que dibuja los objetos
//****************************2***********************************************

void draw_objects()
{

switch (t_objeto){
	case CUBO: cubo.draw(modo,1.0,0.0,0.0,0.0,1.0,0.0,2);break;
	case PIRAMIDE: piramide.draw(modo,1.0,0.0,0.0,0.0,1.0,0.0,2);break;
        case OBJETO_PLY: ply.draw(modo,1.0,0.6,0.0,0.0,1.0,0.3,2);break;
        case ROTACION: rotacion.draw(modo,1.0,0.0,0.0,0.0,1.0,0.0,2);break;
        case ARTICULADO: tanque.draw(modo,0.5,0.7,0.2,0.3,0.6,0.3,2);break;
        case ANDROID: android.draw(modo,0.5,0.7,0.2,0.3,0.6,0.3,2);break;
        case CONO: cono.draw(modo,0.5,0.7,0.2,0.3,0.6,0.3,2);break;
        case CILINDRO: cilindro.draw(modo,0.5,0.7,0.2,0.3,0.6,0.3,2);break;
        case ESFERA: esfera.draw(modo,0.5,0.7,0.2,0.3,0.6,0.3,2);break;
	}

}


//**************************************************************************
//
//***************************************************************************

void draw(void)
{

	clean_window();
	change_observer();
	luces(mi_alfa, mi_beta);
	draw_axis();
	draw_objects();
	glutSwapBuffers();
}



//***************************************************************************
// Funcion llamada cuando se produce un cambio en el tamaño de la ventana
//
// el evento manda a la funcion:
// nuevo ancho
// nuevo alto
//***************************************************************************

void change_window_size(int Ancho1,int Alto1)
{
float Aspect_ratio;

Aspect_ratio=(float) Alto1/(float )Ancho1;
Size_y=Size_x*Aspect_ratio;
change_projection();
glViewport(0,0,Ancho1,Alto1);
glutPostRedisplay();
}


//**********-o*****************************************************************
// Funcion llamada cuando se aprieta una tecla normal
//
// el evento manda a la funcion:
// codigo de la tecla
// posicion x del raton
// posicion y del raton
//***************************************************************************

void normal_key(unsigned char Tecla1,int x,int y)
{
switch (toupper(Tecla1)){
	case '1':modo=POINTS;break;
	case '2':modo=EDGES;break;
	case '3':modo=SOLID;break;
	case '4':modo=SOLID_CHESS;break;
	case '5':modo=SOLID_ILLUMINATED_FLAT;break;
	case '6':modo=SOLID_ILLUMINATED_GOURAUD;break;
	case '7':
			 if (!luz1_activada)
					luz1_activada = true;
			 else
					luz1_activada = false;
	break;
	
	case '8':
			 if (!luz2_activada)
					luz2_activada = true;
			 else
					luz2_activada = false;
	break;
	
        case 'P':t_objeto=PIRAMIDE;break;
        case 'C':t_objeto=CUBO;break;
        case 'O':t_objeto=OBJETO_PLY;break;	
        case 'N':t_objeto=ROTACION;break;
        case 'B':t_objeto=ARTICULADO;break;
        case 'A':t_objeto=ANDROID;break;
        case 'F':t_objeto=CONO;break;
        case 'G':t_objeto=CILINDRO;break;
        case 'H':t_objeto=ESFERA;break;
        case 'M':pos_material++;
				 material_actual = materiales_posibles[pos_material % 6];break;
        
         //Android
    case 'Q':android.giro_brazos+=giro_brazos_speedup;
        if (android.giro_brazos>android.giro_brazos_max) android.giro_brazos=android.giro_brazos_max;
        break;
    case 'W':android.giro_brazos-=giro_brazos_speedup;
        if (android.giro_brazos<android.giro_brazos_min) android.giro_brazos=android.giro_brazos_min;
        break;
    case 'S':
		android.salir_laser+=salir_laser_speedup;
        if (android.salir_laser>android.salir_laser_max) android.salir_laser=android.salir_laser_max;
        break;
    case 'D':
		android.salir_laser-=salir_laser_speedup;
		if (android.salir_laser<android.salir_laser_min) android.salir_laser=android.salir_laser_min;
        break;
    case 'Z':android.giro_cabeza+=giro_cabeza_speedup;break;
    case 'X':android.giro_cabeza-=giro_cabeza_speedup;break;
    
    //Velocidades
    
    case 'E':angulo_animacion_brazos++; break;
    case 'R':angulo_animacion_brazos--; break;
    case 'T':angulo_animacion_laser+=0.1; break;
    case 'Y':angulo_animacion_laser-=0.1; break;
    case 'U':angulo_animacion_cabeza++; break;
    case 'I':angulo_animacion_cabeza--; break;
	}
glutPostRedisplay();
}

//***************************************************************************
// Funcion l-olamada cuando se aprieta una tecla especial
//
// el evento manda a la funcion:
// codigo de la tecla
// posicion x del raton
// posicion y del raton

//***************************************************************************

void special_key(int Tecla1,int x,int y)
{

switch (Tecla1){
	case GLUT_KEY_LEFT:Observer_angle_y--;break;
	case GLUT_KEY_RIGHT:Observer_angle_y++;break;
	case GLUT_KEY_UP:Observer_angle_x--;break;
	case GLUT_KEY_DOWN:Observer_angle_x++;break;
	case GLUT_KEY_PAGE_UP:Observer_distance*=1.2;break;
	case GLUT_KEY_PAGE_DOWN:Observer_distance/=1.2;break;
        case GLUT_KEY_F1:tanque.giro_tubo+=1;
                         if (tanque.giro_tubo>tanque.giro_tubo_max) tanque.giro_tubo=tanque.giro_tubo_max;
                         break;
        case GLUT_KEY_F2:tanque.giro_tubo-=1;
                         if (tanque.giro_tubo<tanque.giro_tubo_min) tanque.giro_tubo=tanque.giro_tubo_min;
                         break;break;
        case GLUT_KEY_F3:tanque.giro_torreta+=5;break;
        case GLUT_KEY_F4:tanque.giro_torreta-=5;break;
        case GLUT_KEY_F5:
						 if (!rotando_l1)
							rotando_l1 = true;
						 else
							rotando_l1 = false;
		break;
		
        case GLUT_KEY_F6:
						 if (!rotando_l2)
							rotando_l2 = true;
						 else
							rotando_l2 = false;
		break;
		
		case GLUT_KEY_F7: 
						  if (material_actual.getBrillo() < 100) 
							material_actual.setBrillo(material_actual.getBrillo() + 1.0); 
		break;
		case GLUT_KEY_F8: 
						  if (material_actual.getBrillo() > 0)
							material_actual.setBrillo(material_actual.getBrillo() - 1.0); 
		break;
	}
glutPostRedisplay();
}



//***************************************************************************
// Funcion de incializacion
//***************************************************************************

void initialize(void)
{

// se inicalizan la ventana y los planos de corte
Size_x=0.5;
Size_y=0.5;
Front_plane=1;
Back_plane=1000;

// se incia la posicion del observador, en el eje z
Observer_distance=4*Front_plane;
Observer_angle_x=0;
Observer_angle_y=0;

// se indica cua*ply1l sera el color para limpiar la ventana	(r,v,a,al)
// blanco=(1,1,1,1) rojo=(1,0,0,1), ...
glClearColor(1,1,1,1);

// se habilita el z-bufer
glEnable(GL_DEPTH_TEST);
change_projection();
glViewport(0,0,Window_width,Window_high);



}


//***************************************************************************
// Programa principal
//
// Se encarga de iniciar la ventana, asignar las funciones e comenzar el
// bucle de eventos
//***************************************************************************


int main(int argc, char **argv)
{
rellena_vector_materiales();
material_actual = materiales_posibles[pos_material];

// creación del objeto ply

ply.parametros(argv[1]);


// perfil 

vector<_vertex3f> perfil2;
_vertex3f aux;
aux.x=1.0;aux.y=-1.4;aux.z=0.0;
perfil2.push_back(aux);
aux.x=1.0;aux.y=-1.1;aux.z=0.0;
perfil2.push_back(aux);
aux.x=0.5;aux.y=-0.7;aux.z=0.0;
perfil2.push_back(aux);
aux.x=0.4;aux.y=-0.4;aux.z=0.0;
perfil2.push_back(aux);
aux.x=0.4;aux.y=0.5;aux.z=0.0;
perfil2.push_back(aux);
aux.x=0.5;aux.y=0.6;aux.z=0.0;
perfil2.push_back(aux);
aux.x=0.3;aux.y=0.6;aux.z=0.0;
perfil2.push_back(aux);
aux.x=0.5;aux.y=0.8;aux.z=0.0;
perfil2.push_back(aux);
aux.x=0.55;aux.y=1.0;aux.z=0.0;
perfil2.push_back(aux);
aux.x=0.5;aux.y=1.2;aux.z=0.0;
perfil2.push_back(aux);
aux.x=0.3;aux.y=1.4;aux.z=0.0;
perfil2.push_back(aux);
rotacion.parametros(perfil2,6,1,0,0,false);


// se llama a la inicialización de glut
glutInit(&argc, argv);

// se indica las caracteristicas que se desean para la visualización con OpenGL
// Las posibilidades son:
// GLUT_SIMPLE -> memoria de imagen simple
// GLUT_DOUBLE -> memoria de imagen doble
// GLUT_INDEX -> memoria de imagen con color indizado
// GLUT_RGB -> memoria de imagen con componentes rojo, verde y azul para cada pixel
// GLUT_RGBA -> memoria de imagen con componentes rojo, verde, azul y alfa para cada pixel
// GLUT_DEPTH -> memoria de profundidad o z-bufer
// GLUT_STENCIL -> memoria de estarcido_rotation Rotation;
glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

// posicion de la esquina inferior izquierdad de la ventana
glutInitWindowPosition(Window_x,Window_y);

// tamaño de la ventana (ancho y alto)
glutInitWindowSize(Window_width,Window_high);

// llamada para crear la ventana, indicando el titulo (no se visualiza hasta que se llama
// al bucle de eventos)
glutCreateWindow("PRACTICA - 4");

// asignación de la funcion llamada "dibujar" al evento de dibujo
glutDisplayFunc(draw);
// asignación de la funcion llamada "change_window_size" al evento correspondiente
glutReshapeFunc(change_window_size);
// asignación de la funcion llamada "normal_key" al evento correspondiente
glutKeyboardFunc(normal_key);
// asignación de la funcion llamada "tecla_Especial" al evento correspondiente
glutSpecialFunc(special_key);
glutIdleFunc(funcion_idle);

// funcion de inicialización
initialize();

// inicio del bucle de eventos
glutMainLoop();
return 0;
}
