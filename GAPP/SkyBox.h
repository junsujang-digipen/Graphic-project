/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: SkyBox.h
Purpose: Scene for testing objs and loader and shaders
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 2 - Bounding Volumes
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 11/26/2022
End Header --------------------------------------------------------*/
#pragma once
#include "Entity.h"

class Camera;
class ShaderManager;
class TextureManager;
class SkyBox :public Entity {
	Camera* targetCamera{};
	ShaderManager* shaderManager{};
	TextureManager* textureManager{};
	std::string skyBoxShaderName{ "SkyBox Shader" };
	void genShader();
	void load();
public:
	SkyBox(Scene* sc, ID id);
	//SkyBox(ShaderManager* sm, TextureManager* tm, std::string cubeMapPath);
	//SkyBox(ShaderManager* sm, TextureManager* tm, std::string left, std::string right, std::string bottom, std::string top, std::string back, std::string front);

	virtual ~SkyBox();
	void setTargetCamera(Camera*);
	//void setTexture(std::string cubeMapPath);
	void setTexture(std::string left, std::string right, std::string bottom, std::string top, std::string back, std::string front);

	void update(double dt)override;
	void draw()override;
	void drawNormal(int num)override;

	std::string getShaderName();
};