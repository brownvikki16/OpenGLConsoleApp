#pragma once

#include "GL/glew.h"

class mesh
{
public:
	mesh();

	//creates mesh
	//takes vertices array, indices array, 
	void CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);
	
	//draw mesh to screen
	void RenderMesh();
	
	//clear up mesh data
	void ClearMesh();

	~mesh();

private:
	GLuint VAO, VBO, IBO;
	GLsizei indexCount;

};

