#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "rayDirectionalLight.h"
#include "rayScene.h"

////////////////////////
//  Ray-tracing stuff //
////////////////////////
Point3D RayDirectionalLight::getDiffuse(Point3D cameraPosition, RayIntersectionInfo& iInfo) {
	Point3D Il = color;
	Point3D L = -direction;

	double lightFacingness = iInfo.normal.dot(L.unit());
	if (lightFacingness < 0) return Point3D(); 
	if ((iInfo.iCoordinate - cameraPosition).dot(iInfo.normal) >= 0) return Point3D(); //either an inside surface or grazing shot
	return iInfo.material->diffuse * lightFacingness * Il;
}
Point3D RayDirectionalLight::getSpecular(Point3D cameraPosition,RayIntersectionInfo& iInfo){
	Point3D Il = color;
	Point3D L = -direction;
	Point3D R = (-L - iInfo.normal.scale(2 * (-L).dot(iInfo.normal))).unit();
	Point3D V = (cameraPosition - iInfo.iCoordinate).unit();

	double lightFacingness = iInfo.normal.dot(L);
	if (lightFacingness < 0) return Point3D(); 
	if ((iInfo.iCoordinate - cameraPosition).dot(iInfo.normal) >= 0) return Point3D(); //either an inside surface or grazing shot
	double vr = pow(V.dot(R), iInfo.material->specularFallOff);
	return iInfo.material->specular * Il * vr;
}
int RayDirectionalLight::isInShadow(RayIntersectionInfo& iInfo,RayShape* shape){

	RayIntersectionInfo inf = RayIntersectionInfo();
	if (shape->intersect(Ray3D(iInfo.iCoordinate + -direction.scale(.0001), -direction), inf) < 0) {
		return 0;
	} 
	return 1;
}
Point3D RayDirectionalLight::transparency(RayIntersectionInfo& iInfo,RayShape* shape,Point3D cLimit){
	RayIntersectionInfo inf = RayIntersectionInfo();
	if (shape->intersect(Ray3D(iInfo.iCoordinate + -direction.scale(.0001), -direction), inf) < 0) {
		return Point3D(1, 1, 1);
	}
	Point3D trans = inf.normal.dot(direction) <= 0 ? Point3D(1,1,1) : inf.material->transparent;
	return  trans * transparency(inf, shape, cLimit);
	
}

//////////////////
// OpenGL stuff //
//////////////////
void RayDirectionalLight::drawOpenGL(int index){
}