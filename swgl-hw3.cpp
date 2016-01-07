#include "swgl.h"
#include <assert.h>
#include <math.h>


//---------------------------------------------------------------------------
//cghw1
//---------------------------------------------------------------------------
GLdouble CTM_MV[16];	//Current Transformation Matrix: ModelView
GLdouble CTM_P[16];		//Current Transformation Matrix: Projection
GLdouble *CTM;			//Pointer to Current Transformation Matrix


bool swTransformation(const GLdouble h[4], GLdouble w[4])
{
	//p = CTM_P*CTM_MV*h


	//prespective division


	//viewport transformation



	return true;
}


//---------------------------------------------------------------------------
//cghw2
//---------------------------------------------------------------------------

//GLdouble *ZBUFFER = NULL;
//int Z_W=-1, Z_H=-1;

void writepixel(int x, int y, GLdouble r, GLdouble g, GLdouble b)
{
	GLubyte map[1]={255};

	glColor3d(r, g, b);
	glWindowPos2i(x, y);
	glBitmap(1, 1, 0, 0, 0, 0, map);
}

bool BresenhamLine(int x1, int y1, int x2, int y2, GLdouble r, GLdouble g, GLdouble b)
{


	return true;
}

bool BresenhamLine(GLdouble x1, GLdouble y1, GLdouble z1, GLdouble x2, GLdouble y2, GLdouble z2, GLdouble r, GLdouble g, GLdouble b)
{



	return true;
}

bool swTriangle(GLdouble x1, GLdouble y1, GLdouble z1,
			 GLdouble x2, GLdouble y2, GLdouble z2,
			 GLdouble x3, GLdouble y3, GLdouble z3,
			 GLdouble r, GLdouble g, GLdouble b)
{


	return true;
}


bool swInitZbuffer(int width, int height)
{

	return true;
}



bool swClearZbuffer()
{

	return true;
}

//---------------------------------------------------------------------------
//cghw3
//---------------------------------------------------------------------------

GLdouble  _ambientLight[] = { 0.3, 0.3, 0.3, 1.0 };
GLdouble  _diffuseLight[] = { 0.7, 0.7, 0.7, 1.0 };
GLdouble  _specularLight[] = { 0.0, 1.0, 0.0, 1.0 };

GLdouble _lightPos[] = { -50.0, 50.0, 100.0, 1.0 };

GLdouble  _ambientMaterial[] = { 0.3, 0.3, 0.3, 1.0 };
GLdouble  _diffuseMaterial[] = { 0.7, 0.7, 0.7, 1.0 };
GLdouble  _specularMaterial[] = { 1.0, 1.0, 1.0, 1.0 };
GLdouble  _shininessMaterial = 128.0;


void writepixelfast(int x, int y, GLdouble r, GLdouble g, GLdouble b)
{
	glBegin(GL_POINTS);
		glColor3d(r, g, b);
		glVertex2i(x, y);
	glEnd();
}



bool swNormalTransformation(const GLdouble h[4], GLdouble w[4])
{
	//apply transformation


	//Normalize

	return true;
}


void swLightfv(GLenum light, GLenum pname, const GLfloat *params)
{
	switch(pname) {
		case GL_AMBIENT:
			_ambientLight[0]=params[0];
			_ambientLight[1]=params[1];
			_ambientLight[2]=params[2];
			_ambientLight[3]=params[3];
			break;

		case GL_DIFFUSE:
			_diffuseLight[0]=params[0];
			_diffuseLight[1]=params[1];
			_diffuseLight[2]=params[2];
			_diffuseLight[3]=params[3];
			break;

		case GL_SPECULAR:
			_specularLight[0]=params[0];
			_specularLight[1]=params[1];
			_specularLight[2]=params[2];
			_specularLight[3]=params[3];
			break;

		case GL_POSITION:
			_lightPos[0]=params[0];
			_lightPos[1]=params[1];
			_lightPos[2]=params[2];
			_lightPos[3]=params[3];

			//------------------------------------------------------
			//ADD transformation to eye coordinate
			//modeviewmatrix * _lightPos


			break;
	}
}

void swMaterialfv(GLenum face, GLenum pname, const GLfloat *params)
{
	switch(pname) {
		case GL_AMBIENT:
			_ambientMaterial[0]=params[0];
			_ambientMaterial[1]=params[1];
			_ambientMaterial[2]=params[2];
			_ambientMaterial[3]=params[3];
			break;
		case GL_DIFFUSE:
			_diffuseMaterial[0]=params[0];
			_diffuseMaterial[1]=params[1];
			_diffuseMaterial[2]=params[2];
			_diffuseMaterial[3]=params[3];
			break;
		case GL_SPECULAR:
			_specularMaterial[0]=params[0];
			_specularMaterial[1]=params[1];
			_specularMaterial[2]=params[2];
			_specularMaterial[3]=params[3];
			break;
	}
}

void swMateriali(GLenum face, GLenum pname, GLint param)
{
	switch(pname) {
		case GL_SHININESS:
			_shininessMaterial=param;
			break;
	}
}


//color = ambient + diffuse + specular
//position of P in view-space (px, py, pz)
//normal of P in view-space (nx, ny, nz)
//light source position in view-space (lx, ly, lz)
bool PhongShading(GLdouble color[3],
				  GLdouble px, GLdouble py, GLdouble pz,
				  GLdouble nx, GLdouble ny, GLdouble nz,
				  GLdouble lx, GLdouble ly, GLdouble lz,
				  GLdouble r, GLdouble g, GLdouble b)
{
	color[0] = color[1] = color[2] = 0.0;

	//caculate vectors



	//color = ambient + diffuse + specular


	return true;
}


//Gouraud shading
bool swTriangleG(GLdouble x1, GLdouble y1, GLdouble z1,
			 GLdouble x2, GLdouble y2, GLdouble z2,
			 GLdouble x3, GLdouble y3, GLdouble z3,
			 GLdouble nx1, GLdouble ny1, GLdouble nz1,
			 GLdouble nx2, GLdouble ny2, GLdouble nz2,
			 GLdouble nx3, GLdouble ny3, GLdouble nz3,
			 GLdouble r1, GLdouble g1, GLdouble b1,
			 GLdouble r2, GLdouble g2, GLdouble b2,
			 GLdouble r3, GLdouble g3, GLdouble b3)
{
	//transformation all data(vertex, normal, light vector, COP) to eye coordiante


	//modified Pong shading equation in each vertex
	//color_vertex[i] = ambient + diffuse + specular
	//PhongShading(color_vertex[i], ...);



	//Raterization:

	return true;
}


////Phong Shading
bool swTriangleP(GLdouble x1, GLdouble y1, GLdouble z1,
			 GLdouble x2, GLdouble y2, GLdouble z2,
			 GLdouble x3, GLdouble y3, GLdouble z3,
			 GLdouble nx1, GLdouble ny1, GLdouble nz1,
			 GLdouble nx2, GLdouble ny2, GLdouble nz2,
			 GLdouble nx3, GLdouble ny3, GLdouble nz3,
			 GLdouble r1, GLdouble g1, GLdouble b1,
			 GLdouble r2, GLdouble g2, GLdouble b2,
			 GLdouble r3, GLdouble g3, GLdouble b3)
{
	//transformation all data(vertex, normal, light vector, COP) to eye coordiante


	//modified Pong shading equation in each vertex
	//PhongShading(color_vertex[i], ...);


	//Raterization:

	return true;
}




