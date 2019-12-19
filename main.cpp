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
#include "DirectionalLight.h"
#include "Game.h"
#include "CommonValues.h"
#include "PointLight.h"
#include "Material.h"
//#include "Model.h"


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

//camera mode
int camMode;

//Textures
//Texture lightTexture;
Texture floorTex;
Texture floorTexRed;
Texture bookcaseTex_01;
Texture bookcaseTex_01_red;
Texture plainWhiteTex;  //for player
Texture plainRedTex;
Texture yellowTexLight; //for orbs
Texture yellowTexDark;  //for orbs
Texture purpleTex;	
Texture turquioseTex;	//for collector

//create light
DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];

//create material
Material shinyMat;
Material dullMat;

//Model skull;

//create game class
Game GameClass;


//Vertex Shader
//note the input variables will change often but the uniform 
//variables will not change and will be applied to every vertex
static const char* vShader = "E:\\Education\\5TH YEAR\\FALL2019\\CPSC499\\GameDev\\OpenGLConsoleApp\\Shaders\\vertShader.txt";

//Fragment Shader
static const char* fShader = "E:\\Education\\5TH YEAR\\FALL2019\\CPSC499\\GameDev\\OpenGLConsoleApp\\Shaders\\fragShader.txt";

//Second Shaders
static const char* vShader2 = "E:\\Education\\5TH YEAR\\FALL2019\\CPSC499\\GameDev\\OpenGLConsoleApp\\Shaders\\vertShader2.txt";

//Fragment Shader
static const char* fShader2 = "E:\\Education\\5TH YEAR\\FALL2019\\CPSC499\\GameDev\\OpenGLConsoleApp\\Shaders\\fragShader2.txt";


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

	unsigned int indices_floor[] = {
		0, 1, 2,
		0, 2, 3

	};
	//array of triangle vertex points
	GLfloat vertices[] = {
		//x       y     z		 u     v		nx     ny    nz
		-1.0f, -1.0f, -0.6f,	   0.0f, 0.0f,		0.0f, 0.0f, 0.0f,//bottom left value
		0.0f, -1.0f, 1.0f,	   0.5f, 0.0f, 		0.0f, 0.0f, 0.0f,//bottom middle
		1.0f, -1.0f, -0.6f,	   1.0f, 0.0f, 		0.0f, 0.0f, 0.0f,//bottom right
		0.0f, 1.0f, 0.0f,	   0.5f, 1.0f, 		0.0f, 0.0f, 0.0f//top middle
	};

	GLfloat verts_floor[] = {
		//x       y     z		 u     v		nx     ny    nz
		0.0f,  0.0f, 0.0f,	   0.0f, 0.0f,		0.0f, 0.0f, 0.0f,//bottom left value
		1.0f, 0.0f, 0.0f,	   1.0f, 0.0f, 		0.0f, 0.0f, 0.0f,//bottom middle
		1.0f, 0.0f, 1.0f,	   1.0f, 1.0f, 		0.0f, 0.0f, 0.0f,//bottom right
		0.0f, 0.0f, 1.0f,	   0.0f, 1.0f, 		0.0f, 0.0f, 0.0f//top middle
	};

	GLfloat verts_wall[] = {
		//x       y     z		 u     v		nx     ny    nz
		0.0f,  0.0f, 0.0f,	   0.0f, 0.0f,		0.0f, 0.0f, 0.0f,//bottom left value
		1.0f, 0.0f, 0.0f,	   1.0f, 0.0f, 		0.0f, 0.0f, 0.0f,//bottom middle
		1.0f, 1.0f, 0.0f,	   1.0f, 1.0f, 		0.0f, 0.0f, 0.0f,//bottom right
		0.0f, 1.0f, 0.0f,	   0.0f, 1.0f, 		0.0f, 0.0f, 0.0f//top middle
	};

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);
	
	//declare and define mesh object from mesh class
	mesh* playerObj = new mesh();
	//create mesh
	playerObj->CreateMesh(vertices, indices, 32, 12);
	//add obj1 to end of list
	meshList.push_back(playerObj);

	//declare and define mesh object from mesh class
	mesh* obj2 = new mesh();
	//create mesh
	obj2->CreateMesh(vertices, indices, 32, 12);
	//add obj2 to end of list
	meshList.push_back(obj2);


	//create floor object
	mesh* floorObj = new mesh();
	floorObj->CreateMesh(verts_floor, indices_floor, 32, 6);
	meshList.push_back(floorObj);

	//create wall object
	mesh* wallObj01 = new mesh();
	wallObj01->CreateMesh(verts_wall, indices_floor, 32, 6);
	meshList.push_back(wallObj01);

	//create camera lens
	mesh* camLens = new mesh();
	camLens->CreateMesh(verts_floor, indices_floor, 32, 6);
	meshList.push_back(camLens);

}


void createShaders()
{
	Shader* shader1 = new Shader();
	shader1->createFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);


	//Shader* shader2 = new Shader();
	//shader2->createFromFiles(vShader2, fShader2);
	//shaderList.push_back(*shader2);

}


int main()
{


	mainWindow = glWindow(WIDTH, HEIGHT);
	mainWindow.Initialise();

	CreateObject();
	createShaders();

	//initialize camera with:
	//startPosition, startUpVector, startYawAngle, startPitchAngle, startTurnspeed, startMoveSpeed, camera Boundary
	camera = Camera(glm::vec3(0.0f, 15.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), -20.0f, 0.0f, 0.1f, 1.0f, 2.0f);
	camMode = 1; //0 = manual control; 1 = player control

	floorTex = Texture("E:\\Education\\5TH YEAR\\FALL2019\\CPSC499\\GameDev\\OpenGLConsoleApp\\Textures\\floorTile.png");
	floorTex.LoadTextureA(false);
	floorTexRed = Texture("E:\\Education\\5TH YEAR\\FALL2019\\CPSC499\\GameDev\\OpenGLConsoleApp\\Textures\\floorTile_red.jpg");
	floorTexRed.LoadTexture();
	plainWhiteTex = Texture("E:\\Education\\5TH YEAR\\FALL2019\\CPSC499\\GameDev\\OpenGLConsoleApp\\Textures\\whiteTex.png");
	plainWhiteTex.LoadTexture();
	bookcaseTex_01 = Texture("E:\\Education\\5TH YEAR\\FALL2019\\CPSC499\\GameDev\\OpenGLConsoleApp\\Textures\\bookcase01.jpg");
	bookcaseTex_01.LoadTexture();
	bookcaseTex_01_red = Texture("E:\\Education\\5TH YEAR\\FALL2019\\CPSC499\\GameDev\\OpenGLConsoleApp\\Textures\\bookcase01_red.jpg");
	bookcaseTex_01_red.LoadTexture();
	plainRedTex = Texture("E:\\Education\\5TH YEAR\\FALL2019\\CPSC499\\GameDev\\OpenGLConsoleApp\\Textures\\redTex.png");
	plainRedTex.LoadTextureA(false);
	yellowTexLight = Texture("E:\\Education\\5TH YEAR\\FALL2019\\CPSC499\\GameDev\\OpenGLConsoleApp\\Textures\\yellowTexLight.png");
	yellowTexLight.LoadTextureA(false);
	yellowTexDark = Texture("E:\\Education\\5TH YEAR\\FALL2019\\CPSC499\\GameDev\\OpenGLConsoleApp\\Textures\\yellowTexDark.png");
	yellowTexDark.LoadTextureA(false);
	turquioseTex = Texture("E:\\Education\\5TH YEAR\\FALL2019\\CPSC499\\GameDev\\OpenGLConsoleApp\\Textures\\turquioseTex.png");
	turquioseTex.LoadTextureA(false);
	purpleTex = Texture("E:\\Education\\5TH YEAR\\FALL2019\\CPSC499\\GameDev\\OpenGLConsoleApp\\Textures\\purpleTex.png");
	purpleTex.LoadTextureA(false);
	


	//set material values
	shinyMat = Material(1.0f, 32);
	dullMat = Material(0.3f, 4);

	//skull = Model();
	//skull.LoadModel("E:\\Education\\5TH YEAR\\FALL2019\\CPSC499\\GameDev\\OpenGLConsoleApp\\Models\\Skull_scene.obj");

	//initialize light
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f, //color
								0.8f, 0.2f, //intensities
								0.0f, -1.0f, 0.0f); //direction
	unsigned int pointLightCount = 0;

	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,	//color 
								0.1f, 0.1f,			//intensities
								5.0f, 0.3f, 3.0f,	//position
								0.3f, 0.2f, 0.1f);	//constant, linear, exp
	pointLightCount++;

	//Initialize game class
	GameClass = Game(camMode);

	//
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformEyePosition = 0;
	GLuint uniformSpecularIntensity = 0;
	GLuint uniformShininess = 0;

	//create projection
	glm::mat4 projection = glm::perspective(45.0f, ((GLfloat)mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight()), 0.1f, 100.0f);

	//create player position
	glm::vec3 playerPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	//create player speed
	GLfloat playerSpeed = 4.0f;
	//create playerStats
	bool playerTouchedOrb = false;
	bool playerConnectedOrb = false;

	//create lightObj pos
	glm::vec3 OrbPos1 = GameClass.GetOrbPosition();

	//create Collector Position
	glm::vec3 collectorPos = GameClass.GetCollectorPosition();

	//Loop until window closed
	//while the shouldClose variable is false, leave window open
	while(!mainWindow.getShouldClose()) 
	{
		
		
		GLfloat currentTime = glfwGetTime();	//get and set current time
		deltaTime = currentTime - lastTime;		//calculate delta time
		lastTime = currentTime;					//set last time to current time
		GLfloat frequency = 5.0f;
		GLfloat sinFactor = sin(currentTime*frequency);
		
		//printf("current Time: %f\n", currentTime);

		//get and handle user input events
		glfwPollEvents();
	
		//set player position
		playerPosition = GameClass.ProcessPositionInput(playerPosition, playerSpeed, mainWindow.getKeys(), deltaTime);
		//set player speed
		playerSpeed = GameClass.ProcessPlayerSpeed(playerSpeed, mainWindow.getKeys());
		//set playerStatus
		playerTouchedOrb = GameClass.touchedOrb;
		playerConnectedOrb = GameClass.connectedOrb;


		//control camera depending on mode
		camera.keyControl(playerPosition, camMode, mainWindow.getKeys(), deltaTime);
		
		//note:mouse control will not work 100%, for now it's just initializing the rotation data
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());


		//clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //clears out all drawn things
		//each pixel has a lot of data in it. this line of code below
		//is clearing the color buffer bit data
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
		//glLoadIdentity();


		//actual drawing, will use shader with the current shader ID
		shaderList[0].UseShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectionLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformEyePosition = shaderList[0].GetDirectionLocation();
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();
		shaderList[0].setDirectionalLight(&mainLight);
		//shaderList[0].setPointLights(pointLights, pointLightCount);
	
	
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		////HANDLE CAM LENS
		////camLensPos = camera.getCameraPosition();
		////camLensPos += glm::vec3(0.0f, -3.0f, 0.0f);
		//glm::mat4 camLensModel(1.0f);
		//camLensModel = glm::translate(camLensModel, glm::vec3(0.0f, 8.0f, 0.0f));
		//camLensModel = glm::scale(camLensModel, glm::vec3(20.0f, 0.0f, 20.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(camLensModel));
		//plainRedTex.UseTexture(false);
		//meshList[4]->RenderMesh();


		//HANDLE PLAYER MODEL
		//4x4 matrix, automatically set to an identity matrix
		glm::mat4 model(1.0f);
		//take identity matrix and apply translation to it. the x value will change to triOffset
		//model = glm::translate(model, glm::vec3(0.3f, 0.1f, 0.0f));
		model = glm::translate(model, playerPosition);
		//rotate model by 45 degrees on the Z axis
		//model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		//set uniform value to desired variable (triOffset)void
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		plainWhiteTex.UseTexture(false);
		meshList[0]->RenderMesh();
		shinyMat.UseMaterial(uniformSpecularIntensity, uniformShininess);
		
		

		//HANDLE ORB 1
		glm::mat4 Orb1(1.0f);
		Orb1 = glm::translate(Orb1, OrbPos1);
		Orb1 = glm::scale(Orb1, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(Orb1));
		//if orb is connected to collector
		if (playerConnectedOrb)
		{
			yellowTexDark.UseTexture(false);
		}
		else
		{
			yellowTexLight.UseTexture(false);
		}
		meshList[1]->RenderMesh();
		dullMat.UseMaterial(uniformSpecularIntensity, uniformShininess);

		//HANDLE COLLECTOR
		glm::mat4 collector(1.0f);
		collector = glm::translate(collector, collectorPos);
		collector = glm::scale(collector, glm::vec3(3.0f, 5.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(collector));
		if (playerConnectedOrb)
		{
			purpleTex.UseTexture(false);
		}
		else
		{
			turquioseTex.UseTexture(false);
		}
		meshList[1]->RenderMesh();
	

		//HANDLE FLOOR MODEL
		glm::mat4 floorModel(1.0f);
		floorModel = glm::translate(floorModel, glm::vec3(20.0f, -2.0f, 0.0f));
		floorModel = glm::rotate(floorModel, 90 * toRadians, glm::vec3(0.0f, -1.0f, 0.0f));
		if (currentTime > 3.0f && !playerConnectedOrb)
		{
			floorModel = glm::rotate(floorModel, (-5 * sinFactor) * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		}
		floorModel = glm::scale(floorModel, glm::vec3(20.0f, 0.0f, 20.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(floorModel));
		if (currentTime > 3.0f && !playerConnectedOrb)
		{
			floorTexRed.UseTexture(false);
		}
		else
		{
			floorTex.UseTexture(false);
		}
		//floorTex.UseTexture(false);
		meshList[2]->RenderMesh();

	
		//HANDLE WALL MODELS
		glm::mat4 wallModel01(1.0f);
		wallModel01 = glm::translate(wallModel01, glm::vec3(0.0f, -2.0f, 0.0f));
		wallModel01 = glm::rotate(wallModel01, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		if (currentTime > 3.0f && !playerConnectedOrb)
		{
			wallModel01 = glm::rotate(wallModel01, (5*sinFactor) * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		}
		wallModel01 = glm::scale(wallModel01, glm::vec3(-20.0f, 10.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(wallModel01));
		if (currentTime > 3.0f && !playerConnectedOrb)
		{
			bookcaseTex_01_red.UseTexture(false);
		}
		else
		{
			bookcaseTex_01.UseTexture(false);
		}
		meshList[3]->RenderMesh();


		glm::mat4 wallModel02(1.0f);
		wallModel02 = glm::translate(wallModel02, glm::vec3(20.0f, -2.0f, 0.0f));
		wallModel02 = glm::rotate(wallModel02, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		if (currentTime > 3.0f && !playerConnectedOrb)
		{
			wallModel02 = glm::rotate(wallModel02, (5 * sinFactor) * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		}
		wallModel02 = glm::scale(wallModel02, glm::vec3(-20.0f, 10.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(wallModel02));
		if (currentTime > 3.0f && !playerConnectedOrb)
		{
			bookcaseTex_01_red.UseTexture(false);
		}
		else
		{
			bookcaseTex_01.UseTexture(false);
		}
		meshList[3]->RenderMesh();

		glm::mat4 wallModel03(1.0f);
		wallModel03 = glm::translate(wallModel03, glm::vec3(20.0f, -2.0f, 0.0f));
		if (currentTime > 3.0f && !playerConnectedOrb)
		{
			wallModel03 = glm::rotate(wallModel03, (5 * sinFactor) * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		}
		wallModel03 = glm::scale(wallModel03, glm::vec3(-20.0f, 10.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(wallModel03));
		if (currentTime > 3.0f && !playerConnectedOrb)
		{
			bookcaseTex_01_red.UseTexture(false);
		}
		else
		{
			bookcaseTex_01.UseTexture(false);
		}
		meshList[3]->RenderMesh();

		glm::mat4 wallModel04(1.0f);
		wallModel04 = glm::translate(wallModel04, glm::vec3(20.0f, -2.0f, 20.0f));
		//wallModel04 = glm::rotate(wallModel04, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		if (currentTime > 3.0f && !playerConnectedOrb)
		{
			wallModel04 = glm::rotate(wallModel04, (5 * sinFactor) * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		}
		wallModel04 = glm::scale(wallModel04, glm::vec3(-20.0f, 10.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(wallModel04));
		if (currentTime > 3.0f && !playerConnectedOrb)
		{
			bookcaseTex_01_red.UseTexture(false);
		}
		else
		{
			bookcaseTex_01.UseTexture(false);
		}
		meshList[3]->RenderMesh();


		if (playerTouchedOrb)
		{
			//printf("orb touched");
			//HANDLE MIDDLE POINTER(S)
			GLfloat middlePosX = (playerPosition.x + OrbPos1.x) / 2;
			GLfloat middlePosY = (playerPosition.y + OrbPos1.y) / 2;
			GLfloat middlePosZ = (playerPosition.z + OrbPos1.z) / 2;
			glm::vec3 middleVec = glm::vec3(middlePosX, middlePosY, middlePosZ);

			glm::mat4 middlePointer(1.0f);
			middlePointer = glm::translate(middlePointer, middleVec);
			//middlePointer = glm::rotate(middlePointer, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			//middlePointer = glm::lookAt(middleVec, playerPosition, glm::vec3(0.0f, 1.0f, 0.0f));
			middlePointer = glm::scale(middlePointer, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(middlePointer));
			yellowTexLight.UseTexture(false);
			meshList[1]->RenderMesh();
		}

		

		//clears shader
		glUseProgram(0);


		//most applications work by having two buffers, one that is currently
		//visibly and one that can't be seen and that is being drawn one. 
		//Swap buffers will swap the two to update it.
		mainWindow.swapBuffers();



	}


	return 0;
}