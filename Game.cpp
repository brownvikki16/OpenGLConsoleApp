#include "Game.h"


Game::Game()
{
	//playerSpeed = 0;
	//playerPos = glm::vec3(0.0f, 0.0f, 0.0f);
	gameMode = 0;
	shiftPressed = false;
}

Game::Game(int mode)
{
	//playerSpeed = speed;
	gameMode = mode;
	//playerPos = playerPosition;
	shiftPressed = false;

}

Game::~Game()
{
	//playerSpeed = 0.0f;
	gameMode = 0;
	//playerPos = glm::vec3(0.0f, 0.0f, 0.0f);
}

glm::vec3 Game::ProcessPositionInput(glm::vec3 playerPos, GLfloat speed, bool* keys, GLfloat deltaTime)
{
	
	GLfloat velocity = speed * deltaTime;

	//if W is pressed, move forward
	if (gameMode == 0)
	{
		return playerPos;
	}
	
	if (gameMode == 1)
	{

		if (keys[GLFW_KEY_W])
		{
			playerPos.z += velocity;
			return playerPos;
		}

		if (keys[GLFW_KEY_S])
		{
			playerPos.z -= velocity;
			return playerPos;
		}

		if (keys[GLFW_KEY_A])
		{
			playerPos.x += velocity;
			return playerPos;
		}

		if (keys[GLFW_KEY_D])
		{
			playerPos.x -= velocity;
			return playerPos;
		}


	}

	return playerPos;
}

GLfloat Game::ProcessPlayerSpeed(GLfloat speed, bool* keys)
{
	if (keys[GLFW_KEY_RIGHT_SHIFT] || shiftPressed == true)
	{
		speed = 3.0f;
		return speed;
		shiftPressed = true;
	}

	if (!keys[GLFW_KEY_RIGHT_SHIFT] || shiftPressed == false)
	{
		speed = 1.0f;
		return speed;
		shiftPressed = false;
	}

	return speed;
}
