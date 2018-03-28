#include <stdlib.h>
#include <math.h>

//#include <SVD/SVDFit.h>
//#include <SVD/MatrixMNTC.h>

#include "geometry.h"
#include <algorithm>
#include <limits>

///////////////////////
// Ray-tracing stuff //
///////////////////////
double BoundingBox3D::intersect(const Ray3D& ray) const {
	//http://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection was quite helpful

	Point3D minp = p[0];
	Point3D maxp = p[1];

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
	if (hitpoint[0] >= minp[0] - err && hitpoint[0] <= maxp[0] + err && hitpoint[1] >= minp[1] - err && hitpoint[1] <= maxp[1] + err && hitpoint[2] >= minp[2] - err && hitpoint[2] <= maxp[2] + err) {
		return striking;
	}
	return -1;
}

/////////////////////
// Animation stuff //
/////////////////////
Matrix3D::Matrix3D(const Point3D& e){
	(*this)=Matrix3D();
}

Matrix3D::Matrix3D(const Quaternion& q){
	(*this)=Matrix3D();
}
Matrix3D Matrix3D::closestRotation(void) const {
	return (*this);
}
/* While these Exp and Log implementations are the direct implementations of the Taylor series, the Log
 * function tends to run into convergence issues so we use the other ones:*/
Matrix3D Matrix3D::Exp(const Matrix3D& m,int iter){
	return m;
}
