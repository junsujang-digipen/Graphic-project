/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: ObjManager.cpp
Purpose: For storing Entity datas
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 3 - Spatial Partitioning
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 02/28/2023
End Header --------------------------------------------------------*/
#include "pch.h"
#include "ObjManager.h"
#include "Scene.h"
#include "EnttComponentStructures.h"
#include "MeshManager.h"
#include "Entity.h"

ObjManager::ObjManager(Scene* sc):scene(sc)
{

}

ObjManager::~ObjManager()
{
	Objs.clear();
}

void ObjManager::update(double dt)
{
	for (auto& e:Objs) {
		e.second->update(dt);
	}
}

void ObjManager::draw()
{
	for (auto& e : Objs) {
		e.second->draw();
	}
}

std::shared_ptr<Entity> ObjManager::getObj(ID id)
{
	return Objs[id];
}

void ObjManager::enrollObj(std::shared_ptr<Entity> obj, ID id)
{
	Objs[id] = obj;
}

std::shared_ptr<Entity> ObjManager::GenObj()
{
	ID id = scene->getENTT().create();
	Objs[id] = std::make_shared<Entity>(scene, id);
	return Objs[id];
}


void ObjManager::SetSceneComponent(ID id) {
	scene->getENTT().emplace<PositionComponent>(id, PositionComponent());
	scene->getENTT().emplace<ScaleComponent>(id, ScaleComponent());
	scene->getENTT().emplace<RotateComponent>(id, RotateComponent{});
	scene->getENTT().emplace<ObjectMatrixComponent>(id, ObjectMatrixComponent{});
}
void ObjManager::SetMeshID(ID id, int MID)
{
	getObj(id)->MeshID = MID;
	scene->getENTT().emplace<MeshIDComponent>(id,  MID);
}

void ObjManager::NewBoundingSphere(BoundingVolumeType newType) {
	for (auto& e : Objs) {
		scene->getENTT().replace<BoundingSphere>(e.first, scene->getMeshManager()->make_BoundingSphere(e.second->MeshID, newType));
	}
}
