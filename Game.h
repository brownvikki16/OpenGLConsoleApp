#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>

class Game
{

public:
	Game();
	~Game();

	glm::vec3 ProcessGameInput(bool* keys, GLfloat deltaTime);

private:
	GLfloat playerSpeed;
	glm::vec3 PlayerPos;
};

