#pragma once

#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>


class glWindow
{
public:
	glWindow();

	glWindow(GLint windowWidth, GLint windowHeight);

	int Initialise();

	//for projection calculation
	int getBufferWidth() { return bufferWidth; }
	int getBufferHeight() { return bufferHeight; }

	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }

	bool* getKeys() { return keys; }

	GLfloat getXChange();
	GLfloat getYChange();

	void swapBuffers() { glfwSwapBuffers(mainWindow); }

	~glWindow();


private:
	GLFWwindow* mainWindow;

	GLint width, height;
	GLint bufferWidth, bufferHeight;
	
	//array of bools for each 1024 keys 
	bool keys[1024];

	//mouse movement variables
	GLfloat lastX;		//set the mouse's last x coordinate
	GLfloat lastY;		//set the mouse's last y coordinate
	GLfloat xChange;	//set the mouse's change in x
	GLfloat yChange;	//set the mouse's change in y
	bool mouseFirstMoved; //check if first mouse movement
	

	//create callback functions
	void createCallbacks();

	//function to be called when keys are pressed, a callback function
	//set to static because glfw needs a static function to call back to
	static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);

	//handles mouse movement
	static void handleMouse(GLFWwindow* window, double xPos, double yPos);
};

