/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Scene.cpp
Purpose: Scene for testing objs and loader and shaders
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 1 - Hybrid Rendering
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

ID Scene::makeEntity()
{
	ID temp = entityContainer.create();

	//Entity& tempEntity = entityContainer.emplace<Entity>(temp,this,temp);

	return temp;
}

Scene::Scene()
{
	
	shaderManager = new ShaderManager{};
	textureManager = new TextureManager{};
	meshManager = new MeshManager{};
	objManager = new ObjManager{this};
}

Scene::~Scene()
{
	delete shaderManager;
	delete textureManager;
	delete meshManager;
	delete objManager;

	entityContainer.clear();
}

void Scene::setEngine(GAPP* app) {
	engine = app;
}

void Scene::Update(double dt)
{
	objManager->update(dt);
}

ENTT& Scene::getENTT()
{
	return entityContainer;
}
