#include "mesh.h"

mesh::mesh()
{
	VAO = 0;
	VBO = 0;
	IBO = 0;
	indexCount = 0;
}

void mesh::CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices)
{
	
	indexCount = numOfIndices;
	
	glGenVertexArrays(1, &VAO);		//creating VAO ID (how many, where the ID is)
	glBindVertexArray(VAO);			//bind VAO with that ID

	
	glGenBuffers(1, &IBO);			//create IBO buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);	   //bind IBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0])*numOfIndices, indices, GL_STATIC_DRAW);  //define buffer data

	glGenBuffers(1, &VBO);			//create a VBO ID (how many, where the ID is)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);		//bind VBO with that ID (type of VBO, where the ID is)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0])*numOfVertices, vertices, GL_STATIC_DRAW);  //define buffer data

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, 0);  //define attribute pointer properties or formatting
	glEnableVertexAttribArray(0);		//enable vertex array
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 3)); //define attributes for texture data
	glEnableVertexAttribArray(1);		//enable vertex array
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 5));  //define attributes for normal data
	glEnableVertexAttribArray(2);
	
	//Unbind VAO
	glBindVertexArray(0);

	//Unbind VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//unbind IBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void mesh::RenderMesh()
{

	if (indexCount == 0)
	{

	} 
	else
	{
		//bind vertex array VAO
		glBindVertexArray(VAO);
		//bind IBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

		//unbind IBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		//unbind vertex array
		glBindVertexArray(0);

		//unbind shader
		//glUseProgram(0);
	}
}


//clears data from graphics card to make space
void mesh::ClearMesh()
{
	if (IBO != 0)
	{
		glDeleteBuffers(1, &IBO);
		IBO = 0;
	}

	if (VBO != 0)
	{
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}

	if (VAO != 0)
	{
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}

	indexCount = 0;
}

mesh::~mesh()
{
	ClearMesh();
}


