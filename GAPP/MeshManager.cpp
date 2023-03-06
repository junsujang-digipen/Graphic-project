/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: MeshManager.cpp
Purpose: For load obj file
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 1 - Hybrid Rendering
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 02/26/2023
End Header --------------------------------------------------------*/
#include "pch.h"
#include "MeshManager.h"
#include "OBJLoader.h"

BoundingBox MeshManager::GenAABBBoundVolume(int i)
{
	BoundingBox temp{};
	temp.Center = (Meshes[i].boundBoxMax + Meshes[i].boundBoxMin) / 2.f;
	temp.HalfExtend = Meshes[i].boundBoxMax - temp.Center;
	temp.DebugMeshID = Cast<int>(BasicMeshType::Box);
	//temp.DebugMeshID = 0;
	return temp;
}
void CircleGrow(glm::vec3 & center, float& radius, std::vector<glm::vec3> & extremePoints) {
	for (auto &v:extremePoints) {
		float tempRadi{ glm::distance(center, v) };
		if (tempRadi > radius) {
			tempRadi += radius;
			tempRadi /= 2.f;
			std::swap(tempRadi,radius);
			glm::vec3 direc{glm::normalize(v - center)};
			center += direc * (radius - tempRadi);
		}
	}
}
BoundingSphere MeshManager::GenRitterBoundVolume(int i)
{
	BoundingSphere temp{};
	constexpr int pointNum{6};
	std::vector<glm::vec3> extremePoints(pointNum);

	for (auto& v: Meshes[i].VertexDatas) {
		extremePoints[0] = v.x == Meshes[i].boundBoxMax.x ? v : extremePoints[0];
		extremePoints[1] = v.x == Meshes[i].boundBoxMin.x ? v : extremePoints[1];
		extremePoints[2] = v.y == Meshes[i].boundBoxMax.y ? v : extremePoints[2];
		extremePoints[3] = v.y == Meshes[i].boundBoxMin.y ? v : extremePoints[3];
		extremePoints[4] = v.z == Meshes[i].boundBoxMax.z ? v : extremePoints[4];
		extremePoints[5] = v.z == Meshes[i].boundBoxMin.z ? v : extremePoints[5];
	}
	glm::ivec2 extremePoint{};
	float radi{};
	for (int k = 0; k < pointNum; ++k) {
		for(int j = 0; j < pointNum; ++j){
			if (k != j) {
				const float tempRadi{ glm::distance(extremePoints[k], extremePoints[j]) };
				if (radi < tempRadi) {
					radi = tempRadi;
					extremePoint = {k,j};
				}
			}
		}
	}

	temp.Center = (extremePoints[extremePoint.x] + extremePoints[extremePoint.y]) / 2.f;
	temp.radius = radi / 2.f;

	CircleGrow(temp.Center, temp.radius, extremePoints);
	
	temp.DebugMeshID = Cast<int>(BasicMeshType::Sphere);
	//temp.DebugMeshID = 1;

	return temp;
}
BoundingSphere MeshManager::GenLarssonBoundVolume(int i)
{
	BoundingSphere temp{};
	temp.Center = (Meshes[i].boundBoxMax + Meshes[i].boundBoxMin) / 2.f;


	temp.DebugMeshID = Cast<int>(BasicMeshType::Sphere);
	return temp;
}
BoundingSphere MeshManager::GenPCABoundVolume(int i)
{
	BoundingSphere temp{};
	temp.Center = (Meshes[i].boundBoxMax + Meshes[i].boundBoxMin) / 2.f;


	temp.DebugMeshID = Cast<int>(BasicMeshType::Sphere);
	return temp;
}
MeshManager::MeshManager()
{
	OBJLoader box{};
	box.FileLoad("../Objects/cube2.obj");
	MeshData boxMD{ box.makeMeshData({1.f,1.f,1.f}) };
	push_MeshData(boxMD);

	OBJLoader Shpere{};
	MakeSphereData(20, 20, Shpere.VertexDatas, Shpere.VertexNormalDatas, Shpere.FaceNormalDatas, Shpere.idxDatas);
	MeshData ShpereMD{ Shpere.makeMeshData({1.f,1.f,1.f}) };
	push_MeshData(ShpereMD);

	OBJLoader SkyBox{};
	SkyBox.FileLoad("../Objects/SkyBox.obj");
	MeshData SkyBoxMD{ SkyBox.makeMeshData({1.f,1.f,1.f}) };
	push_MeshData(SkyBoxMD);
}

MeshManager::~MeshManager()
{
	for (auto& m:Meshes) {
		m.unload();
	}
	Meshes.clear();
}

int MeshManager::push_MeshData(MeshData& md)
{
	md.load();
	Meshes.push_back(md);
	return static_cast<int>(Meshes.size() - 1);
}

const MeshData& MeshManager::getMeshData(int i)
{
	return Meshes[i];
}

MeshData& MeshManager::getMeshDataRef(int i)
{
	return Meshes[i];
}

BoundingBox MeshManager::make_BoundingBox(int i, BoundingVolumeType type)
{
	BoundingBox temp{};
	switch (type)
	{
	case BoundingVolumeType::AABB:
		temp = GenAABBBoundVolume(i);
		break;
	default:
		std::cout << "Type Match failed" << std::endl;
		break;
	}
	return temp;
}

BoundingSphere MeshManager::make_BoundingSphere(int i, BoundingVolumeType type)
{
	BoundingSphere temp{};
	switch (type)
	{
	case BoundingVolumeType::AABB:
		std::cout << "Type Match failed" << std::endl;
		break;
	case BoundingVolumeType::RITTER:
		temp = GenRitterBoundVolume(i);
		break;
	case BoundingVolumeType::LARSSON:
		temp = GenLarssonBoundVolume(i);
		break;
	case BoundingVolumeType::PCA:
		temp = GenPCABoundVolume(i);
		break;
	default:
		break;
	}
	return temp;
}
