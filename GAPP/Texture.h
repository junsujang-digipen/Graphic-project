/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Texture.h
Purpose: For storing Entity datas
Language: c++
Platform: x64
Project: junsu.jang, CS300, Assignment 2 - Implementing Phong Illumination Model
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 11/04/2022
End Header --------------------------------------------------------*/
#pragma once
#include <string>

//will be component
class Shader;
class Texture {

	int w{};
	int h{};
	int comp{};
	unsigned char* image{};

	unsigned int textureID{};
	int textureNumber{};

public:
	Texture(std::string fileName);
	~Texture();
	unsigned int getTextureID() { return textureID; };
	void bindTexture();
	void unbindTexture();
	void sendTextureDataUniform(std::shared_ptr<Shader> shader, const std::string uniformName);
};