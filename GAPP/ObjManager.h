/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: ObjManager.h
Purpose: For storing Entity datas
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 1 - Hybrid Rendering
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 02/28/2023
End Header --------------------------------------------------------*/
#pragma once
#include <map>
#include <memory>
#include "UsingEntt.h"
class Scene;
class Entity;
class ObjManager {
	Scene* scene{};
	std::map<ID,std::shared_ptr<Entity>> Objs{};
public:
	ObjManager(Scene* sc);
	~ObjManager();

	void update(double dt);
	void draw();


	std::shared_ptr<Entity> getObj(ID id);
	
	void enrollObj(std::shared_ptr<Entity> obj , ID id);
	std::shared_ptr<Entity> GenObj();

	void SetSceneComponent(ID id);
	void SetMeshID(ID id, int MeshID);
};