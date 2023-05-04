/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: MeshManager.cpp
Purpose: For load obj file
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 3 - Spatial Partitioning
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 02/26/2023
End Header --------------------------------------------------------*/
#include "pch.h"
#include "MeshManager.h"
#include "OBJLoader.h"


//Epos 6(Ritter)
static constexpr glm::vec3 Normal001_1 = { 1.f, 0.f, 0.f };
static constexpr glm::vec3 Normal001_2 = { 0.f, 1.f, 0.f };
static constexpr glm::vec3 Normal001_3 = { 0.f, 0.f, 1.f };
//Epos 14
static constexpr glm::vec3 Normal111_1 = { 1.f, 1.f, 1.f };
static constexpr glm::vec3 Normal111_2 = { 1.f, 1.f, -1.f };
static constexpr glm::vec3 Normal111_3 = { 1.f, -1.f, 1.f };
static constexpr glm::vec3 Normal111_4 = { 1.f, -1.f, -1.f };
//Epos 26
static constexpr glm::vec3 Normal011_1 = { 1.f, 1.f, 0.f };
static constexpr glm::vec3 Normal011_2 = { 1.f, -1.f, 0.f };
static constexpr glm::vec3 Normal011_3 = { 1.f, 0.f, 1.f };
static constexpr glm::vec3 Normal011_4 = { 1.f, 0.f, -1.f };
static constexpr glm::vec3 Normal011_5 = { 0.f, 1.f, 1.f };
static constexpr glm::vec3 Normal011_6 = { 0.f, 1.f, -1.f };


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
	std::vector<glm::vec3> extremePoints{
		glm::vec3{-FLT_MAX},
		glm::vec3{FLT_MAX},
		glm::vec3{-FLT_MAX},
		glm::vec3{FLT_MAX},
		glm::vec3{-FLT_MAX},
		glm::vec3{FLT_MAX}
	};

	glm::vec3 x_axis{1.f,0.f,0.f};
	glm::vec3 y_axis{0.f,1.f,0.f};
	glm::vec3 z_axis{0.f,0.f,1.f};

	for (auto& v: Meshes[i].VertexDatas) { // TODO
		glm::vec3 dotRes{ glm::dot(x_axis, v),glm::dot(y_axis, v) , glm::dot(z_axis, v) };

		extremePoints[0] = dotRes.x > glm::dot(x_axis, extremePoints[0]) ? v : extremePoints[0];
		extremePoints[1] = dotRes.x < glm::dot(x_axis, extremePoints[1]) ? v : extremePoints[1];

		extremePoints[2] = dotRes.y > glm::dot(y_axis, extremePoints[2]) ? v : extremePoints[2];
		extremePoints[3] = dotRes.y < glm::dot(y_axis, extremePoints[3]) ? v : extremePoints[3];

		extremePoints[4] = dotRes.z > glm::dot(z_axis, extremePoints[4]) ? v : extremePoints[4];
		extremePoints[5] = dotRes.z < glm::dot(z_axis, extremePoints[5]) ? v : extremePoints[5];
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

	CircleGrow(temp.Center, temp.radius, Meshes[i].VertexDatas);
	
	temp.DebugMeshID = Cast<int>(BasicMeshType::Sphere);
	//temp.DebugMeshID = 1;

	return temp;
}
BoundingSphere MeshManager::GenLarssonBoundVolume(int i)
{
	const std::vector<glm::vec3> LarssonNormals = { 
		Normal001_1, Normal001_2, Normal001_3, 
		Normal111_1, Normal111_2, Normal111_3, 
		Normal111_4, Normal011_1, Normal011_2, 
		Normal011_3, Normal011_4, Normal011_5, 
		Normal011_6 };

	BoundingSphere temp{};
	std::vector<glm::vec3> extremePoints{};
	for (auto& axis: LarssonNormals) {
		extremePoints.push_back(Meshes[i].VertexDatas[0]);
		extremePoints.push_back(Meshes[i].VertexDatas[0]);
		int k{static_cast<int>(extremePoints.size()) - 2};
		for (auto& v : Meshes[i].VertexDatas) { 
			float dotRes{ glm::dot(axis, v)};

			extremePoints[k] = dotRes > glm::dot(axis, extremePoints[k]) ? v : extremePoints[k];
			extremePoints[k+1] = dotRes < glm::dot(axis, extremePoints[k+1]) ? v : extremePoints[k+1];
		}
	}
	int pointNum{ static_cast<int>(extremePoints.size()) };
	glm::ivec2 extremePoint{};
	float radi{};
	for (int k = 0; k < pointNum; ++k) {
		for (int j = 0; j < pointNum; ++j) {
			if (k != j) {
				const float tempRadi{ glm::distance(extremePoints[k], extremePoints[j]) };
				if (radi < tempRadi) {
					radi = tempRadi;
					extremePoint = { k,j };
				}
			}
		}
	}

	temp.Center = (extremePoints[extremePoint.x] + extremePoints[extremePoint.y]) / 2.f;
	temp.radius = radi / 2.f;

	CircleGrow(temp.Center, temp.radius, Meshes[i].VertexDatas);

	temp.DebugMeshID = Cast<int>(BasicMeshType::Sphere);
	return temp;
}
BoundingSphere MeshManager::GenPCABoundVolume(int i)
{
	BoundingSphere temp{};
	temp.Center = {};
	for (auto& v : Meshes[i].VertexDatas) {
		temp.Center += v;
	}
	temp.Center /= static_cast<float>(Meshes[i].VertexDatas.size());

	glm::mat3 covMat{};
	for (const auto& v : Meshes[i].VertexDatas)
	{
		covMat[0][0] += (v.x - temp.Center.x) * (v.x - temp.Center.x);
		covMat[1][0] += (v.x - temp.Center.x) * (v.y - temp.Center.y);
		covMat[2][0] += (v.x - temp.Center.x) * (v.z - temp.Center.z);
		covMat[1][1] += (v.y - temp.Center.y) * (v.y - temp.Center.y);
		covMat[2][1] += (v.y - temp.Center.y) * (v.z - temp.Center.z);
		covMat[2][2] += (v.z - temp.Center.z) * (v.z - temp.Center.z);
	}
	glm::vec3 sc{ Meshes[i].boundBoxMax - Meshes[i].boundBoxMin };
	covMat[0][0] /= sc.x * sc.x;
	covMat[1][0] /= sc.x * sc.y;
	covMat[2][0] /= sc.x * sc.z;
	covMat[1][1] /= sc.y * sc.y;
	covMat[2][1] /= sc.y * sc.z;
	covMat[2][2] /= sc.z * sc.z;

	covMat[0][1] = covMat[1][0];
	covMat[0][2] = covMat[2][0];
	covMat[1][2] = covMat[2][1];

	covMat /= static_cast<float>(Meshes[i].VertexDatas.size());
	/// ///////////
	glm::mat3 Q(1.f);
	glm::mat3 X(1.f);

	bool convergence = true;
	float lastSum = 0.f;
	int p = 0, q = 0;
	constexpr int ColRowLeng{3};
	while (convergence == true)
	{
		float maxElement = 0.f;
		float sum = 0.f;
		for (int k = 0; k < ColRowLeng; ++k)
		{
			for (int j = 0; j < ColRowLeng; ++j)
			{
				if (k == j) continue;
				const float abs_value = glm::abs(covMat[k][j]);
				sum += abs_value;
				if (maxElement < abs_value)
				{
					maxElement = abs_value;
					p = k;
					q = j;
				}
			}
		}

		convergence = glm::abs(lastSum - sum) > std::numeric_limits<float>::epsilon();
		if (!convergence) break;
		lastSum = sum;

		float beta = (covMat[q][q] - covMat[p][p]) / (2.f * covMat[p][q]);
		float t = glm::sign(beta) / (glm::abs(beta) + glm::sqrt(beta * beta + 1.f));
		float cosTheta = 1.f / glm::sqrt(t * t + 1.f);
		float sinTheta = cosTheta * t;
		Q = glm::mat3(1.f);
		Q[p][p] = cosTheta;
		Q[p][q] = -sinTheta;
		Q[q][p] = sinTheta;
		Q[q][q] = cosTheta;
		X *= Q;
		covMat = glm::transpose(Q) * covMat * Q;
	}

	float maxElement = 0.f;
	int maxIndex = 0;
	for (int k = 0; k < 3; ++k)
	{
		if (glm::abs(maxElement) < glm::abs(covMat[k][k]))
		{
			maxElement = covMat[k][k];
			maxIndex = k;
		}
	}
	std::vector<glm::vec3> extremePoints{ Meshes[i].VertexDatas[0] , Meshes[i].VertexDatas[1] };
	glm::vec3 axis{ X[maxIndex] };
	for (auto& v : Meshes[i].VertexDatas) {
		float dotRes{ glm::dot(axis, v) };
		float dot0{ glm::dot(axis, extremePoints[0]) };
		float dot1{ glm::dot(axis, extremePoints[1]) };
		float dist{ glm::distance(temp.Center,v) };
		if (dotRes > dot0) {
			extremePoints[0] = v;
		}
		else if(dotRes == dot0){
			if (glm::distance(temp.Center, extremePoints[0]) < dist) {
				extremePoints[0] = v;
			}
		}

		if (dotRes < dot1) {
			extremePoints[1] = v;
		}
		else if (dotRes == dot1) {
			if (glm::distance(temp.Center, extremePoints[1]) < dist) {
				extremePoints[1] = v;
			}
		}
	}

	temp.Center = (extremePoints[1] + extremePoints[0]) / 2.f;
	temp.radius = glm::distance(extremePoints[1], extremePoints[0]) / 2.f;
	CircleGrow(temp.Center, temp.radius, Meshes[i].VertexDatas);

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
