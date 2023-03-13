/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Scene.cpp
Purpose: Scene for testing objs and loader and shaders
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 2 - Bounding Volumes
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 11/25/2022
End Header --------------------------------------------------------*/
#include "pch.h"
#include "Scene.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "MeshManager.h"
#include "Entity.h"
#include "ObjManager.h"
#include "BoundingVolumeManager.h"
#include "Shader.h"

ID Scene::makeEntity()
{
	ID temp = entityContainer.create();

	//Entity& tempEntity = entityContainer.emplace<Entity>(temp,this,temp);

	return temp;
}

Scene::Scene()
{
	makeEntity();
	shaderManager = new ShaderManager{};
	textureManager = new TextureManager{};
	meshManager = new MeshManager{};
	objManager = new ObjManager{this};
	bvManager = new BoundingVolumeManager{this};
	const std::string VshdrNormal{ ShaderHelper::getShaderSourceFromFile("../Shaders/C_DebugShader.vert") };
	const std::string FshdrNormal{ ShaderHelper::getShaderSourceFromFile("../Shaders/C_DebugShader.frag") };
	shaderManager->makeShader("Debug Shader");
	{
		GLuint vshdr = shaderManager->getShader("Debug Shader")->compileShader(ShaderType::VERTEX_SHADER, { VshdrNormal });
		GLuint fshdr = shaderManager->getShader("Debug Shader")->compileShader(ShaderType::FRAGMENT_SHADER, { FshdrNormal });
		shaderManager->getShader("Debug Shader")->attachShader(vshdr);
		shaderManager->getShader("Debug Shader")->attachShader(fshdr);
		shaderManager->getShader("Debug Shader")->linkProgram();
	}
}

Scene::~Scene()
{
	delete shaderManager;
	delete textureManager;
	delete meshManager;
	delete objManager;
	delete bvManager;

	entityContainer.clear();
}

void Scene::setEngine(GAPP* app) {
	engine = app;
}

void Scene::Update(double dt)
{
	objManager->update(dt);
	bvManager->Update(dt);
}

void Scene::Draw()
{
	bvManager->Draw();
}

ENTT& Scene::getENTT()
{
	return entityContainer;
}

std::shared_ptr<Shader> Scene::get_DebugShader()
{
	return  shaderManager->getShader("Debug Shader");
}
