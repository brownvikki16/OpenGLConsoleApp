#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>

class Game
{

public:
	Game();
	Game(int mode);
	~Game();
	
	//glm::vec3 playerPos;
	
	glm::vec3 ProcessPositionInput(glm::vec3 playerPos, GLfloat speed, bool* keys, GLfloat deltaTime);
	GLfloat ProcessPlayerSpeed(GLfloat speed, bool* keys);

private:
	//GLfloat playerSpeed;
	int gameMode;
	bool shiftPressed;
};

