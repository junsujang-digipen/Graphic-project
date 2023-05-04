/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Entity.h
Purpose: For storing Entity datas
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 3 - Spatial Partitioning
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 09/30/2022
End Header --------------------------------------------------------*/
#pragma once
#include "glm/glm.hpp"
#include "UsingEntt.h"
#include <vector>

#include <memory>

class Shader;
class OBJLoader;
class Scene;
class ComponentManager;
class Entity {
protected:
	ID thisID{};
	Scene* scene{};
	std::shared_ptr<ComponentManager> compoManager{};


	bool shouldOBJMatrixUpdate{true};

public:
	Entity(Scene* sc);
	Entity(Scene* sc, ID id);
	ID getID();
	ENTT& getENTT();

	std::shared_ptr<Shader> objShader{};
	std::shared_ptr<Shader> normalVectorShader{};

	int MeshID{};

	glm::vec3 pos{};
	glm::vec3 scale{1.f};
	glm::vec3 rotate{};
	glm::mat4 objectMatrix{};

	void objMatrixUpdate();
	void setPos(const glm::vec3 &p);
	void setScale(const glm::vec3 &s);
	const glm::vec3& getScale() { return scale; }
	void setRotate(const glm::vec3 &r);
	


	virtual void update(double dt);
	virtual void draw();
	virtual void drawNormal(int num);

};