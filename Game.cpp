#include "Game.h"


Game::Game()
{
	//playerSpeed = 0;
	//playerPos = glm::vec3(0.0f, 0.0f, 0.0f);
	gameMode = 0;
	shiftPressed = false;
	touchedOrb = false;
	connectedOrb = false;
	orbCounter = 0;
	OrbPosition = glm::vec3(0.0f, 0.0f, 0.0f);
}

Game::Game(int mode)
{
	//playerSpeed = speed;
	touchedOrb = false;
	connectedOrb = false;
	gameMode = mode;
	//playerPos = playerPosition;
	shiftPressed = false;
	orbCounter = 0;
	OrbPosition = glm::vec3(0.0f, 0.0f, 0.0f);

}

Game::~Game()
{
	//playerSpeed = 0.0f;
	gameMode = 0;
	//playerPos = glm::vec3(0.0f, 0.0f, 0.0f);
}

glm::vec3 Game::ProcessPositionInput(glm::vec3 playerPos, GLfloat speed, bool* keys, GLfloat deltaTime)
{
	enum orbCollisionState orbState = notConnected;
	GLfloat velocity = speed * deltaTime;

	//if W is pressed, move forward
	if (gameMode == 0)
	{
		return playerPos;
	}

	//player is in control
	if (gameMode == 1)
	{
		//touchedOrb = false;
		OrbPosition = GetOrbPosition();

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

		//if orb is not connected to collector...
		if (orbState == notConnected && touchedOrb == false)
		{	//check if player has touched it
			if (playerPos.x >= 3.0f && playerPos.x <= 6.5f)
			{
				if (playerPos.z >= 0.0f && playerPos.z <= 5.0f)
				{
					//change orb state to attached if player touched it
					orbState = attached;
					touchedOrb = true;
				}

			}
		}
		//if we're at the start of the render loop again
		if (orbState == notConnected && touchedOrb == true)
		{
			orbState = attached;
		}

		//if attached
		if (orbState == attached && touchedOrb == true)
		{
			////check if player is far from orb
			//if (playerPos.x >= 7.1f && playerPos.z >= 5.1)
			//{
			//	//keep it true
			//	touchedOrb = true;
			//}

			//check if player touches collector
			if (playerPos.x >= 11.0f && playerPos.x <= 13.0f)
			{
				if (playerPos.z >= 7.5f && playerPos.z <= 9.5f)
				{
					orbState = connected;
					touchedOrb = false;
					connectedOrb = true;
				}
			}

		}
	}


	return playerPos;
}



GLfloat Game::ProcessPlayerSpeed(GLfloat speed, bool* keys)
{
	if (keys[GLFW_KEY_RIGHT_SHIFT] || shiftPressed == true)
	{
		speed = 6.0f;
		return speed;
		shiftPressed = true;
	}

	if (!keys[GLFW_KEY_RIGHT_SHIFT] || shiftPressed == false)
	{
		speed = 4.0f;
		return speed;
		shiftPressed = false;
	}

	return speed;
}

glm::vec3 Game::GetOrbPosition()
{
	OrbPosition = glm::vec3(5.0f, 0.0f, 2.5f);
	
		return OrbPosition;
}

glm::vec3 Game::GetCollectorPosition()
{
	CollectorPosition = glm::vec3(12.0f, 0.0f, 8.5f);
	return CollectorPosition;
}

bool Game::StartOrbConnection(bool start)
{
	if (start == true)
	{
		return(true);
	} 
	else
	{
		return(false);
	}
	return start;
}
