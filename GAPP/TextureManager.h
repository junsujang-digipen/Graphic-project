/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: TextureManager.h
Purpose: Scene for testing objs and loader and shaders
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 2 - Bounding Volumes
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 11/25/2022
End Header --------------------------------------------------------*/
#pragma once
#include <map>
#include <vector>
class Texture;
class ShaderManager;
class TextureManager {

	struct TextureMappingData {
		std::string textureName{};
		std::string uniformName{};
	};

	std::map<std::string, std::shared_ptr<Texture>> textures{};
	std::map<std::string, std::vector<TextureMappingData>> textureMappingDatas{};

public:
	TextureManager();
	void EnrollTexture(std::string textureName, std::string texturePath);
	void EnrollTexture(std::string textureName, std::string texturePath,unsigned wrapMethod);
	void EnrollTexture(std::string textureName, unsigned int texID, int width, int height, unsigned wrapMethod);
	void SetTextureMappingData(std::string shaderName, std::string textureName, std::string uniformName);

	void bindTextures(std::string shaderName, ShaderManager* shaderManager);
	void bindAllTextures(ShaderManager* shaderManager);
	~TextureManager();
};