#include <stdio.h>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

 //window dimensions
const GLint WIDTH = 800, HEIGHT = 600;

//variables
GLuint VAO, VBO, shader;

//Vertex Shader
static const char* vShader = "								 \n\
#version 330												 \n\
															 \n\
layout (location = 0) in vec3 pos;							 \n\
															 \n\
void main()													 \n\
{															 \n\
   gl_Position = vec4(0.4 * pos.x, 0.4 * pos.y, pos.z, 1.0); \n\
}";


//Fragment Shader
static const char* fShader =
"#version 330                                               \n\
out vec4 color;                                             \n\
                                                            \n\
void main()                                                 \n\
{                                                           \n\
  color = vec4(1.0, 0.0, 0.0, 1.0);                         \n\
}";                                                          


void CreateTriangle()
{
	//array of triangle vertex points
	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	//creating VAO ID (how many, where the ID is)
	glGenVertexArrays(1, &VAO);
	//bind VAO with that ID
	glBindVertexArray(VAO);

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

	//Unbind VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Unbind VAO
	glBindVertexArray(0);
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

		//clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //clears out all drawn things
		//each pixel has a lot of data in it. this line of code below
		//is clearing the color buffer bit data
		glClear(GL_COLOR_BUFFER_BIT); 

		//actual drawing, will use shader with the current shader ID
		glUseProgram(shader);
		
		//bind vertex array
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
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