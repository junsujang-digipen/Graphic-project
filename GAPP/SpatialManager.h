/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: SpatialManager.h
Purpose: Bounding volume management and generate hierarchy
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 3 - Spatial Partitioning
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 04/10/2023
End Header --------------------------------------------------------*/
#pragma once
#include "UsingEntt.h"
#include "MeshData.h"
class Scene;
class SpatialManager {
	Scene* scene{};
	ENTT hierarchyContainer{};

	ID OctreeHeadNode{};
	ID BSPtreeHeadNode{};

	struct SplitedMeshOBJ {
		ID objID{ entt::null };
		bool isSplited{ false };
		MeshData splitedData{};
	};

	void ComputeOctree(ID TreeNode, std::vector<ID>& ObjIDs, glm::vec3 center, glm::vec3 halfExtend, int nowLevel);
	void ComputeBSPtree(ID TreeNode, std::vector<SplitedMeshOBJ>& ObjIDs, glm::vec4 plane, int nowLevel, int Triangles );
public:
	SpatialManager(Scene* sc);
	~SpatialManager();
	void BuildOctree();
	void BuildBSPtree();
	void Draw();
	bool ShouldDraw_Octree{false};
	bool ShouldDraw_BSPtree{ false };
	bool ShouldDraw_DebugMesh{ false };
	bool DrawAllLevel{ false };
	int DrawLevel{0};
};