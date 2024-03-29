#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>

#include "Game.h"

class Camera
{
public:
	Camera();
	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startTurnSpeed, GLfloat startMoveSpeed, GLfloat limit);

	void keyControl(glm::vec3 playerPos, int mode, bool* keys, GLfloat deltaTime);

	void mouseControl(GLfloat xChange, GLfloat yChange);

	glm::vec3 getCameraPosition();

	glm::mat4 calculateViewMatrix();

	~Camera();

private:
	glm::vec3 position;	//position of camera in x, y, z
	glm::vec3 front;	//front vector of camera
	glm::vec3 up;		//up vector of camera
	glm::vec3 right;	//right vector of camera
	glm::vec3 worldUp;	//frame of reference for the actual WORLD up
	glm::vec3 camBackward; //vector for camera pointing backward

	GLfloat yaw;
	GLfloat pitch;
	//GLfloat roll;

	GLfloat movementSpeed;
	GLfloat turnSpeed;
	glm::vec3 offset;

	enum camMotionState {dwell, catchUp, Follow};


	void update();
};

