#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "rayScene.h"
#include "raySpotLight.h"


////////////////////////
//  Ray-tracing stuff //
////////////////////////
Point3D RaySpotLight::getDiffuse(Point3D cameraPosition,RayIntersectionInfo& iInfo){
	double dist = (location - iInfo.iCoordinate).length();
	Point3D Il;
	Point3D L = (location - iInfo.iCoordinate).unit();
	double dv = direction.unit().dot(-L);
	if (dv <= cos(cutOffAngle)) return Point3D();
	Il = (color * pow(dv, dropOffRate)) / (constAtten + linearAtten * dist + quadAtten * pow(dist, 2));

	double lightFacingness = iInfo.normal.dot(L.unit());
	if (lightFacingness < 0) return Point3D();
	if ((iInfo.iCoordinate - cameraPosition).dot(iInfo.normal) >= 0) return Point3D(); //either an inside surface or grazing shot
	return iInfo.material->diffuse * lightFacingness * Il;
}
Point3D RaySpotLight::getSpecular(Point3D cameraPosition,RayIntersectionInfo& iInfo){
	double dist = (location - iInfo.iCoordinate).length();
	Point3D Il;
	Point3D L = (location - iInfo.iCoordinate).unit();
	double dv = direction.unit().dot(-L);
	if (dv <= cos(cutOffAngle)) return Point3D();
	Il = (color * pow(dv, dropOffRate)) / (constAtten + linearAtten * dist + quadAtten * pow(dist, 2));
	Point3D R = -L - iInfo.normal.scale(2 * (-L).dot(iInfo.normal));
	Point3D V = (cameraPosition - iInfo.iCoordinate).unit();

	double lightFacingness = iInfo.normal.dot(L);
	if (lightFacingness < 0) return Point3D();
	if ((iInfo.iCoordinate - cameraPosition).dot(iInfo.normal) >= 0) return Point3D(); //either an inside surface or grazing shot
	double vr = pow(V.dot(R), iInfo.material->specularFallOff);
	return iInfo.material->specular * Il * vr;
}
int RaySpotLight::isInShadow(RayIntersectionInfo& iInfo,RayShape* shape){
	RayIntersectionInfo inf = RayIntersectionInfo();
	Point3D pointToLight = (location - iInfo.iCoordinate);

	if (shape->intersect(Ray3D(iInfo.iCoordinate = pointToLight.unit().scale(.0001), pointToLight.unit()), inf, pointToLight.length()) < 0) {
		return 0;
	}
	return 1;
}
Point3D RaySpotLight::transparency(RayIntersectionInfo& iInfo,RayShape* shape,Point3D cLimit){
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
void RaySpotLight::drawOpenGL(int index){
}