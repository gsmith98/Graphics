#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "rayScene.h"
#include "rayBox.h"
#include <limits>
#include <algorithm>

////////////////////////
//  Ray-tracing stuff //
////////////////////////
double RayBox::intersect(Ray3D ray,RayIntersectionInfo& iInfo,double mx){
	//From bounding box implementation
	Point3D minp = center - length.scale(.5);
	Point3D maxp = center + length.scale(.5);

	//t values corresponding to minplane/maxplane intersections
	double intersectminx = (minp[0] - ray.position.p[0]) / ray.direction.p[0];
	double intersectmaxx = (maxp[0] - ray.position.p[0]) / ray.direction.p[0];
	double intersectminy = (minp[1] - ray.position.p[1]) / ray.direction.p[1];
	double intersectmaxy = (maxp[1] - ray.position.p[1]) / ray.direction.p[1];
	double intersectminz = (minp[2] - ray.position.p[2]) / ray.direction.p[2];
	double intersectmaxz = (maxp[2] - ray.position.p[2]) / ray.direction.p[2];

	// min or above t values for each coordinate
	double intersectclosestx = std::min(intersectminx, intersectmaxx);
	double intersectclosesty = std::min(intersectminy, intersectmaxy);
	double intersectclosestz = std::min(intersectminz, intersectmaxz);

	if (intersectclosestx > std::numeric_limits<double>::max()) intersectclosestx = std::numeric_limits<double>::min();
	if (intersectclosesty > std::numeric_limits<double>::max()) intersectclosesty = std::numeric_limits<double>::min();
	if (intersectclosestz > std::numeric_limits<double>::max()) intersectclosestz = std::numeric_limits<double>::min();

	// t value candidate for first touch with box
	double striking = std::max({ intersectclosestx, intersectclosesty, intersectclosestz });
	Point3D hitpoint = ray.position + ray.direction.scale(striking);
	double err = 0.0001;
	if (hitpoint[0] >= minp[0] - err && hitpoint[0] <= maxp[0] + err && hitpoint[1] >= minp[1] - err && hitpoint[1] <= maxp[1] + err && hitpoint[2] >= minp[2] - err && hitpoint[2] <= maxp[2] + err && (striking < mx || mx <= 0)) {
		iInfo.iCoordinate = ray(striking);
		iInfo.material = (*this).material;
		double err = 0.0001;
		Point3D negNorms = iInfo.iCoordinate - minp;
		Point3D posNorms = iInfo.iCoordinate - maxp;
		if (negNorms[0] > 0 - err && negNorms[0] < 0 + err) iInfo.normal = Point3D(-1, 0, 0);
		else if (posNorms[0] > 0 - err && posNorms[0] < 0 + err) iInfo.normal = Point3D(1, 0, 0);
		else if (negNorms[1] > 0 - err && negNorms[1] < 0 + err) iInfo.normal = Point3D(0, -1, 0);
		else if (posNorms[1] > 0 - err && posNorms[1] < 0 + err) iInfo.normal = Point3D(0, 1, 0);
		else if (negNorms[2] > 0 - err && negNorms[2] < 0 + err) iInfo.normal = Point3D(0, 0, -1);
		else if (posNorms[2] > 0 - err && posNorms[2] < 0 + err) iInfo.normal = Point3D(0, 0, 1);
		else printf("ERROR!\n");
		//iInfo.texCoordinate = NULL; 
		return striking;
	}
	
	return -1;

}
BoundingBox3D RayBox::setBoundingBox(void){
	bBox = BoundingBox3D(center - length.scale(.5), center + length.scale(.5));
	return bBox;
}

//////////////////
// OpenGL stuff //
//////////////////
int RayBox::drawOpenGL(int materialIndex){
	material->drawOpenGL();
	glPushMatrix();
	glTranslatef(center[0], center[1], center[2]); //this should first scale then translate
	glScalef(length[0], length[1], length[2]);     //any rotations will be about origin, not center. If this is not desired,
	glutSolidCube(1);								// should specify center as origin and translate with grp matrix in ray file
	glPopMatrix();
	return -1;
}
