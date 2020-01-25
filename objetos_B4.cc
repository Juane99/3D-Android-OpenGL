//**************************************************************************
// Práctica 4 iluminacion
//**************************************************************************

#include "objetos_B4.h"

extern _material material_actual;


//*************************************************************************
// _puntos3D
//*************************************************************************

_puntos3D::_puntos3D()
{
}

//*************************************************************************
// dibujar puntos
//*************************************************************************

void _puntos3D::draw_puntos(float r, float g, float b, int grosor)
{
//**** usando vertex_array ****
glPointSize(grosor);
glColor3f(r,g,b);

glEnableClientState(GL_VERTEX_ARRAY);
glVertexPointer(3,GL_FLOAT,0,&vertices[0]);
glDrawArrays(GL_POINTS,0,vertices.size()); 

/*int i;
glPointSize(grosor);
glColor3f(r,g,b);
glBegin(GL_POINTS);
for (i=0;i<vertices.size();i++){
	glVertex3fv((GLfloat *) &vertices[i]);
	}
glEnd();*/
}


//*************************************************************************
// _triangulos3D
//*************************************************************************

_triangulos3D::_triangulos3D()
{
	
	b_normales_caras=false;
	b_normales_vertices=false;
	ambiente=_vertex4f(0.2,0.4,0.9,1.0);
	difusa=_vertex4f(0.2,0.4,0.9,1.0);
	especular=_vertex4f(0.7,0.7,0.7,1.0);
	brillo=40;
}


//*************************************************************************
// dibujar en modo arista
//*************************************************************************

void _triangulos3D::draw_aristas(float r, float g, float b, int grosor)
{
//**** usando vertex_array ****
glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
glLineWidth(grosor);
glColor3f(r,g,b);

glEnableClientState(GL_VERTEX_ARRAY);
glVertexPointer(3,GL_FLOAT,0,&vertices[0]);
glDrawElements(GL_TRIANGLES,caras.size()*3,GL_UNSIGNED_INT,&caras[0]);

/*int i;
glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
glLineWidth(grosor);
glColor3f(r,g,b);
glBegin(GL_TRIANGLES);
for (i=0;i<caras.size();i++){
	glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
	}
glEnd();*/
}

//*************************************************************************
// dibujar en modo sólido
//*************************************************************************

void _triangulos3D::draw_solido(float r, float g, float b)
{
int i;

glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
glColor3f(r,g,b);
glBegin(GL_TRIANGLES);
for (i=0;i<caras.size();i++){
	glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
	}
glEnd();
}

//*************************************************************************
// dibujar en modo sólido con apariencia de ajedrez
//*************************************************************************

void _triangulos3D::draw_solido_ajedrez(float r1, float g1, float b1, float r2, float g2, float b2)
{
int i;
glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
glBegin(GL_TRIANGLES);
for (i=0;i<caras.size();i++){
	if (i%2==0) glColor3f(r1,g1,b1);
	else glColor3f(r2,g2,b2);
	glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
	glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
	}
glEnd();
}

//*************************************************************************
// dibujar con distintos modos
//*************************************************************************

void _triangulos3D::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor)
{
switch (modo){
	case POINTS:draw_puntos(r1, g1, b1, grosor);break;
	case EDGES:draw_aristas(r1, g1, b1, grosor);break;
	case SOLID_CHESS:draw_solido_ajedrez(r1, g1, b1, r2, g2, b2);break;
	case SOLID:draw_solido(r1, g1, b1);break;
	case SOLID_ILLUMINATED_FLAT:draw_iluminacion_plana(material_actual);break;
	case SOLID_ILLUMINATED_GOURAUD:draw_iluminacion_suave(material_actual);break;
	}
}


//*************************************************************************
// calcular las normales a las caras
//*************************************************************************
void _triangulos3D::calcular_normales_caras(){

	normales_caras.resize(caras.size());

	for (unsigned long i = 0; i < caras.size(); i++){

		_vertex3f
			a1=vertices[caras[i]._1]-vertices[caras[i]._0],
			a2=vertices[caras[i]._2]-vertices[caras[i]._0],	
			n = a1.cross_product(a2);
			
			float m=sqrt(n.x*n.x+n.y*n.y+n.z*n.z);
			
			normales_caras[i] = _vertex3f(n.x/m, n.y/m, n.z/m);
	}

	b_normales_caras = true;
}


//*************************************************************************
// Dibujar el objeto con iluminacion plana (solo caras)
//*************************************************************************
void _triangulos3D::draw_iluminacion_plana(_material mat){
	int i;
	_vertex4f amb = mat.getAmbiental(), dif = mat.getDifusa(), 
			  esp = mat.getEspecular();
			  
	float brill = mat.getBrillo();
	
	if (b_normales_caras==false) calcular_normales_caras();
	
	//glEnable(GL_CULL_FACE);
	
	glShadeModel(GL_FLAT); 
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT, (GLfloat *) &amb);
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, (GLfloat *) &dif);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,(GLfloat *) &esp);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS, brill);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_TRIANGLES);
	for (int i=0; i<caras.size(); i++){
		glNormal3fv((GLfloat *) &normales_caras[i]);
		glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
		glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
		glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
	}
	
	glEnd();
	glDisable(GL_LIGHTING);
	//glDisable(GL_CULL_FACE);
}


//*************************************************************************
// calcular las normales a los vertices
//*************************************************************************
void _triangulos3D::calcular_normales_vertices(){ //La normal de un vertice es la suma de las normales de las caras que lo comparten

	if (b_normales_caras==false) calcular_normales_caras();

	normales_vertices.resize(vertices.size());
	_vertex3f vertice;
	vertice.x = 0.0;
	vertice.y = 0.0;
	vertice.z = 0.0;
	
	for (int i=0; i<vertices.size(); i++)
		normales_vertices[i] = vertice;


	for (unsigned long i = 0; i < caras.size(); i++){
		normales_vertices[caras[i]._2] += normales_caras[i];
		normales_vertices[caras[i]._1] += normales_caras[i];
		normales_vertices[caras[i]._0] += normales_caras[i];
	}
	
	for (int i=0; i<vertices.size(); i++)
		normales_vertices[i].normalize();

	b_normales_vertices = true;
}


//*************************************************************************
// Dibujar el objeto con iluminacion suave 
//*************************************************************************
void _triangulos3D::draw_iluminacion_suave(_material mat){
	int i;
	_vertex4f amb = mat.getAmbiental(), dif = mat.getDifusa(), 
			  esp = mat.getEspecular();
			  
	float brill = mat.getBrillo();	

	if (b_normales_vertices==false) calcular_normales_vertices();
	
	//glEnable(GL_CULL_FACE);
	
	glShadeModel(GL_SMOOTH); //En el suave se usa GL_SMOOTH
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT, (GLfloat *) &amb);
	glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE, (GLfloat *) &dif);
	glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR, (GLfloat *) &esp);
	glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS, brill);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_TRIANGLES);
	for (int i=0; i<caras.size(); i++){
		glNormal3fv((GLfloat *) &normales_vertices[caras[i]._0]);
		glVertex3fv((GLfloat *) &vertices[caras[i]._0]);
		
		glNormal3fv((GLfloat *) &normales_vertices[caras[i]._1]);
		glVertex3fv((GLfloat *) &vertices[caras[i]._1]);
		
		glNormal3fv((GLfloat *) &normales_vertices[caras[i]._2]);
		glVertex3fv((GLfloat *) &vertices[caras[i]._2]);
	}
	
	glEnd();
	glDisable(GL_LIGHTING);
	//glDisable(GL_CULL_FACE);
}


//*************************************************************************
// clase Material
//*************************************************************************

_material::_material(){
}

_material::_material(_vertex4f ambiental, _vertex4f difusa, _vertex4f especular, float mi_brillo)
{
	mat_ambiental = ambiental;
	mat_difusa = difusa;
	mat_especular = especular;
	brillo = mi_brillo;
}

_vertex4f _material::getAmbiental(){
	return mat_ambiental;
}

_vertex4f _material::getDifusa(){
	return mat_difusa;
}

_vertex4f _material::getEspecular(){
	return mat_especular;
}

float _material::getBrillo(){
	return brillo;
}

void _material::setAmbiental(_vertex4f amb){
	mat_ambiental = amb;
}

void _material::setDifusa(_vertex4f dif){
	mat_difusa = dif;
}

void _material::setEspecular(_vertex4f esp){
	mat_especular = esp;
}

void _material::setBrillo(float brillazo){
	brillo = brillazo;
}

//*************************************************************************
// clase cubo
//*************************************************************************

_cubo::_cubo(float tam)
{
//vertices
vertices.resize(8);
vertices[0].x=-tam;vertices[0].y=-tam;vertices[0].z=tam;
vertices[1].x=tam;vertices[1].y=-tam;vertices[1].z=tam;
vertices[2].x=tam;vertices[2].y=tam;vertices[2].z=tam;
vertices[3].x=-tam;vertices[3].y=tam;vertices[3].z=tam;
vertices[4].x=-tam;vertices[4].y=-tam;vertices[4].z=-tam;
vertices[5].x=tam;vertices[5].y=-tam;vertices[5].z=-tam;
vertices[6].x=tam;vertices[6].y=tam;vertices[6].z=-tam;
vertices[7].x=-tam;vertices[7].y=tam;vertices[7].z=-tam;

// triangulos
caras.resize(12);
caras[0]._0=0;caras[0]._1=1;caras[0]._2=3;
caras[1]._0=3;caras[1]._1=1;caras[1]._2=2;
caras[2]._0=1;caras[2]._1=5;caras[2]._2=2;
caras[3]._0=5;caras[3]._1=6;caras[3]._2=2;
caras[4]._0=5;caras[4]._1=4;caras[4]._2=6;
caras[5]._0=4;caras[5]._1=7;caras[5]._2=6;
caras[6]._0=0;caras[6]._1=7;caras[6]._2=4;
caras[7]._0=0;caras[7]._1=3;caras[7]._2=7;
caras[8]._0=3;caras[8]._1=2;caras[8]._2=7;
caras[9]._0=2;caras[9]._1=6;caras[9]._2=7;
caras[10]._0=0;caras[10]._1=1;caras[10]._2=4;
caras[11]._0=1;caras[11]._1=5;caras[11]._2=4;  
}


//*************************************************************************
// clase piramide
//*************************************************************************

_piramide::_piramide(float tam, float al)
{

//vertices 
vertices.resize(5); 
vertices[0].x=-tam;vertices[0].y=0;vertices[0].z=tam;
vertices[1].x=tam;vertices[1].y=0;vertices[1].z=tam;
vertices[2].x=tam;vertices[2].y=0;vertices[2].z=-tam;
vertices[3].x=-tam;vertices[3].y=0;vertices[3].z=-tam;
vertices[4].x=0;vertices[4].y=al;vertices[4].z=0;

caras.resize(6);
caras[0]._0=0;caras[0]._1=1;caras[0]._2=4;
caras[1]._0=1;caras[1]._1=2;caras[1]._2=4;
caras[2]._0=2;caras[2]._1=3;caras[2]._2=4;
caras[3]._0=3;caras[3]._1=0;caras[3]._2=4;
caras[4]._0=3;caras[4]._1=1;caras[4]._2=0;
caras[5]._0=3;caras[5]._1=2;caras[5]._2=1;
}

//*************************************************************************
// clase objeto ply
//*************************************************************************


_objeto_ply::_objeto_ply() 
{
   // leer lista de coordenadas de vértices y lista de indices de vértices
 
}



int _objeto_ply::parametros(char *archivo)
{
int n_ver,n_car;

vector<float> ver_ply ;
vector<int>   car_ply ;
   
_file_ply::read(archivo, ver_ply, car_ply );

n_ver=ver_ply.size()/3;
n_car=car_ply.size()/3;

printf("Number of vertices=%d\nNumber of faces=%d\n", n_ver, n_car);

//if (n_ver<3 || n_car<1){
//	printf("Error %d %d\n",__FILE__,__LINE__);
//	exit(-1);
//	}

vertices.resize(n_ver);
caras.resize(n_car);

_vertex3f ver_aux;
_vertex3i car_aux;

for (int i=0;i<n_ver;i++)
	{ver_aux.x=ver_ply[i*3];
	 ver_aux.y=ver_ply[i*3+1];
	 ver_aux.z=ver_ply[i*3+2];
	 vertices[i]=ver_aux;
	}

for (int i=0;i<n_car;i++)
	{car_aux.x=car_ply[i*3];
	 car_aux.y=car_ply[i*3+1];
	 car_aux.z=car_ply[i*3+2];
	 caras[i]=car_aux;
	}

return(0);
}


//************************************************************************
// objeto por revolucion
//************************************************************************

_rotacion::_rotacion()
{

}


void _rotacion::parametros(vector<_vertex3f> perfil, int num, int tipo, float altura_maxima_pos, float altura_maxima_neg, bool especial)
{
int i,j;
int num_aux;
float incremento = M_PI/40;
_vertex3f vertice_aux;
_vertex3i cara_aux;
_vertex3i tapa_aux;

// tratamiento de los vértice
//Si el valor del primer y/o ultimo punto de x es proximo a 0, lo borramos
num_aux=perfil.size();
vertices.resize((num_aux*num));
caras.resize((num_aux-1)*2*num+2*num);

for (j=0;j<num;j++){
	for (i=0;i<num_aux;i++){
		 vertice_aux.x=perfil[i].x*cos(2.0*M_PI*j/(1.0*num))+
					perfil[i].z*sin(2.0*M_PI*j/(1.0*num));
		 vertice_aux.z=-perfil[i].x*sin(2.0*M_PI*j/(1.0*num))+
					perfil[i].z*cos(2.0*M_PI*j/(1.0*num));
		 vertice_aux.y=perfil[i].y;
		 vertices[i+j*num_aux]=vertice_aux;
	}
}

// tratamiento de las caras 
//Hay que unir los vertices que se van creando e ir formando caras
//El tratamiento del cono es diferente a los demas

int l=0;

if (tipo == 1 || tipo == 2){ 
	for (j=0;j<num;j++)
	  {for (i=0;i<num_aux-1;i++){	
		  cara_aux._0=i+((j+1)%num)*num_aux;
		  cara_aux._1=i+1+((j+1)%num)*num_aux;
		  cara_aux._2=i+1+j*num_aux;
		  caras.push_back(cara_aux);
      
		  cara_aux._0=i+1+j*num_aux;
		  cara_aux._1=i+j*num_aux;
		  cara_aux._2=i+((j+1)%num)*num_aux;
		  caras.push_back(cara_aux);
	  }
	}
}
else if (tipo == 0){ //CONO
	
	vertice_aux.x = 0.0; vertice_aux.y = altura_maxima_pos; vertice_aux.z = 0.0;
	vertices.push_back(vertice_aux);
	
	for (int j=0; j<vertices.size(); j++){
		cara_aux._0=(j+1)%vertices.size(); cara_aux._1=vertices.size()-1; cara_aux._2=j;
		caras.push_back(cara_aux);	
	}
	//Aniadimos la ultima capa a mano
	cara_aux._0=0; cara_aux._1=vertices.size()-1; cara_aux._2=9;
	caras.push_back(cara_aux);	
}

 // tapa inferior
if (fabs(perfil[0].x)>0.0){    
	
	_vertex3f central;
	int vertice_central;
	vector<int> vertices_factibles;
	
	central.x = 0.0; central.y = altura_maxima_neg; central.z = 0.0;
	
	for (int i=0; i<vertices.size(); i++){
		
		if (vertices[i].y == central.y)
			vertices_factibles.push_back(i);
	}
	
	vertices.push_back(central);
	vertice_central = vertices.size()-1;	
	
	for (int j=0; j<vertices_factibles.size(); j++){
		tapa_aux._0=vertices_factibles[j]; tapa_aux._1=vertice_central; tapa_aux._2=vertices_factibles[(j+1)%vertices_factibles.size()];
		caras.push_back(tapa_aux);
	}
}
 
 // tapa superior
if ((fabs(perfil[num_aux-1].x)>0.0) && !especial){
	
	_vertex3f central;
	int vertice_central;
	vector<int> vertices_factibles;
	
	central.x = 0.0; central.y = altura_maxima_pos; central.z = 0.0;
		
	for (int i=0; i<vertices.size(); i++){
		
		if (vertices[i].y == central.y)
			vertices_factibles.push_back(i);
	}
	
	vertices.push_back(central);
	vertice_central = vertices.size()-1;
	
	for (int j=0; j<vertices_factibles.size(); j++){
		tapa_aux._0=vertices_factibles[(j+1)%vertices_factibles.size()]; tapa_aux._1=vertice_central; tapa_aux._2=vertices_factibles[j];
		caras.push_back(tapa_aux);	
	}
}
}

//*************************************************************************
// clase cono
//*************************************************************************

_cono::_cono(float rad, float al){
	
	vector<_vertex3f> perfil;
	_vertex3f aux;
	
	aux.x=rad; aux.y=0.0; aux.z=0.0;
	perfil.push_back(aux);
	
	parametros(perfil, 10, 0, al, 0, false);
}


//*************************************************************************
// clase cilindro
//*************************************************************************

_cilindro::_cilindro(float rad, float al){

	vector<_vertex3f> perfil;
	_vertex3f aux;
	
	aux.x=rad; aux.y=-al; aux.z=0.0;
	perfil.push_back(aux);
	aux.x=rad; aux.y=al; aux.z=0.0;
	perfil.push_back(aux);
	
	parametros(perfil, 10, 1, al, -al, false);
}


//*************************************************************************
// clase esfera
//*************************************************************************

_esfera::_esfera(float rad){

	vector<_vertex3f> perfil;
	_vertex3f aux;
	float incremento = M_PI/60;
	
	for(float i=M_PI;i>0;i-=incremento){
		aux.z=0.0;
        aux.x=(rad)*((float) sin(i));
        aux.y=(rad)*((float) cos(i));
        perfil.push_back(aux);
	}

	parametros(perfil, 30, 2, rad, -rad, false);
}

//*************************************************************************
// clase semiesfera
//*************************************************************************

_semiesfera::_semiesfera(){

}

void _semiesfera::create(){
	vector<_vertex3f> perfil;
	_vertex3f aux;
	float incremento = M_PI/60;
	
	for(float i=M_PI/2;i>0.0;i-=incremento){
		aux.z=0.0;
        aux.x=(radio)*((float) sin(i));
        aux.y=(radio)*((float) cos(i));
        perfil.push_back(aux);
	}

	parametros(perfil, 30, 2, radio, -radio, false);
}

void _semiesfera::setRadio(float rad){
	radio = rad;
}


//*************************************************************************
// clase pildora
//*************************************************************************

_pildora::_pildora(float rad){

vector<_vertex3f> perfil;
_vertex3f aux;
aux.x=0.2;aux.y=-1.0;aux.z=0.0;
perfil.push_back(aux);
aux.x=0.2;aux.y=0.0;aux.z=0.0;
perfil.push_back(aux);
rodamiento.parametros(perfil,12,1,rad,-rad,false);

};

void _pildora::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor)
{
glPushMatrix();

rodamiento.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
glPopMatrix();

glPushMatrix();
glTranslatef(0.0,0.0,0.0);
base.setRadio(0.2);
base.create();
base.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
glPopMatrix();

glPushMatrix();
glTranslatef(0.0,-1.0,0.0);
glRotatef(180,1.0,0.0,0.0);
base.setRadio(0.2);
base.create();
base.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
glPopMatrix();

}


//************************************************************************
// objeto articulado: tanque
//************************************************************************

_chasis::_chasis()
{
// perfil para un cilindro
vector<_vertex3f> perfil;
_vertex3f aux;
aux.x=0.107;aux.y=-0.5;aux.z=0.0;
perfil.push_back(aux);
aux.x=0.107;aux.y=0.5;aux.z=0.0;
perfil.push_back(aux);
rodamiento.parametros(perfil,12,1,0,0,false);
altura=0.22;
};

void _chasis::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor)
{
glPushMatrix();
glScalef(1.0,0.22,0.95);
base.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
glPopMatrix();

glPushMatrix();
glRotatef(90.0,1,0,0);
rodamiento.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
glPopMatrix();

glPushMatrix();
glTranslatef(-0.25,0.0,0.0);
glRotatef(90.0,1,0,0);
rodamiento.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
glPopMatrix();

glPushMatrix();
glTranslatef(-0.5,0.0,0.0);
glRotatef(90.0,1,0,0);
rodamiento.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
glPopMatrix();

glPushMatrix();
glTranslatef(0.25,0.0,0.0);
glRotatef(90.0,1,0,0);
rodamiento.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
glPopMatrix();

glPushMatrix();
glTranslatef(0.5,0.0,0.0);
glRotatef(90.0,1,0,0);
rodamiento.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
glPopMatrix();
}

//************************************************************************

_torreta::_torreta()
{
altura=0.18;
anchura=0.65;
};

void _torreta::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor)
{
glPushMatrix();
glScalef(0.65,0.18,0.6);
base.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
glPopMatrix();

glPushMatrix();
glTranslatef(-0.325,0,0);
glRotatef(90.0,0,0,1);
glScalef(0.18,0.16,0.6);
parte_trasera.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
glPopMatrix();
}

//************************************************************************

_tubo::_tubo()
{
// perfil para un cilindro
vector<_vertex3f> perfil;
_vertex3f aux;
aux.x=0.04;aux.y=-0.4;aux.z=0.0;
perfil.push_back(aux);
aux.x=0.04;aux.y=0.4;aux.z=0.0;
perfil.push_back(aux);
tubo_abierto.parametros(perfil,12,0,0,0,false);
};

void _tubo::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor)
{

glPushMatrix();
glTranslatef(0.4,0,0);
glRotatef(90.0,0,0,1);
tubo_abierto.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
glPopMatrix();
}


//************************************************************************

_tanque::_tanque()
{
giro_tubo=2.0;
giro_torreta=0.0;
giro_tubo_min=-9;
giro_tubo_max=20;
};

void _tanque::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor)
{
glPushMatrix();
chasis.draw(modo, r1, g1, b1, r2, g2, b2, grosor);

glRotatef(giro_torreta,0,1,0);
glPushMatrix();
glTranslatef(0.0,(chasis.altura+torreta.altura)/2.0,0.0);
torreta.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
glPopMatrix();

glPushMatrix();
glTranslatef(torreta.anchura/2.0,(chasis.altura+torreta.altura)/2.0,0.0);
glRotatef(giro_tubo,0,0,1);
tubo.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
glPopMatrix();
glPopMatrix();

};




//************************************************************************
// objeto articulado: android
//************************************************************************

_pata::_pata()
{
	altura = 0.5;
};

void _pata::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor)
{
glPushMatrix();
rodamiento.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
glPopMatrix();

}

//************************************************************************

_brazo::_brazo()
{
altura=0.5;
};

void _brazo::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor)
{
glPushMatrix();
rodamiento.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
glPopMatrix();
}

//************************************************************************

_cuerpo::_cuerpo()
{
// perfil para un cilindro
vector<_vertex3f> perfil;
_vertex3f aux;
aux.x=0.4;aux.y=-0.4;aux.z=0.0;
perfil.push_back(aux);
aux.x=0.4;aux.y=0.4;aux.z=0.0;
perfil.push_back(aux);
cuerpo_ancho.parametros(perfil,30,1,0.4,-0.4,false);
};

void _cuerpo::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor)
{

glPushMatrix();
cuerpo_ancho.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
glPopMatrix();
}


//************************************************************************

_cabeza::_cabeza()
{
// perfil para las antenas
vector<_vertex3f> perfil;
_vertex3f aux;
aux.x=0.02;aux.y=-0.4;aux.z=0.0;
perfil.push_back(aux);
aux.x=0.02;aux.y=0.4;aux.z=0.0;
perfil.push_back(aux);
antenas.parametros(perfil,12,1,1.5,1.5,false);
};

void _cabeza::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor)
{

glPushMatrix();
cabeza.setRadio(0.8);
cabeza.create();
cabeza.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
glPopMatrix();

glPushMatrix();
glTranslatef(0.4,0.6,0.0);
glRotatef(-30,0.0,0.0,1.0);
antenas.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
glPopMatrix();

glPushMatrix();
glTranslatef(-0.4,0.6,0.0);
glRotatef(30,0.0,0.0,1.0);
antenas.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
glPopMatrix();

glPushMatrix();
glTranslatef(-0.3,0.45,0.6);
glRotatef(65,1,0,0);
ojo.setRadio(0.05);
ojo.create();
ojo.draw(modo, 0, 0, 0, 0, 0, 0, grosor);
glPopMatrix();

glPushMatrix();
glTranslatef(0.3,0.45,0.6);
glRotatef(65,1,0,0);
ojo.setRadio(0.05);
ojo.create();
ojo.draw(modo, 0, 0, 0, 0, 0, 0, grosor);
glPopMatrix();

}


//************************************************************************

_mango::_mango()
{
// perfil para las antenas
vector<_vertex3f> perfil;
_vertex3f aux;
aux.x=0.2;aux.y=-0.4;aux.z=0.0;
perfil.push_back(aux);
aux.x=0.2;aux.y=0.4;aux.z=0.0;
perfil.push_back(aux);
cilindro.parametros(perfil,12,1,0.4,-0.4,false);
};

void _mango::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor)
{

glPushMatrix();
glScalef(-1.0,2.5,-1.0);
cilindro.draw(modo, 0, 0, 0, 0, 0, 0, grosor);
glPopMatrix();

glPushMatrix();
glScalef(1.3,0.6,0.6);
glTranslatef(0.0,0.8,0.0);
glRotatef(90,0,0,1);
cilindro.draw(modo, 0, 0, 0, 0, 0, 0, grosor);
glPopMatrix();

glPushMatrix();
glScalef(0.3,0.1,0.1);
glTranslatef(-1.74,4.8,0.0);
glRotatef(90,0,0,1);
conito.draw(modo, 255, 0, 0, 255, 0, 0, grosor);
glPopMatrix();

glPushMatrix();
glScalef(0.3,0.1,0.1);
glTranslatef(1.74,4.8,0.0);
glRotatef(-90,0,0,1);
conito.draw(modo, 255, 0, 0, 255, 0, 0, grosor);
glPopMatrix();

//x --> 0.12	y --> 2		z --> 0.6

}


//************************************************************************

_laser::_laser()
{
// perfil para las antenas
vector<_vertex3f> perfil;
_vertex3f aux;
aux.x=0.12;aux.y=-2;aux.z=0.0;
perfil.push_back(aux);
aux.x=0.12;aux.y=2;aux.z=0.0;
perfil.push_back(aux);
cilindro.parametros(perfil,12,1,2,-2,false);
};

void _laser::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor)
{

glPushMatrix();
cilindro.draw(modo, 255, 0, 0, 255, 0, 0, grosor);
glPopMatrix();

}

//************************************************************************

_android::_android()
{
giro_brazos=2.0;
giro_patas=2.0;
giro_cabeza=2.0;
salir_laser=0.6;
giro_patas_max=20;
giro_patas_min=-9;
giro_brazos_min=-30;
giro_brazos_max=20;
salir_laser_max=4.6;
salir_laser_min = 0.35;
};

void _android::draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor)
{
	
glPushMatrix();
glScalef(2.0,1.6,2.0);
cuerpo.draw(modo,r1,g1,b1,r2,g2,b2,grosor);
glPopMatrix();

glPushMatrix();
glScalef(1.0,0.8,1.0);
glRotatef(giro_brazos,1,0,0);
glTranslatef(1.05,0.15,0.0);
brazo_dcha.draw(modo,r1,g1,b1,r2,g2,b2,grosor);
glPopMatrix();

//Mango
glPushMatrix();
glScalef(0.5,0.5,0.5);
glRotatef(90,0,1,0);
glRotatef(60,0,0,1);
glRotatef(giro_brazos,0,0,1);
glTranslatef(-1.3,0.3,2.1);
mango.draw(modo,r1,g1,b1,r2,g2,b2,grosor);
glPopMatrix();

//Laser
glPushMatrix();
glScalef(0.5,0.5,0.5);
glRotatef(90,0,1,0);
glRotatef(60,0,0,1);
glRotatef(giro_brazos,0,0,1);
glTranslatef(-1.3,-0.02,2.1);
glScalef(1.0,0.5,1.0);
glTranslatef(0,salir_laser,0);
laser.draw(modo,r1,g1,b1,r2,g2,b2,grosor);
glPopMatrix();

glPushMatrix();
glScalef(1.0,0.8,1.0);
glRotatef(giro_brazos,1,0,0);
glTranslatef(-1.05,0.15,0.0);
brazo_izda.draw(modo,r1,g1,b1,r2,g2,b2,grosor);
glPopMatrix();

glPushMatrix();
glTranslatef(0.3,0.0,0.0);
glRotatef(giro_patas,1,0,0);
pata_dcha.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
glPopMatrix();

glPushMatrix();
glTranslatef(-0.3,0.0,0.0);
glRotatef(giro_patas,1,0,0);
pata_izda.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
glPopMatrix();

glPushMatrix();
glTranslatef(0.0,0.7,0.0);
glRotatef(giro_cabeza,0,1,0);
cabeza.draw(modo, r1, g1, b1, r2, g2, b2, grosor);
glPopMatrix();

};
