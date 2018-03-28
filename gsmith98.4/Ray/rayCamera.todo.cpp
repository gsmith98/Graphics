#include <GL/glew.h>
#include <GL/glut.h>
#include <math.h>
#include "rayCamera.h"



//////////////////
// OpenGL stuff //
//////////////////
void RayCamera::drawOpenGL(void){
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	Point3D target = position + direction;
	gluLookAt(position[0], position[1], position[2], target[0], target[1], target[2], up[0], up[1], up[2]);
}
void RayCamera::rotateUp(Point3D center,float angle){
	//Matrix4D mat = Matrix4D(Matrix3D(), -center); //translates center to origin 
	//Matrix3D mat3 = Matrix3D(Point3D(angle, angle, angle));
	//Matrix4D mat4 = (mat3, Point3D());
	//printf("before\n");
	//printf("%f, %f, %f\n", position[0], position[1], position[2]);
	//position = mat4.multPosition(position);
	//printf("after\n");
	
	
	//printf("%f, %f, %f\n", direction[0], direction[1], direction[2]);

	//gluLookAt(position[0]+1, position[1], position[2], direction[0], direction[1], direction[2], up[0], up[1], up[2]);
	//position[0] = 4;
	
	float rad = (position - center).length();
	position -= right * rad * tan(angle);
	Point3D viewvect = center - position;
	position += viewvect.unit().scale(viewvect.length() - rad);
	direction = center - position;

}
void RayCamera::rotateRight(Point3D center,float angle){
	float rad = (position - center).length();
	position += up * rad * tan(angle);
	Point3D viewvect = center - position;
	position += viewvect.unit().scale(viewvect.length() - rad);
	direction = center - position;
}
void RayCamera::moveForward(float dist){
}
void RayCamera::moveRight(float dist){
}
void RayCamera::moveUp(float dist){
}
