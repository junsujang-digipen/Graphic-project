/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: ShaderManager.cpp
Purpose: Scene for testing objs and loader and shaders
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 3 - Spatial Partitioning
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 11/25/2022
End Header --------------------------------------------------------*/

#include "pch.h"
#include "ShaderManager.h"
#include "Shader.h"
ShaderManager::ShaderManager()
{
}

ShaderManager::~ShaderManager()
{
	shaders.clear();
}

void ShaderManager::EnrollShader(std::string shaderName, std::shared_ptr<Shader> shader)
{
	if (shaders.find(shaderName) != shaders.end()) {
		std::cout << shaderName << ": existed shader covered" << std::endl;
	}
	shaders[shaderName] = shader;
}

std::shared_ptr<Shader> ShaderManager::makeShader(std::string shaderName)
{
	std::shared_ptr<Shader> shader = std::make_shared<Shader>(shaderName);
	EnrollShader(shaderName, shader);
	return shader;
}

std::shared_ptr<Shader> ShaderManager::getShader(std::string shaderName)
{
	if (shaders.find(shaderName) == shaders.end()) {
		return nullptr;
	}
	return shaders[shaderName];
}
