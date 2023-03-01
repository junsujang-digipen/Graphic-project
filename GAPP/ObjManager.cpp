/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: ObjManager.cpp
Purpose: For storing Entity datas
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 1 - Hybrid Rendering
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 02/28/2023
End Header --------------------------------------------------------*/
#include "pch.h"
#include "ObjManager.h"
#include "Scene.h"
#include "EnttComponentStructures.h"

ObjManager::ObjManager(Scene* sc):scene(sc)
{

}

ObjManager::~ObjManager()
{
	Objs.clear();
}

void ObjManager::update(double dt)
{
}

void ObjManager::draw()
{
}

std::shared_ptr<Entity> ObjManager::getObj(ID id)
{
	return Objs[id];
}

void ObjManager::enrollObj(std::shared_ptr<Entity> obj, ID id)
{
	Objs[id] = obj;
}

void ObjManager::SetMeshID(ID id, int MID)
{
	scene->getENTT().emplace<MeshID>(id,  MID);
}

