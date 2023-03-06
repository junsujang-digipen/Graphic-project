/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Scene.h
Purpose: Base of scenes
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 1 - Hybrid Rendering
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 09/27/2022
End Header --------------------------------------------------------*/
#pragma once
#include "UsingEntt.h"

class GAPP;
class ShaderManager;
class TextureManager;
class MeshManager;
class Entity;
class ObjManager;
class BoundingVolumeManager;
class Shader;
class Scene {
protected:
	ENTT entityContainer{};

	GAPP* engine{};

	ShaderManager* shaderManager{};
	TextureManager* textureManager{};
	MeshManager* meshManager{};
	ObjManager* objManager{};
	BoundingVolumeManager* bvManager{};
	//IMGui manager

	glm::mat4 WTC{};

	ID makeEntity();

public:
	Scene();
	virtual ~Scene();
	void setEngine(GAPP* app);
	virtual void Load() {};
	virtual void Update(double /*dt*/);
	//virtual void ImGUIUpdate() = 0;
	virtual void Draw();
	virtual void Unload() {};

	ENTT& getENTT();
	ShaderManager* getShaderManager() { return shaderManager; }
	TextureManager* getTextureManager() { return textureManager; }
	MeshManager* getMeshManager() {return meshManager;}
	ObjManager* getObjManager() { return objManager; }
	std::shared_ptr<Shader> get_DebugShader();
	glm::mat4& get_WTC() { return WTC; }
};