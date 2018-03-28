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
	Matrix3D Rx = Matrix3D();
	Matrix3D Ry = Matrix3D();
	Matrix3D Rz = Matrix3D();
	double theta = e.p[0];
	double phi = e.p[1];
	double psi = e.p[2];

	Rx.m[1][1] = Rx.m[2][2] = cos(theta);
	Rx.m[1][2] = -sin(theta);
	Rx.m[2][1] = sin(theta);

	Ry.m[0][0] = Ry.m[2][2] = cos(phi);
	Ry.m[0][2] = sin(phi);
	Ry.m[2][0] = -sin(phi);

	Rz.m[0][0] = Rz.m[1][1] = cos(psi);
	Rz.m[0][1] = -sin(psi);
	Rz.m[1][0] = sin(psi);


	(*this) = Rx*Ry*Rz;
}

Matrix3D::Matrix3D(const Quaternion& quat){

	Quaternion q = quat;
	if (quat.length() != 1) q = quat / quat.squareNorm();

	double a = q.real;
	double b = q.imag.p[0];
	double c = q.imag.p[1];
	double d = q.imag.p[2];

	Matrix3D r = Matrix3D();

	r.m[0][0] = 1 - 2 * c*c - 2 * d*d;
	r.m[1][0] = 2 * b*c - 2 * a*d;
	r.m[2][0] = 2 * b*d + 2 * a*c;
	r.m[0][1] = 2 * b*c + 2 * a*d;
	r.m[1][1] = 1 - 2 * c*c - 2 * d*d;
	r.m[2][1] = 2 * c*d - 2 * a*b;
	r.m[0][2] = 2 * b*d - 2 * a*c;
	r.m[1][2] = 2 * c*d + 2 * a*b;
	r.m[2][2] = 1 - 2 * b*b - 2 * c*c;

	(*this)=r;
}
Matrix3D Matrix3D::closestRotation(void) const {
	Matrix3D r1 = Matrix3D();
	Matrix3D D = Matrix3D();
	Matrix3D r2 = Matrix3D();
	this->SVD(r1, D, r2);

	D.m[0][0] = copysign(1, D.m[0][0]);
	D.m[1][1] = copysign(1, D.m[1][1]);
	D.m[2][2] = copysign(1, D.m[2][2]);

	if (D.m[0][0] * D.m[1][1] * D.m[2][2] < 0) {
		D.m[2][2] = D.m[2][2] * -1; //strictly non-incereasing diagonal entries from the SVD call, flip smallest so trace = 1
	}

	return r1*D*r2;
}
/* While these Exp and Log implementations are the direct implementations of the Taylor series, the Log
 * function tends to run into convergence issues so we use the other ones:*/
Matrix3D Matrix3D::Exp(const Matrix3D& m,int iter){
	Matrix3D mapprox = IdentityMatrix();
	for (int k = 1; k < iter; k++) {
		Matrix3D numerator = IdentityMatrix();
		int denom = 1;
		for (int count = 0; count < k; count++) {
			numerator *= m;
			denom *= (count + 1);
		}
		mapprox += numerator / denom;
	}

	return mapprox;
}
