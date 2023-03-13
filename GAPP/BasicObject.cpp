/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: BasicObject.cpp
Purpose: Scene for testing objs and loader and shaders
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 2 - Bounding Volumes
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 11/26/2022
End Header --------------------------------------------------------*/
#include "pch.h"
#include "BasicObject.h"
#include "Shader.h"
#include "Entity.h"

//BasicObject::BasicObject(Entity* entt):entity(entt)
//{
//	entity->getENTT().emplace<>(entity->getID());
//}

void BasicObject::draw()
{
	glm::mat3 u_normal = glm::transpose(glm::inverse(objectMatrix));
	objShader->sendUniformMatrix3fv("u_normalMatrix", u_normal);
	Entity::draw();
}
