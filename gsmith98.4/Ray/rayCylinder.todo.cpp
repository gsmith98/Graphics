#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "rayScene.h"
#include "rayCylinder.h"


////////////////////////
//  Ray-tracing stuff //
////////////////////////
double RayCylinder::intersect(Ray3D ray,RayIntersectionInfo& iInfo,double mx){
	return -1;
}

BoundingBox3D RayCylinder::setBoundingBox(void){
	Point3D p;
	p = Point3D(radius, height / 2, radius);
	bBox = BoundingBox3D(center + p, center - p);
	return bBox;
}

//////////////////
// OpenGL stuff //
//////////////////
int RayCylinder::drawOpenGL(int materialIndex){
	material->drawOpenGL();
	GLUquadricObj *quad = gluNewQuadric();

	glPushMatrix(); 
	glTranslatef(center[0], center[1], center[2]); 
	glScalef(1, height, 1); 
	glRotatef(-90, 1, 0, 0);
	glTranslatef(0, 0, -.5);
	gluDisk(quad, 0, radius, openGLComplexity, openGLComplexity);
	gluCylinder(quad, radius, radius, 1, openGLComplexity, openGLComplexity);

	glPushMatrix();
	glTranslatef(0, 0, 1);
	gluDisk(quad, 0, radius, openGLComplexity, openGLComplexity);
	glPopMatrix();

	glPopMatrix();

	return -1;
}
