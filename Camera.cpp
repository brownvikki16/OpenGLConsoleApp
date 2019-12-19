#include "Camera.h"
#include "Game.h"

Camera::Camera()
{
	offset = glm::vec3(0.1f, 0.1f, 0.1f);
	
}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startTurnSpeed, GLfloat startMoveSpeed, GLfloat limit)
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

//mode 1 = manual 2 = player
void Camera::keyControl(glm::vec3 playerPos, int mode, bool* keys, GLfloat deltaTime)
{
	GLfloat velocity = movementSpeed * deltaTime;
	enum camMotionState state = dwell;
	
	//if W is pressed, move forward
	if (mode == 0)
	{

		if (keys[GLFW_KEY_W])
		{
			position -= camBackward * velocity;
		}

		//move backwards
		if (keys[GLFW_KEY_S])
		{
			position += camBackward * velocity;
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

		//zoom in
		if (keys[GLFW_KEY_Z])
		{
			position += front * velocity;
		}

		//zoom out
		if (keys[GLFW_KEY_X])
		{
			position -= front * velocity;
		}
	}

	if (mode == 1)
	{
		//glm::vec3 playerPos = playerPos;
		glm::vec3 distance = position - playerPos;
		GLfloat thresholdNear = 0.0001f;
		GLfloat thresholdFar = 0.8f;
		GLfloat thresholdFurther = 1.2f;
		//int moveCountdown = 2;
		if (distance.z < 0.0001f || distance.x < 0.0001f)
		{
			state = dwell;
		}

		if (distance.z < 1.0f && distance.z > 0.0001f)
		{
			state = Follow;
		}

		if (distance.z > 1.0f || distance.x > 1.0f)
		{
			state = catchUp;
		}

		//if (distance.x < 0.0001f)
		//{
		//	state = dwell;
		//}

		if (distance.x < 1.0f && distance.x > 0.0001f)
		{
			state = Follow;
		}

		//if (distance.x > 1.0f)
		//{
		//	state = catchUp;
		//}
		
		
		if (keys[GLFW_KEY_W])
		{

			//implementing states

			if (state == dwell)
			{
				position.z = playerPos.z;
				//moveCountdown = 2;

			}

			if (state == Follow)
			{
				position.z += thresholdNear * velocity;
			}

			if (state == catchUp)
			{
				position.z += thresholdFar * velocity;
			}


			/*printf("new cam position: (%f, %f, %f) \n", position.x, position.y, position.z);
			printf("new player/target position: (%f, %f, %f) \n", playerPos.x, playerPos.y, playerPos.z);
			printf("new distance: (%f, %f, %f) \n", distance.x, distance.y, distance.z);*/
			//printf("velocity: %f \n", velocity);

		}

		if (keys[GLFW_KEY_A])
		{


			//implementing states

			if (state == dwell)
			{
				position.x = playerPos.x;
				//moveCountdown = 2;

			}

			if (state == Follow)
			{
				position.x += thresholdNear * velocity;
			}

			if (state == catchUp)
			{
				position.x += thresholdFar * velocity;
			}


			/*printf("new cam position: (%f, %f, %f) \n", position.x, position.y, position.z);
			printf("new player/target position: (%f, %f, %f) \n", playerPos.x, playerPos.y, playerPos.z);
			printf("new distance: (%f, %f, %f) \n", distance.x, distance.y, distance.z);*/
			//printf("velocity: %f \n", velocity);
		}

		if (keys[GLFW_KEY_S])
		{

			//implementing states

			if (state == dwell)
			{
			position.z = playerPos.z;
			//moveCountdown = 2;

			}

			if (state == Follow)
			{
			position.z += -(thresholdNear) * velocity;
			}

			if (state == catchUp)
			{
			position.z += -(thresholdFar) * velocity;
			}


	/*	printf("new cam position: (%f, %f, %f) \n", position.x, position.y, position.z);
		printf("new player/target position: (%f, %f, %f) \n", playerPos.x, playerPos.y, playerPos.z);
		printf("new distance: (%f, %f, %f) \n", distance.x, distance.y, distance.z);
		printf("velocity: %f \n", velocity);*/
		}

		if (keys[GLFW_KEY_D])
		{
			//implementing states

			if (state == dwell)
			{
				position.x = playerPos.x;
				//moveCountdown = 2;
			}

			if (state == Follow)
			{
				position.x += -(thresholdNear) * velocity;
			}

			if (state == catchUp)
			{
				position.x += -(thresholdFar) * velocity;
			}


			/*printf("new cam position: (%f, %f, %f) \n", position.x, position.y, position.z);
			printf("new player/target position: (%f, %f, %f) \n", playerPos.x, playerPos.y, playerPos.z);
			printf("new distance: (%f, %f, %f) \n", distance.x, distance.y, distance.z);*/
			//printf("velocity: %f \n", velocity);
		}

	
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

glm::vec3 Camera::getCameraPosition()
{
	return position;
}



glm::mat4 Camera::calculateViewMatrix()
{
	GLfloat sinFactor = sin(glfwGetTime());
	//GLfloat cosFactor = cos(glfwGetTime());
	//GLfloat tanFactor = tan(glfwGetTime());
	glm::vec3 camPosShake = glm::vec3(position.x, position.y, position.z);
	
	//in a game change position + front to position of main character
	return glm::lookAt(camPosShake, position + front, up);

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

	//set backward vector to right and rotate it
	camBackward = right; 
	camBackward.x = cos(glm::radians(yaw));
	camBackward.z = sin(glm::radians(yaw));
}

Camera::~Camera()
{
}
