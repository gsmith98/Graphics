#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "rayGroup.h"
#include <limits>
#include <algorithm>

////////////////////////
//  Ray-tracing stuff //
////////////////////////
double RayGroup::intersect(Ray3D ray,RayIntersectionInfo& iInfo,double mx){
	double dist = -1;
	RayIntersectionInfo inf = RayIntersectionInfo();
	Ray3D transformedRay;

	int hitNum = 0;
	for (int j = 0; j < sNum; j++) {
		transformedRay = Ray3D(getInverseMatrix().multPosition(ray.position), getInverseMatrix().multDirection(ray.direction).unit());
		double bboxdist = shapes[j]->bBox.intersect(transformedRay);
		if (!(bboxdist < 0 || (bboxdist > mx && mx > 0))) {
			hits[hitNum].shape = shapes[j];
			hits[hitNum].t = bboxdist;
			hitNum++;
		}
	}

	qsort(hits, hitNum, sizeof(RayShapeHit), RayShapeHit::Compare);

	for (int i = 0; i < hitNum; i++) 
	{
		transformedRay = Ray3D(getInverseMatrix().multPosition(ray.position), getInverseMatrix().multDirection(ray.direction).unit());
		dist = hits[i].shape->intersect(transformedRay, inf, mx);

		Point3D hitCoord;
		if (dist != -1) {
			hitCoord = getMatrix().multPosition(inf.iCoordinate);
			dist = (hitCoord - ray.position).length();
		}

		if (dist != -1 && (mx <= 0 || dist < mx)) {
			iInfo = inf;
			iInfo.iCoordinate = hitCoord;
			iInfo.normal = getNormalMatrix().multDirection(iInfo.normal).unit();
			return dist;
		}
	}
	
	return -1;
}

BoundingBox3D RayGroup::setBoundingBox(void){
	bBox = BoundingBox3D();
	for (int i = 0; i < sNum; i++) {
		shapes[i]->setBoundingBox();
		bBox += shapes[i]->bBox.transform(getMatrix());
	}
	return bBox;

}

int StaticRayGroup::set(void){
	Matrix4D matrix = getMatrix();
	inverseTransform = matrix.invert();
	normalTransform = matrix.transpose().invert();
	return 1;
}
//////////////////
// OpenGL stuff //
//////////////////

int RayGroup::drawOpenGL(int materialIndex){
	return -1;
}

/////////////////////
// Animation Stuff //
/////////////////////
Matrix4D ParametrizedEulerAnglesAndTranslation::getMatrix(void){
	return Matrix4D::IdentityMatrix();
}
Matrix4D ParametrizedClosestRotationAndTranslation::getMatrix(void){
	return Matrix4D::IdentityMatrix();
}
Matrix4D ParametrizedRotationLogarithmAndTranslation::getMatrix(void){
	return Matrix4D::IdentityMatrix();
}
Matrix4D ParametrizedQuaternionAndTranslation::getMatrix(void){
	return Matrix4D::IdentityMatrix();
}
