#include "Texture.h"


Texture::Texture()
{
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = "";
}


Texture::Texture(const char* fileLoc)
{
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = fileLoc;
}


bool Texture::LoadTextureA()
{
	//storing texture data into char array
	unsigned char* textureData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);

	//check if data is valid
	if (!textureData)
	{
		printf("Failed to find: %s\n", fileLocation);
		return false;
	}

	glGenTextures(1, &textureID); //generate 1 texture with the TextureID
	glBindTexture(GL_TEXTURE_2D, textureID); //bind texture to ID

	//set texture parameters
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//setup texture image data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
	glGenerateMipmap(GL_TEXTURE_2D);

	
	//unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);

	//free up stbi image data
	stbi_image_free(textureData);

	return true;

}

bool Texture::LoadTexture()
{
	//storing texture data into char array
	unsigned char* textureData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);

	//check if data is valid
	if (!textureData)
	{
		printf("Failed to find: %s\n", fileLocation);
		return false;
	}

	glGenTextures(1, &textureID); //generate 1 texture with the TextureID
	glBindTexture(GL_TEXTURE_2D, textureID); //bind texture to ID

	//set texture parameters
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//setup texture image data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
	glGenerateMipmap(GL_TEXTURE_2D);


	//unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);

	//free up stbi image data
	stbi_image_free(textureData);

	return true;
}

void Texture::UseTexture()
{
	//creates a texture unit, will allow you to bind and activate multiple textures
	//allows for multiple textures on one object
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

}

void Texture::ClearTexture()
{
	glDeleteTextures(1, &textureID);
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = "";
}

Texture::~Texture()
{
}
