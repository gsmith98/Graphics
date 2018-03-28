#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "rayScene.h"
#include "raySphere.h"

////////////////////////
//  Ray-tracing stuff //
////////////////////////
double RaySphere::intersect(Ray3D ray,RayIntersectionInfo& iInfo,double mx){
	Point3D p0 = ray.position;
	Point3D center = (*this).center;
	double r = (*this).radius;
	double a = 1;
	double b = 2 * ray.direction.dot(p0 - center);
	double c = (p0 - center).squareNorm() - pow(r, 2);

	//roots = (-b +- sqrt(b^2 - 4ac))/2a
	double rootterm = pow(b, 2) - 4 * a *c;

	if (rootterm < 0) { //boolean intersect test (false positive if behind camera)
		return -1;
	}

	double root1 = (-b + sqrt(rootterm)) / (2.0 * a); 
	double root2 = (-b - sqrt(rootterm)) / (2.0 * a);
	
	double toReturn = -1;
	if (root1 > 0) {
		iInfo.iCoordinate = ray(root1);
		iInfo.normal = (iInfo.iCoordinate - center).unit();
		toReturn = root1;
	}
	else {
		root1 = root2 + 1;
	}

	if (root2 > 0 && root2 < root1) {
		iInfo.iCoordinate = ray(root2);
		iInfo.normal = (iInfo.iCoordinate - center).unit();
		toReturn = root2;
	}

	iInfo.material = (*this).material;
	//iInfo.texCoordinate = NULL; 

	return toReturn;
}
BoundingBox3D RaySphere::setBoundingBox(void){
	bBox.p[0] = this-> center - Point3D(radius, radius, radius);
	bBox.p[1] = center + Point3D(radius, radius, radius);
	return bBox;
}

//////////////////
// OpenGL stuff //
//////////////////
int RaySphere::drawOpenGL(int materialIndex){
	return -1;
}
