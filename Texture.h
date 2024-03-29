#pragma once

#include <GL/glew.h>

#include "SOIL2/include/stb_image.h"

class Texture
{
public:
	Texture();					//default constructor
	Texture(const char* fileLoc);		//main constructor

	bool LoadTexture();			//load texture data (RGB)
	bool LoadTextureA(bool blendOn);		//loads texture with alpha data (RGBA)
	void UseTexture(bool blendOn);			//use texture data
	void BlendTexture();
	void ClearTexture();		//clear texture data

	~Texture();					//default deconstructor

private:
	GLuint textureID;			//ID for texture
	int width, height, bitDepth;//texture image data
	const char* fileLocation;			//char array of file location
};

