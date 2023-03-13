/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: EnttComponentStructures.h
Purpose: For storing Entity datas
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 2 - Bounding Volumes
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 02/28/2023
End Header --------------------------------------------------------*/
#pragma once
#include "UsingEntt.h"

//struct MeshID {
//	int id{};
//};
//struct Model {
//	
//};
struct BoundingBox {
	glm::vec3 Center{};
	glm::vec3 HalfExtend{};
	int DebugMeshID{};
};

struct BoundingSphere {
	glm::vec3 Center{};
	float radius{};
	int DebugMeshID{};
};

struct IncludeBVComponent {
	std::vector<ID> BVIDs{};
};

struct LeafComponent {
	ID left{entt::null};
	ID right{ entt::null };
};
struct LevelComponent {
	int level{};
};
enum class BVHierarchyType {
	BOTTOM_UP = 0,
	TOP_DOWN,
};


struct PositionComponent {
	glm::vec3 pos{0.f};
};
struct ScaleComponent {
	glm::vec3 scale{ 1. };
};
struct RotateComponent {
	glm::vec3 rotate{0.f};
};
struct ObjectMatrixComponent {
	glm::mat4 objectMatrix{1.f};
};
