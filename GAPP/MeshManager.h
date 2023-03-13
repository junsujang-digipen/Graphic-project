/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: MeshManager.h
Purpose: For load obj file
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 2 - Bounding Volumes
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 02/26/2023
End Header --------------------------------------------------------*/
#pragma once
#include "MeshData.h"
#include "EnttComponentStructures.h"
#include "BasicMeshType.h"

class MeshManager {
	std::vector<MeshData> Meshes{};

	BoundingBox GenAABBBoundVolume(int i);
	BoundingSphere GenRitterBoundVolume(int i);
	BoundingSphere GenLarssonBoundVolume(int i);
	BoundingSphere GenPCABoundVolume(int i);

public:
	MeshManager();
	~MeshManager();

	int push_MeshData(MeshData& md);
	const MeshData& getMeshData(int i);
	MeshData& getMeshDataRef(int i);

	BoundingBox make_BoundingBox(int i, BoundingVolumeType type = BoundingVolumeType::AABB);
	BoundingSphere make_BoundingSphere(int i, BoundingVolumeType type = BoundingVolumeType::RITTER);
};

void CircleGrow(glm::vec3& center, float& radius, std::vector<glm::vec3>& extremePoints);