#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include "GL/glew.h"

class Shader
{
public:
	Shader();

	void createFromString(const char* vertexCode, const char* fragmentCode);
	void createFromFiles(const char* vertexLocation, const char* fragmentLocation);
	
	std::string readFile(const char* fileLocation);
	
	GLuint getProjectionLocation();
	GLuint getModelLocation();
	GLuint getViewLocation();
	GLuint getAmbientIntensityLocation();
	GLuint GetAmbientColorLocation();
	GLuint GetDiffuseIntensityLocation();
	GLuint GetDirectionLocation();
	
	void UseShader();
	void clearShader();

	~Shader();

private:
	GLuint shaderID, uniformProjection, uniformModel, uniformView, 
		uniformAmbientIntensity, uniformAmbientColor, uniformDiffuseIntensity, uniformDirection;

	void compileShader(const char* vertexCode, const char* fragmentCode);
	//pass string into compilleShader
	void addShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);

};

