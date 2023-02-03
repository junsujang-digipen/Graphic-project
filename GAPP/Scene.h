/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Scene.h
Purpose: Base of scenes
Language: c++
Platform: x64
Project: junsu.jang, CS300, Assignment 3 - Dynamic Environment Mapping
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 09/27/2022
End Header --------------------------------------------------------*/
#pragma once
#include "UsingEntt.h"

class GAPP;
class ShaderManager;
class TextureManager;
class Entity;
class Scene {
protected:
	ENTT entityContainer{};

	ShaderManager* shaderManager{};
	TextureManager* textureManager{};

	Entity* makeEntity();

public:
	Scene();
	virtual ~Scene();
	virtual void Load() {};
	virtual void Update(double /*dt*/) {};
	//virtual void ImGUIUpdate() = 0;
	virtual void Draw() {};
	virtual void Unload() {};

	ENTT& getENTT();
};