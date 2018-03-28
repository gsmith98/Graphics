#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "rayTriangle.h"
#include <algorithm>

////////////////////////
//  Ray-tracing stuff //
////////////////////////
void RayTriangle::initialize(void){
	plane = Plane3D(v[0]->position, v[1]->position, v[2]->position);
	v1 = v[1]->position - v[0]->position; //T2 - T1
	v2 = v[2]->position - v[0]->position; //T3 - T1
}

double RayTriangle::intersect(Ray3D ray, RayIntersectionInfo& iInfo, double mx) {
	//intersect plane
	double divisor = ray.direction.dot(plane.normal);
	double dist = -(ray.position.dot(plane.normal) - plane.distance) / divisor;
	if (dist < 0 || (dist > mx && mx > 0)) return -1;

	//solve:     v1.x * B + v2.x * Y = (p-T1).x
	//           v1.y * B + v2.y * Y = (p-T1).y 
	//           v1.z * B + v2.z * Y = (p-T1).z
	//
	//   We only need 2 of these equations, but any one of them could prove useless. The y one is useless when the triangle is in the y=0 plane. 
	//           
	// 1-B-Y = A  and A, B, Y >= 0 ? if not no intersect
	//
	//Ax+By=C
	//Dx+Ey=F  --> x = CE-BF/AE-BD  and y = AF-CD/AE-BD

	double a, b, c, d, e, f;
	Point3D v3 = ray(dist) - v[0]->position;
	
	int i = 0;
	int j = 1;
	double detdiv = 0;
	while (true) {
		a = v1[i];
		b = v2[i];
		c = v3[i];
		d = v1[j];
		e = v2[j];
		f = v3[j];
		detdiv = (a*e - b*d);
		if (detdiv != 0) {
			break;
		}
		i = (i + 1) % 3;
		j = (j + 1) % 3;
	}

	double B = (c*e - b*f) / detdiv;
	double Y = (a*f - c*d) / detdiv;
	double A = 1 - B - Y;

	if (A >= 0 && B >= 0 && Y >= 0) {
		iInfo.iCoordinate = ray(dist);
		iInfo.material = (*this).material;
		iInfo.normal = plane.normal;
		//iInfo.texCoordinate = NULL; 
		return dist;
	}

	return -1;
}
BoundingBox3D RayTriangle::setBoundingBox(void){
	Point3D list[3] = { v[0]->position, v[1]->position, v[2]->position };
	bBox = BoundingBox3D(list, 3); 
	return bBox;
}

//////////////////
// OpenGL stuff //
//////////////////
int RayTriangle::drawOpenGL(int materialIndex){
	glEnable(GL_TEXTURE_2D);
	material->drawOpenGL();
	
	glBegin(GL_TRIANGLES);
	glNormal3f(plane.normal[0], plane.normal[1], plane.normal[2]);
	glTexCoord2d(v[0]->texCoordinate[0], v[0]->texCoordinate[1]);
	glVertex3f(v[0]->position[0], v[0]->position[1], v[0]->position[2]);
	glNormal3f(plane.normal[0], plane.normal[1], plane.normal[2]);
	glTexCoord2d(v[1]->texCoordinate[0], v[1]->texCoordinate[1]);
	glVertex3f(v[1]->position[0], v[1]->position[1], v[1]->position[2]);
	glNormal3f(plane.normal[0], plane.normal[1], plane.normal[2]);
	glTexCoord2d(v[2]->texCoordinate[0], v[2]->texCoordinate[1]);
	glVertex3f(v[2]->position[0], v[2]->position[1], v[2]->position[2]);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	return -1;
}
