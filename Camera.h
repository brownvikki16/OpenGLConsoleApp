#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>

class Camera
{
public:
	Camera();
	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startTurnSpeed, GLfloat startMoveSpeed);

	void keyControl(bool* keys, GLfloat deltaTime);

	void mouseControl(GLfloat xChange, GLfloat yChange);

	glm::mat4 calculateViewMatrix();

	~Camera();

private:
	glm::vec3 position;	//position of camera in x, y, z
	glm::vec3 front;	//same but front
	glm::vec3 up;		//same but up
	glm::vec3 right;	//same but right
	glm::vec3 worldUp;	//frame of reference for the actual WORLD up

	GLfloat yaw;
	GLfloat pitch;
	//GLflaot roll;

	GLfloat movementSpeed;
	GLfloat turnSpeed;

	void update();
};

