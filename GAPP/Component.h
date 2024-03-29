/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Component.h
Purpose: For load obj file
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 3 - Spatial Partitioning
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 02/27/2023
End Header --------------------------------------------------------*/
#pragma once
class Entity;
class ComponentManager;
class Component {
	friend ComponentManager;
protected:
	Entity* owner{};
public:
	Component(Entity* o);
	virtual void update(double /*dt*/) {}
};