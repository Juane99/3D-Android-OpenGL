//**************************************************************************
// Práctica IG usando objetos
//**************************************************************************

#include <vector>
#include <GL/gl.h>
#include "vertex.h"
#include <stdlib.h>
#include "file_ply_stl.hpp"

using namespace std;

const float AXIS_SIZE=5000;
typedef enum{POINTS,EDGES,SOLID_CHESS,SOLID,SOLID_ILLUMINATED_FLAT,
             SOLID_ILLUMINATED_GOURAUD} _modo;

//*************************************************************************
// clase punto
//*************************************************************************

class _puntos3D
{
public:

  
	_puntos3D();
void 	draw_puntos(float r, float g, float b, int grosor);

vector<_vertex3f> vertices;
};

//*************************************************************************
// clase Material
//*************************************************************************

class _material
{
private:
	_vertex4f mat_ambiental, mat_difusa, mat_especular;
	float brillo;
public:

	_material(_vertex4f ambiental, _vertex4f difusa, _vertex4f especular, float mi_brillo);
	_material();
	_vertex4f getAmbiental();
	_vertex4f getDifusa();
	_vertex4f getEspecular();
	float getBrillo();
	void setAmbiental(_vertex4f amb);
	void setDifusa(_vertex4f dif);
	void setEspecular(_vertex4f esp);
	void setBrillo(float brillazo);
};

//*************************************************************************
// clase triángulo
//*************************************************************************

class _triangulos3D: public _puntos3D
{
public:

	_triangulos3D();
void 	draw_aristas(float r, float g, float b, int grosor);
void    draw_solido(float r, float g, float b);
void 	draw_solido_ajedrez(float r1, float g1, float b1, float r2, float g2, float b2);
void 	draw_iluminacion_plana(_material mat);
void 	draw_iluminacion_suave(_material mat);

void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);

void	calcular_normales_caras();
void 	calcular_normales_vertices();

vector<_vertex3i> caras;

vector<_vertex3f> normales_caras; //Para iluminacion plana
vector<_vertex3f> normales_vertices; //Para iluminacion suave

bool b_normales_caras;
bool b_normales_vertices;

_vertex4f ambiente;     //coeficientes ambiente y difuso
_vertex4f difusa;
_vertex4f especular;           //coeficiente especular
float brillo;                  //exponente del brillo 

};



//*************************************************************************
// clase cubo
//*************************************************************************

class _cubo: public _triangulos3D
{
public:

	_cubo(float tam=0.5);
};


//*************************************************************************
// clase piramide
//*************************************************************************

class _piramide: public _triangulos3D
{
public:

	_piramide(float tam=0.5, float al=0.75);
};

//*************************************************************************
// clase objeto ply
//*************************************************************************

class _objeto_ply: public _triangulos3D
{
public:
   _objeto_ply();

int   parametros(char *archivo);
};

//************************************************************************
// objeto por revolución
//************************************************************************

class _rotacion: public _triangulos3D
{
public:
       _rotacion();
void  parametros(vector<_vertex3f> perfil1, int num1, int tipo, float altura_maxima_pos, float altura_maxima_neg, bool especial);

vector<_vertex3f> perfil; 
int num;
};


//*************************************************************************
// clase cono
//*************************************************************************

class _cono: public _rotacion
{
public:

	_cono(float rad=1.2, float al=1.7);
};


//*************************************************************************
// clase cilindro
//*************************************************************************

class _cilindro: public _rotacion
{
public:

	_cilindro(float rad=1.0, float al=1.0);
};


//*************************************************************************
// clase esfera
//*************************************************************************

class _esfera: public _rotacion
{
public:

	_esfera(float rad=1.5);
};

//*************************************************************************
// clase semiesfera
//*************************************************************************

class _semiesfera: public _rotacion
{
private:
	float radio;
public:
	_semiesfera();
	void setRadio(float rad);
	void create();
};


//*************************************************************************
// clase pildora
//*************************************************************************

class _pildora: public _rotacion
{
public:

	_pildora(float rad=2.0);
void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);

protected:
_rotacion  rodamiento;
_semiesfera  base;
};


//************************************************************************
// objeto articulado: tanque
//************************************************************************

class _chasis: public _triangulos3D
{
public:
       _chasis();
void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);

float altura;

protected:
_rotacion  rodamiento;
_cubo  base;
};

//************************************************************************

class _torreta: public _triangulos3D
{
public:
       _torreta();
void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);

float altura;
float anchura;

protected:
_cubo  base;
_piramide parte_trasera;
};

//************************************************************************

class _tubo: public _triangulos3D
{
public:
       _tubo();
void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);

protected:
_rotacion tubo_abierto; // caña del cañón
};

//************************************************************************

class _tanque: public _triangulos3D
{
public:
       _tanque();
void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);

float giro_tubo;
float giro_torreta;

float giro_tubo_min;
float giro_tubo_max;

protected:
_chasis  chasis;
_torreta  torreta;
_tubo     tubo;
};

//************************************************************************
// objeto articulado: Android
//************************************************************************

class _pata: public _triangulos3D
{
public:
       _pata();
void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);

float altura;

protected:
_pildora  rodamiento;
};

//************************************************************************

class _brazo: public _triangulos3D
{
public:
       _brazo();
void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);

float altura;

protected:
_pildora rodamiento;
};

//************************************************************************

class _cuerpo: public _triangulos3D
{
public:
       _cuerpo();
void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);

protected:
_rotacion cuerpo_ancho; // cuerpo del android
};

//************************************************************************

class _cabeza: public _triangulos3D
{
public:
       _cabeza();
void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);

protected:
_semiesfera cabeza;
_semiesfera ojo;
_rotacion antenas;

};

//************************************************************************

class _mango: public _triangulos3D
{
public:
       _mango();
void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);

protected:
_rotacion cilindro;
_cono conito;
};


//************************************************************************

class _laser: public _triangulos3D
{
public:
       _laser();
void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);

protected:
_rotacion cilindro;
};

//************************************************************************

class _android: public _triangulos3D
{
public:
       _android();
void 	draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);

float giro_cabeza;
float giro_patas;
float giro_brazos;
float salir_laser;
float salir_laser_escalado;

float giro_patas_min;
float giro_patas_max;
float giro_brazos_min;
float giro_brazos_max;
float salir_laser_max;
float salir_laser_min;
float salir_laser_escalado_max;
float salir_laser_escalado_min;


protected:
_brazo brazo_izda;
_brazo brazo_dcha;
_pata pata_izda;
_pata pata_dcha;
_cuerpo cuerpo;
_cabeza cabeza;
_laser laser;
_mango mango;
};


