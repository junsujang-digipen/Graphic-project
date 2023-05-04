/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: TestScene.h
Purpose: Scene for testing objs and loader and shaders
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 3 - Spatial Partitioning
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

	glm::mat4 WTC{};

	std::shared_ptr<Shader> diffuseShader{};
	std::shared_ptr<Shader> NormalShdrProgram{};
	std::shared_ptr<Shader> LightShader{};
	std::vector<std::shared_ptr<Entity>> Obj{};
	std::vector<std::shared_ptr<Entity>> ObjSpheres{};
	std::shared_ptr<Entity> ObjCircleLine{};
	std::shared_ptr<Entity> ObjPlane{};

	std::vector<std::shared_ptr<Texture>> textures{};
	int OBJtextureMappingNum{0};
	bool IsGPUtextureMapping{true };
	bool IsPositionEntityMapping{ true };
	glm::vec3 ObjAmbient{0.1f};
	glm::vec3 ObjEmissive{ 0.f };

	int SpCurrNum{1};

	double time{};
	float orbitSpeed{0.1f};
	int normalDrawState{0};
	int nowObj{ 0 };
	float ObjRotate{0.f};

	short nowScenario{0};

public:
	TestScene();
	virtual ~TestScene();
	void Load() override;
	void Update(double dt) override;
	void Draw()  override;
	void Unload()  override;
};