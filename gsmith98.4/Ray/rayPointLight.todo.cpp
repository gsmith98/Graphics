#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "rayPointLight.h"
#include "rayScene.h"

////////////////////////
//  Ray-tracing stuff //
////////////////////////
Point3D RayPointLight::getDiffuse(Point3D cameraPosition, RayIntersectionInfo& iInfo) {
	double dist = (location - iInfo.iCoordinate).length();
	Point3D Il = color / (constAtten + linearAtten * dist + quadAtten * pow(dist, 2));
	Point3D L = (location - iInfo.iCoordinate).unit();

	double lightFacingness = iInfo.normal.dot(L.unit());
	if (lightFacingness < 0) return Point3D();
	if ((iInfo.iCoordinate - cameraPosition).dot(iInfo.normal) >= 0) return Point3D(); //either an inside surface or grazing shot
	return iInfo.material->diffuse * lightFacingness * Il;
}
Point3D RayPointLight::getSpecular(Point3D cameraPosition,RayIntersectionInfo& iInfo){
	double dist = (location - iInfo.iCoordinate).length();
	Point3D Il = color / (constAtten + linearAtten * dist + quadAtten * pow(dist, 2));
	Point3D L = (location - iInfo.iCoordinate).unit();
	Point3D R = -L - iInfo.normal.scale(2 * (-L).dot(iInfo.normal));
	Point3D V = (cameraPosition - iInfo.iCoordinate).unit();

	double lightFacingness = iInfo.normal.dot(L);
	if (lightFacingness < 0) return Point3D();
	if ((iInfo.iCoordinate - cameraPosition).dot(iInfo.normal) >= 0) return Point3D(); //either an inside surface or grazing shot
	double vr = pow(V.dot(R), iInfo.material->specularFallOff);
	return iInfo.material->specular * Il * vr;
}
int RayPointLight::isInShadow(RayIntersectionInfo& iInfo,RayShape* shape){
	RayIntersectionInfo inf = RayIntersectionInfo();
	Point3D pointToLight = (location - iInfo.iCoordinate);

	if (shape->intersect(Ray3D(iInfo.iCoordinate = pointToLight.unit().scale(.0001), pointToLight.unit()), inf, pointToLight.length()) < 0) {
		return 0;
	}
	return 1;
}
Point3D RayPointLight::transparency(RayIntersectionInfo& iInfo,RayShape* shape,Point3D cLimit){
	RayIntersectionInfo inf = RayIntersectionInfo();
	Point3D pointToLight = (location - iInfo.iCoordinate);

	if (shape->intersect(Ray3D(iInfo.iCoordinate + pointToLight.unit().scale(.0001), pointToLight.unit()), inf) < 0) {
		return Point3D(1, 1, 1);
	}
	Point3D trans = inf.normal.dot(-pointToLight) <= 0 ? Point3D(1, 1, 1) : inf.material->transparent;
	return  trans * transparency(inf, shape, cLimit);
}


//////////////////
// OpenGL stuff //
//////////////////
void RayPointLight::drawOpenGL(int index){
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0 + index);
	GLfloat light_color[4] = { color[0], color[1], color[2], 1.0f };
	GLfloat light_position[4] = { location[0], location[1], location[2],  1.0f };
	glLightfv(GL_LIGHT0 + index, GL_DIFFUSE, light_color);
	glLightfv(GL_LIGHT0 + index, GL_SPECULAR, light_color);
	glLightfv(GL_LIGHT0 + index, GL_POSITION, light_position);
	GLfloat catten[1] = { constAtten };
	GLfloat latten[1] = { linearAtten };
	GLfloat qatten[1] = { quadAtten };
	glLightfv(GL_LIGHT0 + index, GL_CONSTANT_ATTENUATION, catten);
	glLightfv(GL_LIGHT0 + index, GL_LINEAR_ATTENUATION, latten);
	glLightfv(GL_LIGHT0 + index, GL_QUADRATIC_ATTENUATION, qatten);
}