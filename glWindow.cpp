#include "glWindow.h"


glWindow::glWindow()
{
	width = 800;
	height = 600;
	
	//set all keys to 0
	for (size_t i = 0; i< 1024; i++)
	{
		keys[i] = 0;
	}
}

glWindow::glWindow(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	
	//set all keys to 0
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}

int glWindow::Initialise()
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

	//create the window
	mainWindow = glfwCreateWindow(width, height, "Test Window", NULL, NULL);

	//check if window is being constructed properly
	if (!mainWindow)
	{
		printf("GLFW window creation failed");
		glfwTerminate();
		return 1;
	}

	//get buffer size information (buffer = actual viewport being drawn)
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//handle key and mouse input
	createCallbacks();
	//make mouse invisible in the window
	//glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	//set the context for GLEW to use, let GLEW know that the window 
	//we created is the one the OpenGL should draw to
	glfwMakeContextCurrent(mainWindow);

	//allow modern extensions features 
	glewExperimental = GL_TRUE;

	//initialize glew
	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		printf("Error: %s", glewGetErrorString(error));
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	//gl depth test to determine what is in front and behind
	glEnable(GL_DEPTH_TEST);

	//create viewport with size
	glViewport(0, 0, bufferWidth, bufferHeight);
	
	//function to assign a user pointer to this window to access specific functions
	glfwSetWindowUserPointer(mainWindow, this);
}

GLfloat glWindow::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat glWindow::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

glWindow::~glWindow()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}

void glWindow::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, handleKeys);
	glfwSetCursorPosCallback(mainWindow, handleMouse);
}

void glWindow::handleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
	//get the user point for the main window and assign it to theWindow variable
	glWindow* theWindow = static_cast<glWindow*>(glfwGetWindowUserPointer(window));

	//if ESCAPE is pressed, 
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		//close window
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	//check if keys are valid
	if (key >= 0 && key < 1024)
	{
		
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			//printf("pressed: %d\n", key);
		}

		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			//printf("released: %d\n", key);
		}
	}
}

void glWindow::handleMouse(GLFWwindow* window, double xPos, double yPos)
{
	//get the user point for the main window and assign it to theWindow variable
	glWindow* theWindow = static_cast<glWindow*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	//set changed value for x
	theWindow->xChange = xPos - theWindow->lastX;
	//set changed value for y, swapped to avoid inverted movements
	theWindow->yChange = theWindow->lastY - yPos;

	//set last positions to be current positions
	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
	//printf("x:%.6f, y:%.6f\n", theWindow->xChange, theWindow->yChange);
}
