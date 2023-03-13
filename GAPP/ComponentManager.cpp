/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: ComponentManager.cpp
Purpose: For load obj file
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 2 - Bounding Volumes
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 02/27/2023
End Header --------------------------------------------------------*/
#include "pch.h"
#include "ComponentManager.h"
#include "Component.h"
ComponentManager::ComponentManager(void* o): owner(o)
{
}

ComponentManager::~ComponentManager()
{
	components.clear();
}

void ComponentManager::pushComponent(std::shared_ptr<Component> c)
{
	components.push_back(c);
	c.get()->owner = reinterpret_cast<Entity*>(owner);
}

void ComponentManager::update(double dt)
{
	for (auto& c: components) {
		c.get()->update(dt);
	}
}
