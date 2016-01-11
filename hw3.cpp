//NCCU CS
//Xshading

#include <math.h>
#include <stdlib.h>

#include "GLee.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "swgl.h"
#include "math3d.h"
#include "glm.h"

#include <iostream>

#ifndef bool
#define bool int
#define false 0
#define true 1
#endif

#ifndef M_PI
#define M_PI 3.14159
#endif

int     cnt=0;

int 	winWidth, winHeight;

float 	angle = 0.0, axis[3], trans[3];
bool 	trackingMouse = false;
bool 	redrawContinue = false;
bool    trackballMove = false;
GLdouble TRACKM[16]={1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};

GLdouble DEBUG_M[16];

GLdouble Angle1=0, Angle2=0;
GLint TICK=0;

GLMmodel *MODEL;

GLfloat lightPos[] = { 0.f, 0.0f, 0.0, 1.0f };

GLdouble MODELSCALE = 1.5;
GLdouble LIGHTP = 1.5;


int DRAWTYPE = 3;// 0:hw1, 1:hw2, 2:Gouraud shading, 3: Phong Shading


//GLfloat adjust[4][2570];//2557 num of vertices.
/*----------------------------------------------------------------------*/



GLfloat vertices[][3] = {
    {-1.0,-1.0,-1.0},{1.0,-1.0,-1.0}, {1.0,1.0,-1.0}, {-1.0,1.0,-1.0},
    {-1.0,-1.0,1.0}, {1.0,-1.0,1.0}, {1.0,1.0,1.0}, {-1.0,1.0,1.0}
};

GLfloat colors[][3] = {
    {0.0,0.0,0.0},{1.0,0.0,0.0}, {1.0,1.0,0.0}, {0.0,1.0,0.0},
    {0.0,0.0,1.0}, {1.0,0.0,1.0}, {1.0,1.0,1.0}, {0.0,1.0,1.0}
};

//一堆垃圾
void useless(){
/*
float glmMax(GLfloat a, GLfloat b)
{
    if (b > a)
        return b;
    return a;
}

float glmDot(GLfloat* u, GLfloat* v)
{
    //assert(u); assert(v);

    return u[0]*v[0] + u[1]*v[1] + u[2]*v[2];
}

void findAdjustway(GLMmodel* model)
{

	GLfloat *n1, *n2, *n3;
	GLfloat *v1, *v2, *v3;


    //12 21 23 32 31 13
    GLfloat n1minusn2[3];    //GLfloat v1minusv2[3];

    GLfloat n2minusn1[3];    //GLfloat v2minusv1[3];

    GLfloat n2minusn3[3];    //GLfloat v2minusv3[3];

    GLfloat n3minusn2[3];    //GLfloat v3minusv2[3];

    GLfloat n3minusn1[3];    //GLfloat v3minusv1[3];

    GLfloat n1minusn3[3];    //GLfloat v1minusv3[3];

    GLfloat viminusvii[3][6];
    GLfloat nndotvv[6];

	//get current color
	GLdouble col[4];
	glGetDoublev(GL_CURRENT_COLOR, col);

    for (int i = 0; i < model->numtriangles; i++) {

        GLMtriangle* triangle = &(model->triangles[i]);
        //GLfloat* vertices = &(model->vertices[i]);

        n1 = &model->normals[3 * triangle->nindices[0]];
        v1 = &model->vertices[3 * triangle->vindices[0]];
        n2 = &model->normals[3 * triangle->nindices[1]];
        v2 = &model->vertices[3 * triangle->vindices[1]];
		n3 = &model->normals[3 * triangle->nindices[2]];
        v3 = &model->vertices[3 * triangle->vindices[2]];

        //(ni-nj)dot()
        for(int j=0;j<3;j++){
            n1minusn2[j]=(n1[j]-n2[j]);
            viminusvii[j][0]=(v1[j]-v2[j]);
            //v1minusv2[j]=(v1[j]-v2[j]);

            n2minusn1[j]=(n2[j]-n1[j]);
            viminusvii[j][2]=(v2[j]-v1[j]);
            //v2minusv1[j]=(v2[j]-v1[j]);

            n2minusn3[j]=(n2[j]-n3[j]);
            viminusvii[j][3]=(v2[j]-v3[j]);
            //v2minusv3[j]=(v2[j]-v3[j]);

            n3minusn2[j]=(n3[j]-n2[j]);
            viminusvii[j][5]=(v3[j]-v2[j]);
            //v3minusv2[j]=(v3[j]-v2[j]);

            n3minusn1[j]=(n3[j]-n1[j]);
            viminusvii[j][4]=(v3[j]-v1[j]);
            //v3minusv1[j]=(v3[j]-v1[j]);

            n1minusn3[j]=(n1[j]-n3[j]);
            viminusvii[j][1]=(v1[j]-v3[j]);
            //v1minusv3[j]=(v1[j]-v3[j]);

        }
        nndotvv[0] = glmDot(n1minusn2,viminusvii[0]);//12
        nndotvv[2] = glmDot(n2minusn1,viminusvii[2]);//21
        nndotvv[3] = glmDot(n2minusn3,viminusvii[3]);//23
        nndotvv[5] = glmDot(n3minusn2,viminusvii[5]);//32
        nndotvv[4] = glmDot(n3minusn1,viminusvii[4]);//31
        nndotvv[1] = glmDot(n1minusn3,viminusvii[1]);//13

        //decide witch derection is better
        for(int k=0;k<3;k++){
                if(adjust[0][ triangle->nindices[k] ]==0){
                    if( nndotvv[2*k] > nndotvv[2*k+1] ){
                        adjust[0][ triangle->nindices[k] ] = 0.1*viminusvii[0][2*k];
                        adjust[1][ triangle->nindices[k] ] = 0.1*viminusvii[1][2*k];
                        adjust[2][ triangle->nindices[k] ] = 0.1*viminusvii[2][2*k];
                        adjust[3][ triangle->nindices[k] ] = nndotvv[2*k];
                    }
                    else{
                        adjust[0][ triangle->nindices[k] ] = 0.1*viminusvii[0][2*k+1];
                        adjust[1][ triangle->nindices[k] ] = 0.1*viminusvii[1][2*k+1];
                        adjust[2][ triangle->nindices[k] ] = 0.1*viminusvii[2][2*k+1];
                        adjust[3][ triangle->nindices[k] ] = nndotvv[2*k+1];
                    }
                }
                else if( glmMax ( nndotvv[2*k], nndotvv[2*k+1] ) > adjust[3][ triangle->nindices[k] ]){
                    if( nndotvv[2*k] > nndotvv[2*k+1] ){
                        adjust[0][ triangle->nindices[k] ] = 0.1*viminusvii[0][2*k];
                        adjust[1][ triangle->nindices[k] ] = 0.1*viminusvii[1][2*k];
                        adjust[2][ triangle->nindices[k] ] = 0.1*viminusvii[2][2*k];
                        adjust[3][ triangle->nindices[k] ] = nndotvv[2*k];
                    }
                    else{
                        adjust[0][ triangle->nindices[k] ] = 0.1*viminusvii[0][2*k+1];
                        adjust[1][ triangle->nindices[k] ] = 0.1*viminusvii[1][2*k+1];
                        adjust[2][ triangle->nindices[k] ] = 0.1*viminusvii[2][2*k+1];
                        adjust[3][ triangle->nindices[k] ] = nndotvv[2*k+1];
                    }
                }
        }

    }


}
*/
}


inline void SwglTri(GLdouble x1, GLdouble y1, GLdouble z1,
			 GLdouble x2, GLdouble y2, GLdouble z2,
			 GLdouble x3, GLdouble y3, GLdouble z3,
			 GLdouble nx1=1, GLdouble ny1=0, GLdouble nz1=0,
			 GLdouble nx2=1, GLdouble ny2=0, GLdouble nz2=0,
			 GLdouble nx3=1, GLdouble ny3=0, GLdouble nz3=0,
			 GLdouble r1=1, GLdouble g1=1, GLdouble b1=1,
			 GLdouble r2=1, GLdouble g2=1, GLdouble b2=1,
			 GLdouble r3=1, GLdouble g3=1, GLdouble b3=1)
{
	//copy to homogenous coordinate
	GLdouble h1[4]={x1, y1, z1, 1.0};
	GLdouble h2[4]={x2, y2, z2, 1.0};
	GLdouble h3[4]={x3, y3, z3, 1.0};

	//window coordinate
	GLdouble w1[4]={x1, y1, 0, 1.0};
	GLdouble w2[4]={x2, y2, 0, 1.0};
	GLdouble w3[4]={x3, y3, 0, 1.0};

	//implement the opengl pipeline here
	swTransformation(h1, w1);
	swTransformation(h2, w2);
	swTransformation(h3, w3);

	switch(DRAWTYPE) {
		case 0:
		{
			//copy to homogenous coordinate
			GLdouble h1[4]={x1, y1, z1, 1.0};
			GLdouble h2[4]={x2, y2, z2, 1.0};
			GLdouble h3[4]={x3, y3, z3, 1.0};

			//window coordinate
			GLdouble w1[4]={x1, y1, 0, 1.0};
			GLdouble w2[4]={x2, y2, 0, 1.0};
			GLdouble w3[4]={x3, y3, 0, 1.0};

			//implement the opengl pipeline here
			swTransformation(h1, w1);
			swTransformation(h2, w2);
			swTransformation(h3, w3);

			writepixel(w1[0], w1[1], 0, r1, g1, b1);//writepixel(w1[0], w1[1], r1, g1, b1)
			writepixel(w2[0], w2[1], 0, r2, g2, b2);
			writepixel(w3[0], w3[1], 0, r3, g3, b3);
		}
		break;

		case 1:
		{
			//copy to homogenous coordinate
			GLdouble h1[4]={x1, y1, z1, 1.0};
			GLdouble h2[4]={x2, y2, z2, 1.0};
			GLdouble h3[4]={x3, y3, z3, 1.0};

			//window coordinate
			GLdouble w1[4]={x1, y1, 0, 1.0};
			GLdouble w2[4]={x2, y2, 0, 1.0};
			GLdouble w3[4]={x3, y3, 0, 1.0};

			//implement the opengl pipeline here
			swTransformation(h1, w1);
			swTransformation(h2, w2);
			swTransformation(h3, w3);

			swTriangle(w1[0], w1[1], w1[2],
					   w2[0], w2[1], w2[2],
					   w3[0], w3[1], w3[2],
					   r1, g1, b1);
		}
		break;

		case 2:
		{
			swTriangleG(x1, y1, z1,
						x2, y2, z2,
						x3, y3, z3,
						nx1, ny1, nz1,
						nx2, ny2, nz2,
						nx3, ny3, nz3,
						r1, g1, b1,
						r2, g2, b2,
						r3, g3, b3);
		}
		break;

		case 3:
		{
			swTriangleP(x1, y1, z1,
						x2, y2, z2,
						x3, y3, z3,
						nx1, ny1, nz1,
						nx2, ny2, nz2,
						nx3, ny3, nz3,
						r1, g1, b1,
						r2, g2, b2,
						r3, g3, b3);
		}
		break;

	}

}


void SwglTri(int index1, int index2, int index3)
{
	SwglTri( vertices[index1][0], vertices[index1][1], vertices[index1][2],
		     vertices[index2][0], vertices[index2][1], vertices[index2][2],
			 vertices[index3][0], vertices[index3][1], vertices[index3][2]);
}


void SwglLine(GLdouble x1, GLdouble y1, GLdouble z1, GLdouble x2, GLdouble y2, GLdouble z2)
{
	//copy to homogenous coordinate
	GLdouble h1[4]={x1, y1, z1, 1.0};
	GLdouble h2[4]={x2, y2, z2, 1.0};

	GLdouble w1[4]={x1, y1, 0, 1.0}; //window coordinate
	GLdouble w2[4]={x2, y2, 0, 1.0};

	//implement the opengl pipeline here
	swTransformation(h1, w1);
	swTransformation(h2, w2);

	////draw the 2D line
	//glBegin(GL_LINES);
	//	//glColor3fv(colors[index1]);
	//	glVertex2f(w1[0], w1[1]);
	//	//glColor3fv(colors[index2]);
	//	glVertex2f(w2[0], w2[1]);
	//glEnd();

	//implement
	switch(DRAWTYPE) {
		case 0:
		{
			writepixel(w1[0], w1[1], 0, 1, 0, 0); //writepixel(w1[0], w1[1], 1, 0, 0);
			writepixel(w2[0], w2[1], 0, 0, 1, 0); //writepixel(w1[0], w1[1], 1, 0, 0);
		}
		break;

		case 1: case 2: case 3:
		{
			GLdouble col[4];
			glGetDoublev(GL_CURRENT_COLOR, col);
			BresenhamLine(w1[0], w1[1], w1[2], w2[0], w2[1], w2[2], col[0], col[1], col[2]);
		}
		break;
	}
}

void SwglLine(int index1, int index2)
{
	SwglLine(vertices[index1][0], vertices[index1][1], vertices[index1][2],
		     vertices[index2][0], vertices[index2][1], vertices[index2][2]);
}



void SolidQuad(int a, int b, int c, int d, bool USINGOPENGL)
{
	if(USINGOPENGL) {
		glBegin(GL_TRIANGLES);
			glVertex3fv(vertices[a]);
			glVertex3fv(vertices[b]);
			glVertex3fv(vertices[c]);

			glVertex3fv(vertices[c]);
			glVertex3fv(vertices[d]);
			glVertex3fv(vertices[a]);
		glEnd();
	} else {
		SwglTri(a, b, c);
		SwglTri(c, d, a);
	}
}


void swSolidCube(void)
{
    // map vertices to faces
    SolidQuad(1,0,3,2, false);
    SolidQuad(3,7,6,2, false);
    SolidQuad(7,3,0,4, false);
    SolidQuad(2,6,5,1, false);
    SolidQuad(4,5,6,7, false);
    SolidQuad(5,4,0,1, false);
}

void glSolidCube(void)
{
    // map vertices to faces */
    SolidQuad(1,0,3,2, true);
    SolidQuad(3,7,6,2, true);
    SolidQuad(7,3,0,4, true);
    SolidQuad(2,6,5,1, true);
    SolidQuad(4,5,6,7, true);
    SolidQuad(5,4,0,1, true);
}



void OpenglLine(GLdouble x1, GLdouble y1, GLdouble z1, GLdouble x2, GLdouble y2, GLdouble z2)
{
	glBegin(GL_LINES);
		glVertex3f(x1, y1, z1);
		glVertex3f(x2, y2, z2);
	glEnd();
}

void OpenglLine(int index1, int index2)
{
	OpenglLine(vertices[index1][0], vertices[index1][1], vertices[index1][2],
		       vertices[index2][0], vertices[index2][1], vertices[index2][2]);
}

void WireQuad(int a, int b, int c , int d, bool USINGOPENGL)
{
	if(USINGOPENGL) {
		OpenglLine(a, b);
		OpenglLine(b, c);
		OpenglLine(c, d);
		OpenglLine(d, a);
	} else {
		SwglLine(a, b);
		SwglLine(b, c);
		SwglLine(c, d);
		SwglLine(d, a);
	}
}


void swWireCube(void)
{
    // map vertices to faces
    WireQuad(1,0,3,2, false);
    WireQuad(3,7,6,2, false);
    WireQuad(7,3,0,4, false);
    WireQuad(2,6,5,1, false);
    WireQuad(4,5,6,7, false);
    WireQuad(5,4,0,1, false);
}

void glWireCube(void)
{
    // map vertices to faces */
    WireQuad(1,0,3,2, true);
    WireQuad(3,7,6,2, true);
    WireQuad(7,3,0,4, true);
    WireQuad(2,6,5,1, true);
    WireQuad(4,5,6,7, true);
    WireQuad(5,4,0,1, true);
}

void polygon(int a, int b, int c , int d, int face)
{
    /* draw a polygon via list of vertices */
    glBegin(GL_POLYGON);
  	glColor3fv(colors[a]);
  	glVertex3fv(vertices[a]);
  	glColor3fv(colors[b]);
  	glVertex3fv(vertices[b]);
  	glColor3fv(colors[c]);
  	glVertex3fv(vertices[c]);
  	glColor3fv(colors[d]);
  	glVertex3fv(vertices[d]);
    glEnd();
}

void colorcube(void)
{

    /* map vertices to faces */
    polygon(1,0,3,2,0);
    polygon(3,7,6,2,1);
    polygon(7,3,0,4,2);
    polygon(2,6,5,1,3);
    polygon(4,5,6,7,4);
    polygon(5,4,0,1,5);
}


GLvoid swglmDraw(GLMmodel* model)
{
	GLfloat *n1, *n2, *n3;
	GLfloat *v1, *v2, *v3;

	//get current color
	GLdouble col[4];
	glGetDoublev(GL_CURRENT_COLOR, col);

    for (unsigned int i = 0; i < model->numtriangles; i++) {

        GLMtriangle* triangle = &(model->triangles[i]);
        //GLfloat* vertices = &(model->vertices[i]);

        n1 = &model->normals[3 * triangle->nindices[0]];
        v1 = &model->vertices[3 * triangle->vindices[0]];
        n2 = &model->normals[3 * triangle->nindices[1]];
        v2 = &model->vertices[3 * triangle->vindices[1]];
		n3 = &model->normals[3 * triangle->nindices[2]];
        v3 = &model->vertices[3 * triangle->vindices[2]];


/*
        if(i<20){

        std::cout<< model->numtriangles <<'\n';
        std::cout<< model->numvertices <<'\n';
        std::cout<< triangle->vindices[0] <<" "<< triangle->vindices[1] <<" "<< triangle->vindices[2] <<'\n';
        std::cout<< v1[0] <<" "<< v1[1] <<" "<< v1[2] <<'\n';
        //std::cout<< model->vertices[3 * triangle->vindices[0]+ 0] <<" "<< model->vertices[3 * triangle->vindices[0] + 1] <<" "<< model->vertices[3 * triangle->vindices[0] + 2] <<'\n';

        std::cout<< n1[0] <<" "<< n1[1] <<" "<< n1[2] <<'\n';

        std::cout<< col[0] <<" "<< col[1] <<" "<< col[2] <<'\n';
        if(i==19){
                std::cout<<"oooooooooooooooooooo"<<'\n';
            }
        }
*/

        for(int i=0;i<3;i++){
            n1[i] = n1[i] + 0.1;
            n2[i] = n2[i] + 0.1;
            n3[i] = n3[i] + 0.1;
        }

        glBegin(GL_TRIANGLES);
        for (int i = 0; i < model->numtriangles; i++) {


            glNormal3fv(n1);
            glVertex3fv(v1);

            glNormal3fv(n2);
            glVertex3fv(v2);

            glNormal3fv(n3);
            glVertex3fv(v3);

        }
        glEnd();


    }


}

void softPath(void)
{

    glViewport(winWidth/2, 0, winWidth/2, winHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();



	gluPerspective(60, (GLfloat)(winWidth*0.5)/winHeight, 0.1, 25); //(60, (GLfloat)(winWidth*0.5)/winHeight, 0.1, 25)
	glGetDoublev(GL_PROJECTION_MATRIX, DEBUG_M);


    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(1, 1, 4, 0, 0, 0, 0, 1, 0);//1, 1, 4, 0, 1, 0, 0, 1, 0
	glGetDoublev(GL_MODELVIEW_MATRIX, DEBUG_M);



    glPushMatrix();
		glRotated(double(TICK), 0, 1, 0);
		glRotated(double(TICK)*0.1, 0, 0, 1);
		glTranslatef(LIGHTP, LIGHTP, 0);
		glutSolidSphere(0.05, 32, 32);
        glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glPopMatrix();

	glPushMatrix();
		//multiple trackball matrix
		glMultMatrixd(TRACKM);

		glScaled(MODELSCALE, MODELSCALE, MODELSCALE);
		glColor3f(1.0, 1.0, 1.0); //(0.7, 1.0, 1.0)
        swglmDraw(MODEL);//自造
		//glmDraw(MODEL, GLM_SMOOTH);//GLM_FLAT

		//glutSolidSphere(1, 20, 20);
	glPopMatrix();


	/*---------------------------------------*/
{/*
    //Do not change, setting a basic transformation
	glViewport(0, 0, winWidth, winHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	//glOrtho(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0);
	glOrtho(0, winWidth, 0, winHeight, -2.0, 2.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//glColor3f(1, 0, 0);
	//OpenglLine(winWidth/2, 0, 0, winWidth, winHeight, 0);


	//
	//replace the opengl function in openglPath() to softgl
    //

	swClearZbuffer();



	//view transform
	swViewport(winWidth/2, 0, winWidth/2, winHeight);

    swMatrixMode(GL_PROJECTION);
    swLoadIdentity();

	//swOrtho(-2.0, 2.0, -2.0, 2.0, -3.0, 3.0);
	//swFrustum(-2.0, 2.0, -2.0, 2.0, -3.0, 3.0);
	swuPerspective(60, (GLfloat)(winWidth*0.5)/winHeight, 0.1, 25);

    swMatrixMode(GL_MODELVIEW);
	swLoadIdentity();
	swuLookAt(1, 1, 4, 0, 1, 0, 0, 1, 0);

    swPushMatrix();
		swRotated(double(TICK), 0, 1, 0);
		swRotated(double(TICK)*0.1, 0, 0, 1);
		swTranslated(LIGHTP, LIGHTP, 0);
		//swutSolidSphere(0.05, 32, 32);
        swLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	swPopMatrix();


	//world coordinate
	glColor3f(1, 0, 0);
	SwglLine(0, 0, 0, 3, 0, 0);
	glColor3f(0, 1, 0);
	SwglLine(0, 0, 0, 0, 3, 0);
	glColor3f(0, 0, 1);
	SwglLine(0, 0, 0, 0, 0, 3);

	swPushMatrix();
		//multiple trackball matrix
		swMultMatrixd(TRACKM);

		swScaled(MODELSCALE, MODELSCALE, MODELSCALE);
		glColor3f(0.7, 1.0, 1.0);

		swglmDraw(MODEL);

	swPopMatrix();
*/
/*
	swPushMatrix();
		swTranslated(0, 2, 0);
		swMultMatrixd(TRACKM);

		SwglTri(-1, 0, 0,
				1, 0, 0,
				0, 1, 0,
				0, 0, 1,
				0, 0, 1,
				0, 0, 1,
				1, 0, 0,
				0, 1, 0,
				0, 0, 1);
	swPopMatrix();
*/
}
}

void openglPath(void)
{
    //view transform
	glViewport(0, 0, winWidth/2, winHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	//glOrtho(-2.0, 2.0, -2.0, 2.0, -3.0, 25.0);
	//glFrustum(-2.0, 2.0, -2.0, 2.0, -3.0, 3.0);
	gluPerspective(60, (GLfloat)(winWidth*0.5)/winHeight, 0.1, 25); //(60, (GLfloat)(winWidth*0.5)/winHeight, 0.1, 25)
	glGetDoublev(GL_PROJECTION_MATRIX, DEBUG_M);


    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(1, 1, 4, 0, 0, 0, 0, 1, 0);//1, 1, 4, 0, 1, 0, 0, 1, 0
	glGetDoublev(GL_MODELVIEW_MATRIX, DEBUG_M);



    glPushMatrix();
		glRotated(double(TICK), 0, 1, 0);
		glRotated(double(TICK)*0.1, 0, 0, 1);
		glTranslatef(LIGHTP, LIGHTP, 0);
		glutSolidSphere(0.05, 32, 32);
        glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glPopMatrix();


	//world coordinate
	/*
	glColor3f(1, 1, 1);
	OpenglLine(0, 0, 0, 3, 0, 0);
	glColor3f(1, 1, 1);
	OpenglLine(0, 0, 0, 0, 3, 0);
	glColor3f(1, 1, 1);
	OpenglLine(0, 0, 0, 0, 0, 3);
    */

	glPushMatrix();
		//multiple trackball matrix
		glMultMatrixd(TRACKM);

		glScaled(MODELSCALE, MODELSCALE, MODELSCALE);
		glColor3f(1.0, 1.0, 1.0); //(0.7, 1.0, 1.0)
		glmDraw(MODEL, GLM_SMOOTH);//GLM_FLAT  GLM_SMOOTH
		//glutSolidSphere(1, 20, 20);
	glPopMatrix();

}

/*----------------------------------------------------------------------*/
/*
** These functions implement a simple trackball-like motion control.
*/

float lastPos[3] = {0.0F, 0.0F, 0.0F};
int curx, cury;
int startX, startY;

void trackball_ptov(int x, int y, int width, int height, float v[3])
{
    float d, a;

    /* project x,y onto a hemi-sphere centered within width, height */
    v[0] = (2.0F*x - width) / width;
    v[1] = (height - 2.0F*y) / height;
    d = (float) sqrt(v[0]*v[0] + v[1]*v[1]);
    v[2] = (float) cos((M_PI/2.0F) * ((d < 1.0F) ? d : 1.0F));
    a = 1.0F / (float) sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    v[0] *= a;
    v[1] *= a;
    v[2] *= a;
}


void mouseMotion(int x, int y)
{
    float curPos[3], dx, dy, dz;

    trackball_ptov(x, y, winWidth, winHeight, curPos);
	if(trackingMouse)
	{
		dx = curPos[0] - lastPos[0];
		dy = curPos[1] - lastPos[1];
		dz = curPos[2] - lastPos[2];

		if (dx || dy || dz) {
			angle = 90.0F * sqrt(dx*dx + dy*dy + dz*dz);

			axis[0] = lastPos[1]*curPos[2] - lastPos[2]*curPos[1];
			axis[1] = lastPos[2]*curPos[0] - lastPos[0]*curPos[2];
			axis[2] = lastPos[0]*curPos[1] - lastPos[1]*curPos[0];

			lastPos[0] = curPos[0];
			lastPos[1] = curPos[1];
			lastPos[2] = curPos[2];
		}
	}
    glutPostRedisplay();
}

void startMotion(int x, int y)
{
    trackingMouse = true;
    redrawContinue = false;
    startX = x; startY = y;
    curx = x; cury = y;
    trackball_ptov(x, y, winWidth, winHeight, lastPos);
	trackballMove=true;
}

void stopMotion(int x, int y)
{
	trackingMouse = false;

    if (startX != x || startY != y) {
		redrawContinue = true;
    } else {
		angle = 0.0F;
		redrawContinue = false;
		trackballMove = false;
    }
}

/*----------------------------------------------------------------------*/

void displayfont()
{
    //Font
	char mss[30]="Ordinary Shading";
	//sprintf(mss, "Score %d", Gamescore);
	glColor3f(1.0, 1.0, 1.0);  //set font color
	void * font = GLUT_BITMAP_9_BY_15;

	glWindowPos2i(30, winHeight-20);    //set font start position, (30, winHeight-20)
	for(unsigned int i=0; i<strlen(mss); i++) {
		glutBitmapCharacter(font, mss[i]);
	}

	char mss1[30]="Xshading";
	glWindowPos2i(10+(winWidth/2), winHeight-20);    //set font start position
	for(unsigned int i=0; i<strlen(mss1); i++) {
		glutBitmapCharacter(font, mss1[i]);
	}
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	displayfont();

    if (trackballMove) {
		glPushMatrix();
			glLoadMatrixd(TRACKM);
			glRotatef(angle, axis[0], axis[1], axis[2]);
			glGetDoublev(GL_MODELVIEW_MATRIX, TRACKM);
		glPopMatrix();
	}

	openglPath();

	//we must disable the opengl's depth test, then the software depth test will work

	//glDisable(GL_DEPTH_TEST);
	//glDisable(GL_LIGHTING);
	softPath();
	//glEnable(GL_LIGHTING);
	//glEnable(GL_DEPTH_TEST);

    glutSwapBuffers();
}

/*----------------------------------------------------------------------*/

void mouseButton(int button, int state, int x, int y)
{
	if(button==GLUT_RIGHT_BUTTON) {
		exit(0);
	}

	if(button==GLUT_LEFT_BUTTON) switch(state)
	{
		case GLUT_DOWN:
			y=winHeight-y;
			startMotion(x, y);
			break;
		case GLUT_UP:
			y=winHeight-y;
			stopMotion(x, y);
			break;
    }
}

void myReshape(int w, int h)
{
    winWidth = w;
    winHeight = h;

	swInitZbuffer(w/2, h);
}

void spinCube()
{
    if (redrawContinue) glutPostRedisplay();
}

void update(int i)
{
	TICK++;
	int temp=TICK%180;
	if(temp<90)
		Angle1++;
	else
		Angle1--;

	//int temp2=TICK%90;
	if(temp<90)
		Angle2+=0.5;
	else
		Angle2-=0.5;

	glutPostRedisplay();
	glutTimerFunc(33, update, ++i);
}

void myKeys(unsigned char key, int x, int y)
{
	switch(key)
	{
		case ' ':
			glPushMatrix();
				glLoadIdentity();
				glGetDoublev(GL_MODELVIEW_MATRIX, TRACKM);
			glPopMatrix();

			Angle1=0, Angle2=0;
			TICK=0;
			MODELSCALE = 1.5;
			LIGHTP = 1.5;
			break;

		case 'a':
			MODELSCALE += 0.5;
			break;
		case 's':
			if(MODELSCALE > 1)
				MODELSCALE -= 0.5;
			break;

		case 'z':
			LIGHTP += 0.1;
			std::cout<<"LIGHTP "<<LIGHTP<<'\n';
			break;
		case 'x':
			LIGHTP -= 0.1;
			std::cout<<"LIGHTP "<<LIGHTP<<'\n';
			break;

        case 'Q':
        case 'q':
			exit(0);
			break;

        case '0':
			DRAWTYPE=0;
			break;
        case '1':
			DRAWTYPE=1;
			break;
        case '2':
			DRAWTYPE=2;
			break;
        case '3':
			DRAWTYPE=3;
			break;
	}
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
    if(cnt<1){
            std::cout<<"main"<<'\n';

            //findAdjustway(MODEL);

            cnt+=1;
    }
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1000, 500);
    glutCreateWindow("Xshading");

    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);
    glutIdleFunc(spinCube);
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMotion);
	glutKeyboardFunc(myKeys);
	glutTimerFunc(33, update, 0);

	glEnable(GL_DEPTH_TEST);
	//Font = FontCreate(wglGetCurrentDC(), "Times", 32, 0, 1);



	//Read model
	MODEL = glmReadOBJ("bunny.obj");//Millennium_Falcon
	glmUnitize(MODEL);
	glmFacetNormals(MODEL);
	glmVertexNormals(MODEL, 60);

    // Light values and coordinates
    GLfloat  ambientLight[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    GLfloat  diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    GLfloat  specular[] = { 5.0f, 0.7f, 0.7f, 1.0f };


    GLfloat  specref[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat  shininess = 32.0f;

    // Enable lighting
    glEnable(GL_LIGHTING);

    // Setup and enable light 0
    glLightfv(GL_LIGHT0,GL_AMBIENT,ambientLight);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseLight);
    glLightfv(GL_LIGHT0,GL_SPECULAR, specular);
    glEnable(GL_LIGHT0);

    // Enable color tracking
    glEnable(GL_COLOR_MATERIAL);
    // Set Material properties to follow glColor values
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// All materials hereafter have full specular reflectivity with a high shine
    glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
    glMateriali(GL_FRONT, GL_SHININESS, shininess);

	// BackgroundColor
	glClearColor(0.7,0.7,0.7,0);

	//
	/*
	swLightfv(GL_LIGHT0,GL_AMBIENT,ambientLight);
    swLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseLight);
    swLightfv(GL_LIGHT0,GL_SPECULAR, specular);

    swMaterialfv(GL_FRONT, GL_SPECULAR, specref);
    swMateriali(GL_FRONT, GL_SHININESS, shininess);
*/

    glutMainLoop();

	return 0;
}


