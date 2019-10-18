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
#include "Texture.h"
#include "Light.h"
#include "Game.h"
#include "Model.h"


//#include "include/SOIL2.h"


 //window dimensions
const GLint WIDTH = 1920, HEIGHT = 1080;
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

//Textures
Texture stoneTexture;

//create light
Light mainLight;

//Model skull;

//create game class
Game GameClass;


//Vertex Shader
//note the input variables will change often but the uniform 
//variables will not change and will be applied to every vertex
static const char* vShader = "E:\\Education\\5TH YEAR\\FALL2019\\CPSC499\\GameDev\\OpenGLConsoleApp\\Shaders\\vertShader.txt";

//Fragment Shader
static const char* fShader = "E:\\Education\\5TH YEAR\\FALL2019\\CPSC499\\GameDev\\OpenGLConsoleApp\\Shaders\\fragShader.txt";

//calculate average normals
void calcAverageNormals(unsigned int * indices, unsigned int indiceCount, GLfloat * vertices, 
						unsigned int verticesCount, unsigned int vLength, unsigned int normalOffset)
{
	//for each triangle
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		//set variables to hold the 1st indices value of each triangle
		//multiplying by vLength will jump through multiple indices and vertice values
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;

		//define vertice edges/lines
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2+ 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		//calculate the normal of that surface
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		//add values into normal coordinates
		in0 += normalOffset;	in1 += normalOffset;		in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;

	}

	//for each vertices, replace with normalized normals
	for (size_t i = 0; i < verticesCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

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
		//x       y     z		 u     v		nx     ny    nz
		-1.0f, -1.0f, 0.0f,	   0.0f, 0.0f,		0.0f, 0.0f, 0.0f,//bottom left value
		0.0f, -1.0f, 1.0f,	   0.5f, 0.0f, 		0.0f, 0.0f, 0.0f,//bottom middle
		1.0f, -1.0f, 0.0f,	   1.0f, 0.0f, 		0.0f, 0.0f, 0.0f,//bottom right
		0.0f, 1.0f, 0.0f,	   0.5f, 1.0f, 		0.0f, 0.0f, 0.0f//top middle
	};

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);
	
	//declare and define mesh object from mesh class
	mesh* obj1 = new mesh();

	//create mesh
	obj1->CreateMesh(vertices, indices, 32, 12);
	//add obj1 to end of list
	meshList.push_back(obj1);

	//declare and define mesh object from mesh class
	mesh* obj2 = new mesh();

	//create mesh
	obj2->CreateMesh(vertices, indices, 32, 12);
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

	mainWindow = glWindow(WIDTH, HEIGHT);
	mainWindow.Initialise();

	CreateObject();
	createShaders();

	//initialize camera with:
	//startPosition, startUpVector, startYawAngle, startPitchAngle, startTurnspeed, startMoveSpeed
	camera = Camera(glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), -20.0f, 0.0f, 0.1f, 1.0f);

	stoneTexture = Texture("E:\\Education\\5TH YEAR\\FALL2019\\CPSC499\\GameDev\\OpenGLConsoleApp\\Textures\\stonePath.png");
	stoneTexture.LoadTextureA();

	//skull = Model();
	//skull.LoadModel("E:\\Education\\5TH YEAR\\FALL2019\\CPSC499\\GameDev\\OpenGLConsoleApp\\Models\\Skull_scene.obj");

	//initialize light
	mainLight = Light(1.0f, 1.0f, 1.0f, 0.3f, 2.0f, -1.0f, -2.0f, 1.0f);

	//Initialize game class
	//GameClass = Game(1.0f, glm::vec3(0.3f, 0.1f, 0.0f));


	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformAmbientIntensity = 0;
	GLuint uniformAmbientColor = 0;
	GLuint uniformDirection = 0;
	GLuint uniformDiffuseIntensity = 0;


	//create projection
	glm::mat4 projection = glm::perspective(45.0f, ((GLfloat)mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight()), 0.1f, 100.0f);

	//create player position
	glm::vec3 playerPos;// = glm::vec3(0.3f, 0.1f, 0.0f);

	//Loop until window closed
	//while the shouldClose variable is false, leave window open
	while(!mainWindow.getShouldClose()) 
	{
		
		GLfloat currentTime = glfwGetTime();	//get and set current time
		deltaTime = currentTime - lastTime;		//calculate delta time
		lastTime = currentTime;					//set last time to current time

		//get and handle user input events
		glfwPollEvents();
	
		//control camera depending on mode
		camera.keyControl(1, mainWindow.getKeys(), deltaTime);
		
		//note:mouse control will not work 100%, for now it's just initializing the rotation data
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());


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
		uniformAmbientColor = shaderList[0].GetAmbientColorLocation();
		uniformAmbientIntensity = shaderList[0].getAmbientIntensityLocation();
		uniformDirection = shaderList[0].GetDirectionLocation();
		uniformDiffuseIntensity = shaderList[0].GetDiffuseIntensityLocation();


		mainLight.UseLight(uniformAmbientIntensity, uniformAmbientColor, uniformDiffuseIntensity, uniformDirection);

		//4x4 matrix, automatically set to an identity matrix
		glm::mat4 model(1.0f);
		//take identity matrix and apply translation to it. the x value will change to triOffset
		//model = glm::translate(model, glm::vec3(0.3f, 0.1f, 0.0f));
		playerPos = GameClass.ProcessGameInput(mainWindow.getKeys(), deltaTime);
		model = glm::translate(model, playerPos);
		//rotate model by 45 degrees on the Z axis
		//model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));


		//set uniform value to desired variable (triOffset)void
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		stoneTexture.UseTexture();
		meshList[0]->RenderMesh();
		

		glm::mat4 model2(1.0f);
		model2 = glm::translate(model2, glm::vec3(0.3f, 0.1f, -2.5f));
		model2 = glm::scale(model2, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model2));
		stoneTexture.UseTexture();
		meshList[1]->RenderMesh();

		model2 = glm::mat4(1.0f);
		model2 = glm::translate(model2, glm::vec3(0.3f, 0.1f, -2.5f));
		//model2 = glm::scale(model2, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model2));
		//stoneTexture.UseTexture();
		//skull.renderModel();


		//clears shader
		glUseProgram(0);



		//most applications work by having two buffers, one that is currently
		//visibly and one that can't be seen and that is being drawn one. 
		//Swap buffers will swap the two to update it.
		mainWindow.swapBuffers();

	}


	return 0;
}