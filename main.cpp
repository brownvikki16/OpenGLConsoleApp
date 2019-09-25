#include <stdio.h>
#include <string.h>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

 //window dimensions
const GLint WIDTH = 800, HEIGHT = 600;
//convert to Radians
const float toRadians = 3.14159265f / 180.0f;


//variables
GLuint VAO, VBO, IBO, shader, uniformModel;

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
static const char* vShader = 
"								\n\
#version 330												\n\
															\n\
layout (location = 0) in vec3 pos;							\n\
															\n\
out vec4 vertexColor;										\n\
															\n\
uniform mat4 model;											\n\
															\n\
void main()													\n\
{															\n\
   gl_Position = model * vec4(pos, 1.0);					\n\
   vertexColor = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);		\n\
}";


//Fragment Shader
static const char* fShader =
"#version 330                                               \n\
in vec4 vertexColor;										\n\
out vec4 color;                                             \n\
                                                            \n\
void main()                                                 \n\
{                                                           \n\
  color = vertexColor;				                        \n\
}";                                                          


void CreateTriangle()
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

	//creating VAO ID (how many, where the ID is)
	glGenVertexArrays(1, &VAO);
	//bind VAO with that ID
	glBindVertexArray(VAO);

	//create IBO buffer
	glGenBuffers(1, &IBO);
	//bind IBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	//define buffer data
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//create a VBO ID (how many, where the ID is)
	glGenBuffers(1, &VBO);
	//bind VBO with that ID (type of VBO, where the ID is)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//define attribute pointer properties or formatting
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//enable vertex array
	glEnableVertexAttribArray(0);

	//Unbind VAO
	glBindVertexArray(0);

	//Unbind VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//unbind IBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

//
void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	//creates empty shader of defined type
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
		printf("Error compiling the %d shader: '%s'\n", shaderType, eLog);
		return;
	}

	glAttachShader(theProgram, theShader);
}


void CompileShaders() {
	//create the shader program and give it an ID
	shader = glCreateProgram();

	//make sure shader was created correctly
	if (!shader) {
		printf("Error creating shader program\n");
		return;
	}

	//
	AddShader(shader, vShader, GL_VERTEX_SHADER);
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);

	//getting error code information
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}

	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return;
	}

	uniformModel = glGetUniformLocation(shader, "model");

}




int main()
{
	//initialize GLFW
	//if it fails, send message and terminate glfw
	if (!glfwInit())
	{
		printf("GLFW initialization failed");
		glfwTerminate();
		return 1;
	}

	//setup GLFW window properties
	//set the version of the OpenGl window to version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//core profile = no backward compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//forward compat =  allows forward compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//create window
	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "test", NULL, NULL);

	//check if window is being constructed properly
	if (!mainWindow)
	{
		printf("GLFW window creation failed");
		glfwTerminate();
		return 1;
	}

	//get buffer size information (buffer = actual viewport being drawn)
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//set the context for GLEW to use, let GLEW know that the window 
	//we created is the one the OpenGL should draw to
	glfwMakeContextCurrent(mainWindow);

	//allow modern extensions features 
	glewExperimental = GL_TRUE;

	//initialize glew
	if (glewInit() != GLEW_OK)
	{
		printf("GLEW initialization failed");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	//gl depth test to determine what is in front and behind
	glEnable(GL_DEPTH_TEST);

	//create viewport with size
	glViewport(0, 0, bufferWidth, bufferHeight);

	CreateTriangle();
	CompileShaders();

	//Loop until window closed
	//while the shouldClose variable is false, leave window open
	while(!glfwWindowShouldClose(mainWindow)) 
	{
		//get and handle user input events
		glfwPollEvents();

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

		//clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //clears out all drawn things
		//each pixel has a lot of data in it. this line of code below
		//is clearing the color buffer bit data
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

		//actual drawing, will use shader with the current shader ID
		glUseProgram(shader);

		//4x4 matrix, automatically set to an identity matrix
		glm::mat4 model(1.0f);
		//take identity matrix and apply translation to it. the x value will change to triOffset
		//model = glm::translate(model, glm::vec3(triOffset, 0.0f, 0.0f));
		//rotate model by 45 degrees on the Z axis
		model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

		//set uniform value to desired variable (triOffset)void
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		
		//bind vertex array
		glBindVertexArray(VAO);
		//bind IBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
		
		//unbind IBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		
		//unbind vertex array
		glBindVertexArray(0);
		
		//unbind shader
		glUseProgram(0);



		//most applications work by having two buffers, one that is currently
		//visibly and one that can't be seen and that is being drawn one. 
		//Swap buffers will swap the two to update it.
		glfwSwapBuffers(mainWindow);

	}




	return 0;
}