#pragma once

#include <gl/glew.h>

class Material
{
public:
	Material();
	Material(GLfloat sIntensity, GLfloat shine);

	void UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation);


	~Material();


private:
	GLfloat specularIntensity; //reflectiveness of the object
	GLfloat shininess; //specular power, how smooth the surface looks

};

