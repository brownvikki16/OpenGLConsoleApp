#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "mesh.h"
#include "Shader.h"
#include "glWindow.h"
#include "Camera.h"


 //window dimensions
const GLint WIDTH = 800, HEIGHT = 600;
//convert to Radians
const float toRadians = 3.14159265f / 180.0f;

//create window
glWindow mainWindow;

//delaTime variables
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

//vector of Mesh class
std::vector<mesh*>meshList;
std::vector<Shader>shaderList;

//create camera
Camera camera;

bool direction = true;
float triOffset = 0.0f;   
float triMaxOffset = 0.7f;  
float triIncrement = 0.0005f;  

float curAngle = 0.0f;

bool sizeDirection = true;
float curSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;

//Vertex Shader
//note the input variables will change often but the uniform 
//variables will not change and will be applied to every vertex
static const char* vShader = "E:\\Education\\5TH YEAR\\FALL2019\\CPSC499\\GameDev\\OpenGLConsoleApp\\Shaders\\vertShader.txt";

//Fragment Shader
static const char* fShader = "E:\\Education\\5TH YEAR\\FALL2019\\CPSC499\\GameDev\\OpenGLConsoleApp\\Shaders\\fragShader.txt";


void CreateObject()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	//array of triangle vertex points
	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f, //0
		0.0f, -1.0f, 1.0f, //1
		1.0f, -1.0f, 0.0f, //2
		0.0f, 1.0f, 0.0f, //3
	};
	
	//declare and define mesh object from mesh class
	mesh* obj1 = new mesh();

	//create mesh
	obj1->CreateMesh(vertices, indices, 12, 12);
	//add obj1 to end of list
	meshList.push_back(obj1);

	//declare and define mesh object from mesh class
	mesh* obj2 = new mesh();

	//create mesh
	obj2->CreateMesh(vertices, indices, 12, 12);
	//add obj2 to end of list
	meshList.push_back(obj2);
}


void createShaders()
{
	Shader* shader1 = new Shader();

	shader1->createFromFiles(vShader, fShader);

	shaderList.push_back(*shader1);
}


int main()
{
	mainWindow = glWindow(800, 600);
	mainWindow.Initialise();

	CreateObject();
	createShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, 0.0f, 0.1f, 1.0f);

	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;


	//create projection
	glm::mat4 projection = glm::perspective(45.0f, ((GLfloat)mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight()), 0.1f, 100.0f);


	//Loop until window closed
	//while the shouldClose variable is false, leave window open
	while(!mainWindow.getShouldClose()) 
	{
		
		GLfloat currentTime = glfwGetTime();	//get and set current time
		deltaTime = currentTime - lastTime;		//calculate delta time
		lastTime = currentTime;					//set last time to current time

		//get and handle user input events
		glfwPollEvents();

		camera.keyControl(mainWindow.getKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		/*
		//if triangle is moving positive, add increment value to offset
		if (direction)
		{
			triOffset += triIncrement;
		}
		else  //opposite
		{
			triOffset -= triIncrement;
		}
		if (abs(triOffset) >= triMaxOffset)
		{
			direction = !direction;  //swap bool value
		}

		curAngle += 0.01f;
		if (curAngle >=360)
		{
			curAngle -= 360;
		}

		if (sizeDirection)
		{
			curSize += 0.001f;
		}
		else
		{
			curSize -= 0.001f;
		}

		if (curSize>= maxSize || curSize <= minSize)
		{
			sizeDirection = !sizeDirection;
		}
		*/

		//clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //clears out all drawn things
		//each pixel has a lot of data in it. this line of code below
		//is clearing the color buffer bit data
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

		//actual drawing, will use shader with the current shader ID
		shaderList[0].UseShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectionLocation();
		uniformView = shaderList[0].getViewLocation();


		//4x4 matrix, automatically set to an identity matrix
		glm::mat4 model(1.0f);
		//take identity matrix and apply translation to it. the x value will change to triOffset
		//model = glm::translate(model, glm::vec3(0.0f, 0.5f, -2.5f));
		//rotate model by 45 degrees on the Z axis
		//model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));


		//set uniform value to desired variable (triOffset)void
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

		meshList[0]->RenderMesh();
		

		glm::mat4 model2(1.0f);
		model2 = glm::translate(model2, glm::vec3(0.3f, 0.1f, -2.5f));
		model2 = glm::scale(model2, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model2));
		meshList[1]->RenderMesh();

		//clears shader
		glUseProgram(0);



		//most applications work by having two buffers, one that is currently
		//visibly and one that can't be seen and that is being drawn one. 
		//Swap buffers will swap the two to update it.
		mainWindow.swapBuffers();

	}




	return 0;
}