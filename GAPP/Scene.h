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

class GAPP;
class ShaderManager;
class TextureManager;
class Scene {
protected:
	ShaderManager* shaderManager{};
	TextureManager* textureManager{};
public:
	Scene();
	virtual ~Scene();
	virtual void Load() = 0;
	virtual void Update(double dt) = 0;
	//virtual void ImGUIUpdate() = 0;
	virtual void Draw() = 0;
	virtual void Unload() = 0;
};