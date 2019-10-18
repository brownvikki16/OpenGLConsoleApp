#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>

class Game
{

public:
	Game();
	Game(GLfloat speed, glm::vec3 playerPos);
	~Game();
	
	glm::vec3 playerPos;
	
	glm::vec3 ProcessGameInput(bool* keys, GLfloat deltaTime);

private:
	GLfloat playerSpeed;
};

