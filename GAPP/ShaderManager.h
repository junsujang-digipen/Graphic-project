/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: ShaderManager.h
Purpose: Scene for testing objs and loader and shaders
Language: c++
Platform: x64
Project: junsu.jang, CS300, Assignment 3 - Dynamic Environment Mapping
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 11/25/2022
End Header --------------------------------------------------------*/
#pragma once
#include <map>
#include <string>
#include <memory>

class Shader;
class ShaderManager {
	std::map<std::string, std::shared_ptr<Shader>> shaders{};
public:
	ShaderManager();
	~ShaderManager();
	void EnrollShader(std::string shaderName, std::shared_ptr<Shader> shader);
	std::shared_ptr<Shader> makeShader(std::string shaderName);
	std::shared_ptr<Shader> getShader(std::string shaderName);
};