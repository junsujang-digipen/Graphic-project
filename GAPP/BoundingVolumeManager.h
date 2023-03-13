/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: BoundingVolumeManager.h
Purpose: Bounding volume management and generate hierarchy
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 2 - Bounding Volumes
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 03/04/2023
End Header --------------------------------------------------------*/
#pragma once
#include "UsingEntt.h"
class Scene;
class BoundingVolumeManager 	
{
	Scene* scene{};
	ENTT hierarchyContainer{};
	ID TDBox{};
	ID TDSphere{};
	ID BUHierarchy{};
	glm::vec3 levelColor(int level);
	float GetDistanceSquare(const ID& a, const ID& b);
	std::pair<float, std::pair<ID, ID>> closestPair_aux(std::vector< ID > const& indices, int start, int n);
	std::pair<float, std::pair<ID, ID>> findIntervalMin(std::vector< ID > const& indices, const int left, const int right, const int leftN, const int rightN, const  float leftInterval, const float rightInterval, float minDist);
	std::pair<float, std::pair<ID, ID>> closestPair(std::vector< ID > const& points);
	void BU_LevelSet(ID treeNode, int level);

	void findNodesToMerge(std::vector<ID>& HierarchyVolumes, ID &leftNode, ID &rightNode);
	void mergeBoxNodes(ID TreeNode, std::vector<ID> HierarchyVolumes);
	void mergeCircleNodes(ID TreeNode, std::vector<ID> HierarchyVolumes);
	int partitioning(std::vector<ID>& ObjIDs, int stIndex, int numObjects);
	ID calc_BU_Hierarchy(std::vector<ID> ObjIDs);
	void calc_TD_Hierarchy(ID TreeNode, std::vector<ID>& ObjIDs, int stIndex, int numObjects, int nowLevel);
	void calc_TD_SphereHierarchy(ID TreeNode, std::vector<ID>& ObjIDs, int stIndex, int numObjects, int nowLevel);
	void computeBoundingBox(ID TreeNode, std::vector<ID>& ObjIDs, int stIndex, int numObjects);
	void computeBoundingSphere(ID TreeNode, std::vector<ID>& ObjIDs , int stIndex, int numObjects);
public:
	BoundingVolumeManager(Scene *sc);
	void Update(double dt);
	void Draw();
	void calcHierarchy();

	int DrawLevel{0};
	bool DrawAllLevel{false};
	bool ShouldDrawBoundingSphere{false};
	bool ShouldDrawBoundingBox{false};
	bool ShouldDraw_BU_BoundingBoxHierarchy{ false };
	bool ShouldDraw_BU_BoundingSphereHierarchy{ false };
	bool ShouldDraw_TD_BoundingBoxHierarchy{ false };
	bool ShouldDraw_TD_BoundingSphereHierarchy{ false };
};
