#include "Camera.h"

Camera::Camera()
{
}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startTurnSpeed, GLfloat startMoveSpeed)
{
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	movementSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;

	update();
}

void Camera::keyControl(bool* keys, GLfloat deltaTime)
{
	GLfloat velocity = movementSpeed * deltaTime;

	//if W is pressed, move forward
	if (keys[GLFW_KEY_W])
	{
		position += front * velocity;
	}

	//move backwards
	if (keys[GLFW_KEY_S])
	{
		position -= front * velocity;
	}
	
	//move left
	if (keys[GLFW_KEY_A])
	{
		position -= right * velocity;
	}

	//move right
	if (keys[GLFW_KEY_D])
	{
		position += right * velocity;
	}
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	//limit pitch movement
	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}

	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	//call update
	update();
	
}


glm::mat4 Camera::calculateViewMatrix()
{
	//in a game change position + front to position of main character
	return glm::lookAt(position, position + front, up);

}

void Camera::update()
{
	//calculate front based on yaw and pitch
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	//calculate the right vector by cross product of front and worldUp
	right = glm::normalize(glm::cross(front, worldUp));
	
	//calculate the up vector by cross product of right and front
	up = glm::normalize(glm::cross(right, front));
}

Camera::~Camera()
{
}