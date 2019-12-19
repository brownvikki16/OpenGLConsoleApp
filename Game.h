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
	
	bool touchedOrb;
	bool connectedOrb;
	int orbCounter;
	
	glm::vec3 ProcessPositionInput(glm::vec3 playerPos, GLfloat speed, bool* keys, GLfloat deltaTime);
	GLfloat ProcessPlayerSpeed(GLfloat speed, bool* keys);
	glm::vec3 GetOrbPosition();
	glm::vec3 GetCollectorPosition();
	bool StartOrbConnection(bool start);

private:
	//GLfloat playerSpeed;
	int gameMode;
	bool shiftPressed;
	glm::vec3 OrbPosition;
	glm::vec3 CollectorPosition;
	enum orbCollisionState {notConnected, attached, connected}; 
};

