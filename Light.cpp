#include "Light.h"

Light::Light()
{
	color = glm::vec3(1.0f, 1.0f, 1.0f);
	ambientIntensity = 1.0f;

	//direction = glm::vec3(0.0f, -1.0f, 0.0f); //vector pointing down
	diffuseIntensity = 0.0f;
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity)
{
	color = glm::vec3(red, green, blue);
	ambientIntensity = aIntensity;

	//direction = glm::vec3(xDir, yDir, zDir);
	diffuseIntensity = dIntensity;
}


Light::~Light()
{
}
