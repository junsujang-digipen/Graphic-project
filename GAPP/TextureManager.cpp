/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: TextureManager.cpp
Purpose: Scene for testing objs and loader and shaders
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 1 - Hybrid Rendering
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 11/25/2022
End Header --------------------------------------------------------*/
#include "pch.h"
#include "TextureManager.h"
#include "ShaderManager.h"
#include "Texture.h"

TextureManager::TextureManager()
{
}

void TextureManager::EnrollTexture(std::string textureName, std::string texturePath)
{
	if (textures.find(textureName) != textures.end()) {
		std::cout << textureName<< ": existed texture covered" << std::endl;
	}
	textures[textureName] = std::make_shared<Texture>(texturePath);
}

void TextureManager::EnrollTexture(std::string textureName, std::string texturePath, unsigned wrapMethod)
{
	if (textures.find(textureName) != textures.end()) {
		std::cout << textureName << ": existed texture covered" << std::endl;
	}
	textures[textureName] = std::make_shared<Texture>(texturePath, wrapMethod);
}

void TextureManager::EnrollTexture(std::string textureName, unsigned int texID, int width, int height, unsigned wrapMethod)
{
	if (textures.find(textureName) != textures.end()) {
		std::cout << textureName << ": existed texture covered" << std::endl;
	}
	textures[textureName] = std::make_shared<Texture>(texID, width,height, wrapMethod);
}

void TextureManager::SetTextureMappingData(std::string shaderName, std::string textureName, std::string uniformName)
{
	// texture name, uniform name be repeated check??
	textureMappingDatas[shaderName].push_back(TextureMappingData{ textureName,uniformName });
}

void TextureManager::bindTextures(std::string shaderName, ShaderManager* shaderManager)
{
	unsigned int textureNumber{0};
	std::shared_ptr<Shader> shader{ shaderManager->getShader(shaderName) };
	if (shader == nullptr) {
		std::cout << shaderName << " does not exist in shader manager" << std::endl;
		return;
	}
	for (auto& data : textureMappingDatas[shaderName]) {
		textures[data.textureName]->setTextureNumber(textureNumber++);
		textures[data.textureName]->sendTextureDataUniform(shader, data.uniformName);
		textures[data.textureName]->bindTexture();
	}
}

void TextureManager::bindAllTextures(ShaderManager* shaderManager)
{
	for (auto& data: textureMappingDatas) {
		bindTextures((data.first),shaderManager);
	}
}

TextureManager::~TextureManager()
{
	textures.clear();
	textureMappingDatas.clear();
}
