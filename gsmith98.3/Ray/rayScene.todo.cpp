#include "rayScene.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>


///////////////////////
// Ray-tracing stuff //
///////////////////////

Point3D RayScene::Reflect(Point3D v,Point3D n){
	return v - n.scale(2 * (v).dot(n));
}

int RayScene::Refract(Point3D v,Point3D n,double ir,Point3D& refract){ 
	refract = v; return 1; // uncomment for reference image

	double Ni = 1.0;
	double Nr = ir;

	if (v.dot(n) > 0) { //leaving (not entering) Nr region (if solid) (should avoid looking at triangles from the back)
		Ni = ir;
		Nr = 1;
		n = -n;
	}

	double thetai = acos(n.dot(-v) / v.length());
	double sinthetar = (Ni/Nr) * sin(thetai);
	if (sinthetar < -1 || sinthetar > 1) return 0;
	double thetar = asin(sinthetar);
	refract = n.scale((Ni/Nr)*cos(thetai) - cos(thetar)) + v.scale((Ni/Nr));
	return 1;
}

Ray3D RayScene::GetRay(RayCamera* camera,int i,int j,int width,int height){
	double d = height / (2.0 * tan((camera->heightAngle/2.0)));
	Point3D p0 = camera->position;
	Point3D pij = camera->direction.scale(d) + camera->right.scale(i + .5 - .5*width) + camera->up.scale(j + .5 - .5*height);
	Ray3D ray = Ray3D(p0, pij.unit());
	return Ray3D(p0, pij.unit());
}

Point3D RayScene::GetColor(Ray3D ray, int rDepth, Point3D cLimit) {
	static int recursiveCalls = 0;
	static Point3D potentialchange = Point3D(1,1,1);

	if (recursiveCalls >= rDepth || potentialchange.length() < cLimit.length()) return Point3D(); //using climit length is close enough

	RayIntersectionInfo iInfo = RayIntersectionInfo();
	double dist = group->intersect(ray, iInfo, -1);

	if (dist == -1) { 
		return (recursiveCalls > 0) ? Point3D() : background; // uncomment for reference image
		return background; 
	}
	else {
		Point3D rgb = Point3D();
		rgb = iInfo.material->emissive + iInfo.material->ambient * (*this).ambient;
		
		for (int l = 0; l < lightNum; l++) {
			Point3D shadowTerm = lights[l]->transparency(iInfo, group, cLimit);
			Point3D diffuse = lights[l]->getDiffuse(ray.position, iInfo);
			Point3D specular = lights[l]->getSpecular(ray.position, iInfo);
			
			rgb += (diffuse +specular)*shadowTerm;
			for (int i = 0; i < 3; i++) {
				if (rgb[i] < 0) rgb[i] = 0;
				if (rgb[i] > 1) rgb[i] = 1;
			}
		}

		Point3D reflected = Point3D();
		if (iInfo.normal.dot(ray.direction) < 0) { //only reflect off of outer surfaces (differs from slides, but in line with reference. Intuitionally the inside is not as reflective)
			Point3D reflectDirection = Reflect(ray.direction, iInfo.normal);
			Ray3D reflectRay = Ray3D(iInfo.iCoordinate + reflectDirection.scale(.0001), reflectDirection);
			potentialchange *= iInfo.material->specular;
			recursiveCalls++;
			reflected = iInfo.material->specular * GetColor(reflectRay, rDepth, cLimit);
			recursiveCalls--;
			potentialchange /= iInfo.material->specular;
		}
		rgb += reflected;
		for (int i = 0; i < 3; i++) {
			if (rgb[i] < 0) rgb[i] = 0;
			if (rgb[i] > 1) rgb[i] = 1;
		}

		Point3D transparent = Point3D();
		Point3D refractedDirection;
		if (Refract(ray.direction, iInfo.normal, iInfo.material->refind, refractedDirection)) {
			Ray3D refractedRay = Ray3D(iInfo.iCoordinate + refractedDirection.scale(.0001), refractedDirection);
			potentialchange *= iInfo.material->transparent;
			recursiveCalls++;
			transparent = iInfo.material->transparent * GetColor(refractedRay, rDepth, cLimit);
			recursiveCalls--;
			potentialchange /= iInfo.material->transparent;
		}

		Point3D antitransparent = Point3D(1, 1, 1) - iInfo.material->transparent;
		antitransparent = Point3D(1, 1, 1); // uncomment for reference image
		rgb = rgb*antitransparent + transparent;
		for (int i = 0; i < 3; i++) {
			if (rgb[i] < 0) rgb[i] = 0;
			if (rgb[i] > 1) rgb[i] = 1;
		}

		return rgb;
	}
}

//////////////////
// OpenGL stuff //
//////////////////
void RayMaterial::drawOpenGL(void){
	
	GLfloat ambientLight[4] = { ambient[0], ambient[1], ambient[2], 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	GLfloat material_diffuse_color[4] = { diffuse[0], diffuse[1], diffuse[2], 1.f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_diffuse_color);
	GLfloat material_specular_color[4] = { specular[0], specular[1], specular[2], 1.f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_specular_color);
	GLfloat material_emissive_color[4] = { emissive[0], emissive[1], emissive[2], 1.f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material_emissive_color);
	GLfloat material_shininess[1] = { specularFallOff };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, material_shininess);
	

	if (this->tex != 0) {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, tex->openGLHandle);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
}
void RayTexture::setUpOpenGL(void){
	unsigned char * chars = new unsigned char[img->width()*img->height() * 4];

	for (int i = 0; i < img->width(); i++) {
		for (int j = 0; j < img->height(); j++) {
			chars[j * img->width() * 4 + i * 4 + 0] = (GLubyte) img->pixel(i, j).r;
			chars[j * img->width() * 4 + i * 4 + 1] = (GLubyte) img->pixel(i, j).g;
			chars[j * img->width() * 4 + i * 4 + 2] = (GLubyte) img->pixel(i, j).b;
			chars[j * img->width() * 4 + i * 4 + 3] = (GLubyte) img->pixel(i, j).a;
		}
	}

	glGenTextures(1, &openGLHandle);
	glBindTexture(GL_TEXTURE_2D, openGLHandle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->width(), img->height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)&chars[0]);
	glBindTexture(GL_TEXTURE_2D, 0);
}
