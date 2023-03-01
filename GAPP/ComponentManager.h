/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: ComponentManager.h
Purpose: For load obj file
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 1 - Hybrid Rendering
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 02/27/2023
End Header --------------------------------------------------------*/
#pragma once
#include <memory>
class Component;
class ComponentManager {
	void* owner{};// use template?
	std::vector<std::shared_ptr<Component>> components{};
public:
	ComponentManager(void* o);
	~ComponentManager();
	void pushComponent(std::shared_ptr<Component> c);
	void update(double dt);
};