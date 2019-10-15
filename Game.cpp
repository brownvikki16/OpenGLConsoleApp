#include "Game.h"

Game::Game()
{
	playerSpeed = 3.0f;
	PlayerPos.x = 0.0f;	PlayerPos.y = 0.0f; PlayerPos.z = 0.0f;

}

Game::~Game()
{
	playerSpeed = 0.0f;
	PlayerPos.x = 0.0f;	PlayerPos.y = 0.0f; PlayerPos.z = 0.0f;
}


glm::vec3 Game::ProcessGameInput(bool* keys, GLfloat deltaTime)
{
	
	GLfloat velocity = playerSpeed * deltaTime;

	//if W is pressed, move forward
	if (keys[GLFW_KEY_Q])
	{
		PlayerPos.z += velocity;
		return PlayerPos;
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

	return PlayerPos;
}
