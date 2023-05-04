/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Texture.h
Purpose: For storing Entity datas
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 3 - Spatial Partitioning
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 11/04/2022
End Header --------------------------------------------------------*/
#pragma once
#include <string>
#include "Declare.h"

//will be component
class Shader;
class Texture {

	int w{};
	int h{};
	int comp{};
	unsigned char* image{};

	unsigned int textureID{};
	int textureNumber{};

	std::string filePath{};
	unsigned wrapMethod{ GL_REPEAT };

public:
	Texture(std::string fileName, unsigned WM = GL_REPEAT);
	Texture(std::string fileName, unsigned char* img, int width, int height, unsigned WM = GL_REPEAT);
	Texture(unsigned int texID, int width, int height, unsigned WM = GL_CLAMP_TO_EDGE);
	~Texture();

	void load();
	void unload();

	void setTextureNumber(int num);
	unsigned int getTextureID() { return textureID; };
	void bindTexture();
	void unbindTexture();
	void sendTextureDataUniform(std::shared_ptr<Shader> shader, const std::string uniformName);
};