#include "Game.h"


Game::Game()
{
	playerSpeed = 0;
	playerPos = glm::vec3(0.0f, 0.0f, 0.0f);
}

Game::Game(GLfloat speed, glm::vec3 playerPosition)
{
	playerSpeed = speed;
	playerPos = playerPosition;

}

Game::~Game()
{
	playerSpeed = 0.0f;
	playerPos = glm::vec3(0.0f, 0.0f, 0.0f);
}


glm::vec3 Game::ProcessGameInput(bool* keys, GLfloat deltaTime)
{
	
	GLfloat velocity = playerSpeed * deltaTime;

	//if W is pressed, move forward
	if (keys[GLFW_KEY_Q])
	{
		playerPos.x += velocity;
		return playerPos;
	}

	////move backwards
	//if (keys[GLFW_KEY_S])
	//{
	//	position.z -= velocity;
	//	return position;
	//}

	////move left
	//if (keys[GLFW_KEY_A])
	//{
	//	position.x -= velocity;
	//	return position;
	//}

	////move right
	//if (keys[GLFW_KEY_D])
	//{
	//	position.x += velocity;
	//	return position;
	//}

	return playerPos;
}
