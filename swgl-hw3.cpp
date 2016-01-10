#include "swgl.h"
#include <assert.h>
#include <math.h>


//---------------------------------------------------------------------------
//cghw1
//---------------------------------------------------------------------------
GLdouble CTM_MV[16];	//Current Transformation Matrix: ModelView
GLdouble CTM_P[16];		//Current Transformation Matrix: Projection
GLdouble *CTM;			//Pointer to Current Transformation Matrix

GLdouble MultTemp[16];
GLdouble VectorTemp[4];

GLint ViewportX , ViewportY ;
GLsizei ViewportWidth, ViewportHeight;

void MatrixMul(const double A[16], const double B[16])
{
	for(int i = 0;i <= 12; i=i+4)
    {
        for(int j = 0;j < 4; j++)
            MultTemp[i + j] = B[i]*A[j] + B[i+1]*A[j+4] + B[i+2]*A[j+8] + B[i+3]*A[j+12];
    }
}
void MatrixMulVector(const double A[16], const double v1[4])
{
	for(int i = 0;i <= 3; i=i+1)
    {
        VectorTemp[i] = A[i]*v1[0] + A[i+4]*v1[1] + A[i+8]*v1[2] + A[i+12]*v1[3];
    }
}

bool swTransformation(const GLdouble h[4], GLdouble w[4])
{
	//p = CTM_P*CTM_MV*h
    MatrixMul(CTM_P,CTM_MV);
	MatrixMulVector( MultTemp , h );

	//prespective division
    for(int i = 0 ; i<4 ; i++)
		VectorTemp[i] = VectorTemp[i] / VectorTemp[3];

	//viewport transformation
    w[0] =  (VectorTemp[0]+1) * (ViewportWidth / 2) + ViewportX;
	w[1] =  (VectorTemp[1]+1) * (ViewportHeight / 2) + ViewportY;
    w[2] =  VectorTemp[2];
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
    bool steep = abs(y2 - y1) > abs(x2 - x1);
    if(steep)
    {
		swap(x1 , y1);
		swap(x2 , y2);
	}
	if(x1 > x2)
    {
		swap(x1 , x2);
		swap(y1 , y2);
		swap(z1 , z2);
    }
	int deltaX = x2 - x1;
	int deltaY = abs(y2 - y1);
	int error = deltaX / 2;
	int ystep;
	int y = y1;

	if(y1 < y2) ystep = 1;
	else ystep = -1;

//-------------Zbuffer----------------------------
        GLdouble curZ = z1;
        GLdouble deltaZ = ZBuffer(x1, x2, z1, z2);
//-------------Zbuffer----------------------------

	for(int i = x1; i <= x2; i++)
	{
		if(steep)
        {
			writepixel(y , i, curZ, r, g, b);
		}
		else
		{
			writepixel(i , y, curZ, r, g, b);
		}

		error = error - deltaY;

		if(error < 0)
		{
			y = y + ystep;
			error = error + deltaX;
		}
		curZ += deltaZ;
		//printf("%f",curZ);
	}
	return true;
}

bool swTriangle(GLdouble x1, GLdouble y1, GLdouble z1,
			 GLdouble x2, GLdouble y2, GLdouble z2,
			 GLdouble x3, GLdouble y3, GLdouble z3,
			 GLdouble r, GLdouble g, GLdouble b)
{
    GLdouble list[2000][3];
	int listCount = 0;

	if (x1 >= x2)
    {
		swap(x1 , x2);
		swap(y1 , y2);
		swap(z1 , z2);
	}
	if (x2 >= x3)
	{
		swap(x2 , x3);
		swap(y2 , y3);
		swap(z2 , z3);
	}
	if (x1 >= x2)
	{
		swap(x1 , x2);
		swap(y1 , y2);
		swap(z1 , z2);
	}

	GLdouble point[3][3];
	point[0][0] = x1;
	point[0][1] = y1;
	point[0][2] = z1;

	point[1][0] = x2;
	point[1][1] = y2;
	point[1][2] = z2;

	point[2][0] = x3;
	point[2][1] = y3;
	point[2][2] = z3;

	for (int k = 0 ; k <= 2 ; k++)
    {
		GLdouble x1,y1,z1,x2,y2,z2;
		x1 = point[(k+3)%3][0];
		y1 = point[(k+3)%3][1];
		z1 = point[(k+3)%3][2];
		x2 = point[(k+4)%3][0];
		y2 = point[(k+4)%3][1];
		z2 = point[(k+4)%3][2];

		bool steep=abs(y2 - y1) > abs(x2 - x1);
		if(steep)
		{
			swap(x1 , y1);
			swap(x2 , y2);
		}
		if(x1 > x2)
		{
			swap(x1 , x2);
			swap(y1 , y2);
			swap(z1 , z2);
		}

        int deltaX = x2 - x1;
        int deltaY = abs(y2 - y1);
        int error = deltaX / 2;
        int ystep;
        int y = y1;

        if(y1 < y2) ystep = 1;
        else ystep = -1;

//-------------Zbuffer----------------------------
        GLdouble curZ = z1;
        GLdouble deltaZ = ZBuffer(x1, x2, z1, z2);
//-------------Zbuffer----------------------------

		for(int i = x1; i <= x2; i++)
		{
			if(steep){
				list[listCount][0]=y;
				list[listCount][1]=i;
				list[listCount][2]=curZ;
			}
			else{
				list[listCount][0]=i;
				list[listCount][1]=y;
				list[listCount][2]=curZ;
			}
			error = error - deltaY;
			if(error < 0)
			{
				y = y + ystep;
				error = error + deltaX;
			}
			listCount++;
			curZ += deltaZ;
		}
	}

	for(int i = 0 ,j = 0 ;i < listCount && j < listCount; i++)
    {
        if(list[i][0]==list[j][0])
        {
            BresenhamLine(list[i][0],list[i][1],list[i][2],list[j][0],list[j][1],list[j][2],r,g,b);
            i = 0;
            j++;
        }
    }
	return true;
}


bool swInitZbuffer(int width, int height)
{
    for (int i=0;i < 1920;i++)
		for(int j=0;j < 1080;j++)
			Zbuffer[i][j]=2;
	return true;
}



bool swClearZbuffer()
{
    for (int i=0;i < 1920;i++)
		for(int j=0;j < 1080;j++)
			Zbuffer[i][j]=2;
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




