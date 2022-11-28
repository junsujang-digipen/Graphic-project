/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Scene.cpp
Purpose: Scene for testing objs and loader and shaders
Language: c++
Platform: x64
Project: junsu.jang, CS300, Assignment 3 - Dynamic Environment Mapping
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 11/25/2022
End Header --------------------------------------------------------*/
#include "pch.h"
#include "Scene.h"
#include "ShaderManager.h"
#include "TextureManager.h"

Scene::Scene()
{
	shaderManager = new ShaderManager{};
	textureManager = new TextureManager{};
}

Scene::~Scene()
{
	delete shaderManager;
	delete textureManager;
}
