/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: EnttComponentStructures.h
Purpose: For storing Entity datas
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 3 - Spatial Partitioning
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 02/28/2023
End Header --------------------------------------------------------*/
#pragma once
#include "UsingEntt.h"

struct MeshIDComponent {
	int id{};
};
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
struct OctoLeafComponent {
	ID leaf1{ entt::null };
	ID leaf2{ entt::null };
	ID leaf3{ entt::null };
	ID leaf4{ entt::null };
	ID leaf5{ entt::null };
	ID leaf6{ entt::null };
	ID leaf7{ entt::null };
	ID leaf8{ entt::null };
};
struct BSPLeafComponent {
	ID front{ entt::null };
	ID behind{ entt::null };
};
struct PlaneComponent {
	glm::vec4 plane{};
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

struct ObjectMeshData {
	int meshID{};
	ID objID{entt::null};
};
struct OctreeObjectDataComponent {
	std::vector<ObjectMeshData> IDs{};
};
struct BSPtreeObjectDataComponent {
	std::vector<ObjectMeshData> IDs{};
};