#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Light
{
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity);

	//void UseLight(GLfloat ambientIntensityLocation, GLfloat colorLocation, GLfloat diffuseIntensityLocation);

	~Light();

protected:
	glm::vec3 color;
	GLfloat ambientIntensity;

	//glm::vec3 direction;
	GLfloat diffuseIntensity;

};

