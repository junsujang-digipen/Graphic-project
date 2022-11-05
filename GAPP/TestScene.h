/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: TestScene.h
Purpose: Scene for testing objs and loader and shaders
Language: c++
Platform: x64
Project: junsu.jang, CS300, Assignment 1 - Render an OBJ file
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 09/27/2022
End Header --------------------------------------------------------*/
#pragma once
#include "Scene.h"
#include <memory>
#include <vector>
#include "glm/glm.hpp"
class Shader;
class Entity;
class Texture;
class TestScene :public Scene {
	std::shared_ptr<Shader> diffuseShader{};
	std::shared_ptr<Shader> NormalShdrProgram{};
	std::vector<std::shared_ptr<Entity>> Obj{};
	std::vector<std::shared_ptr<Entity>> ObjSpheres{};
	std::shared_ptr<Entity> ObjCircleLine{};
	std::shared_ptr<Entity> ObjPlane{};

	std::vector<std::shared_ptr<Texture>> textures{};
	glm::vec3 ObjAmbient{0.5f};
	glm::vec3 ObjEmissive{ 0.f };

	int SpCurrNum{1};

	double time{};
	float orbitSpeed{0.1f};
	int normalDrawState{0};
	int nowObj{ 0 };
	float ObjRotate{0.f};

	bool IsScenario3{false};

public:
	TestScene();
	void Load() override;
	void Update(double dt) override;
	void Draw()  override;
	void Unload()  override;
};