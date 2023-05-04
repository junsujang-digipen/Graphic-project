/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: SpatialManager.cpp
Purpose: Bounding volume management and generate hierarchy
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 3 - Spatial Partitioning
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 04/10/2023
End Header --------------------------------------------------------*/
#include "pch.h"
#include "SpatialManager.h"
#include "Scene.h"
#include "EnttComponentStructures.h"
#include "BasicMeshType.h"
#include "IntersectTest.h"
#include "MeshManager.h"
#include <GL/glew.h>
#include "Shader.h"
#include "glm/ext/matrix_transform.hpp"
#include "MeshManager.h"
#include "Random.h"

#define MAX_LEVEL 8
#define MIN_TRIANGLES 3000

glm::vec3 levelColor(int level);

void SpatialManager::ComputeOctree(ID TreeNode, std::vector<ID>& ObjIDs,glm::vec3 center, glm::vec3 halfExtend , int nowLevel)
{

	hierarchyContainer.emplace_or_replace<LevelComponent>(TreeNode, nowLevel);
	hierarchyContainer.emplace_or_replace<BoundingBox>(TreeNode, center, halfExtend,Cast(BasicMeshType::Box));

	//obj check
	std::vector<ID> includedObjIDs{};
	ENTT& reg = scene->getENTT();
	int triangleNum{0};
	Box3D box{ BoundingBox{center,halfExtend} };
	for (auto& o:ObjIDs) {
		BoundingBox& objBB{ reg.get<BoundingBox>(o) };
		if (BoxBoxIntersect(objBB, BoundingBox{ center,halfExtend }) == true) {
			const glm::mat4 Matrix = reg.get<ObjectMatrixComponent>(o).objectMatrix;
			includedObjIDs.push_back(o);
			if (triangleNum > MIN_TRIANGLES) continue;
			MeshData& tempMeshData =  scene->getMeshManager()->getMeshDataRef(reg.get<MeshIDComponent>(o).id);
			int size{ (int)tempMeshData.idxDatas.size() };
			for (int i = 0; i < size; i +=3) {
				Triangle tri{
					glm::vec3{Matrix * glm::vec4{tempMeshData.VertexDatas[tempMeshData.idxDatas[i]],1.f} },
					glm::vec3{Matrix * glm::vec4{tempMeshData.VertexDatas[tempMeshData.idxDatas[i + 1]],1.f} } ,
					glm::vec3{Matrix * glm::vec4{tempMeshData.VertexDatas[tempMeshData.idxDatas[i + 2]],1.f} } };
				if (TriandBoxIntersect(tri, box) == true) {
					++triangleNum;
				}
			}
		}
	}

	if (nowLevel > MAX_LEVEL || includedObjIDs.size() == 0 || triangleNum < MIN_TRIANGLES) {
		OctreeObjectDataComponent tempnodeData{};
		for (auto& o : includedObjIDs) {
			const glm::mat4 Matrix = reg.get<ObjectMatrixComponent>(o).objectMatrix;
			MeshData& tempMeshData = scene->getMeshManager()->getMeshDataRef(reg.get<MeshIDComponent>(o).id);
			MeshData newMeshData{};
			int size{ (int)tempMeshData.idxDatas.size() };
			for (int i = 0; i < size; i += 3) {
				Triangle tri{
				glm::vec3{Matrix * glm::vec4{tempMeshData.VertexDatas[tempMeshData.idxDatas[i]],1.f} },
				glm::vec3{Matrix * glm::vec4{tempMeshData.VertexDatas[tempMeshData.idxDatas[i + 1]],1.f} } ,
				glm::vec3{Matrix * glm::vec4{tempMeshData.VertexDatas[tempMeshData.idxDatas[i + 2]],1.f} } };
				if (TriandBoxIntersect(tri, box) == true) {
					//Split mesh
					std::vector<Triangle> insideTriangle = GetInsideTriangleInBox(tri, BoundingBox{ center, halfExtend });
					for (auto& t : insideTriangle) {
						int nowidx{ (int)newMeshData.VertexDatas.size() };
						newMeshData.VertexDatas.push_back(t.v1);
						newMeshData.VertexDatas.push_back(t.v2);
						newMeshData.VertexDatas.push_back(t.v3);
						newMeshData.idxDatas.push_back(nowidx);
						newMeshData.idxDatas.push_back(nowidx + 1);
						newMeshData.idxDatas.push_back(nowidx + 2);
					}
				}
			}
			int meshID = scene->getMeshManager()->push_MeshData(newMeshData);
			tempnodeData.IDs.push_back({ meshID, o });
		}
		hierarchyContainer.emplace_or_replace<OctreeObjectDataComponent>(TreeNode, tempnodeData);
	}
	else {
		ID leaf1{ hierarchyContainer.create() };
		ID leaf2{ hierarchyContainer.create() };
		ID leaf3{ hierarchyContainer.create() };
		ID leaf4{ hierarchyContainer.create() };
		ID leaf5{ hierarchyContainer.create() };
		ID leaf6{ hierarchyContainer.create() };
		ID leaf7{ hierarchyContainer.create() };
		ID leaf8{ hierarchyContainer.create() };
		
		hierarchyContainer.emplace_or_replace<OctoLeafComponent>(TreeNode, leaf1, leaf2, leaf3, leaf4, leaf5, leaf6, leaf7, leaf8);
		glm::vec3 halfhalf{halfExtend/2.f};
		ComputeOctree(leaf1,includedObjIDs, center + glm::vec3{ halfhalf.x ,halfhalf.y,halfhalf.z }, halfhalf, nowLevel + 1);
		ComputeOctree(leaf2,includedObjIDs, center + glm::vec3{ -halfhalf.x ,halfhalf.y,halfhalf.z }, halfhalf, nowLevel + 1);
		ComputeOctree(leaf3,includedObjIDs, center + glm::vec3{ halfhalf.x ,-halfhalf.y,halfhalf.z }, halfhalf, nowLevel + 1);
		ComputeOctree(leaf4,includedObjIDs, center + glm::vec3{ -halfhalf.x ,-halfhalf.y,halfhalf.z }, halfhalf, nowLevel + 1);
		ComputeOctree(leaf5,includedObjIDs, center + glm::vec3{ halfhalf.x ,halfhalf.y,-halfhalf.z }, halfhalf, nowLevel + 1);
		ComputeOctree(leaf6,includedObjIDs, center + glm::vec3{ -halfhalf.x ,halfhalf.y,-halfhalf.z }, halfhalf, nowLevel + 1);
		ComputeOctree(leaf7,includedObjIDs, center + glm::vec3{ halfhalf.x ,-halfhalf.y,-halfhalf.z }, halfhalf, nowLevel + 1);
		ComputeOctree(leaf8,includedObjIDs, center + glm::vec3{ -halfhalf.x ,-halfhalf.y,-halfhalf.z }, halfhalf, nowLevel + 1);
	}
}

void SpatialManager::ComputeBSPtree(ID TreeNode, std::vector<SplitedMeshOBJ>& ObjIDs, glm::vec4 plane, int nowLevel, int Triangles)
{
	hierarchyContainer.emplace_or_replace<LevelComponent>(TreeNode, nowLevel);
	if (nowLevel > MAX_LEVEL || ObjIDs.size() == 0 || Triangles < MIN_TRIANGLES) {
		BSPtreeObjectDataComponent tempnodeData{};
		for (auto& o : ObjIDs) {
			int meshID{ scene->getMeshManager()->push_MeshData(o.splitedData) };
			tempnodeData.IDs.push_back({ meshID, o.objID });
		}
		hierarchyContainer.emplace_or_replace<BSPtreeObjectDataComponent>(TreeNode, tempnodeData);
		return;
	}

	hierarchyContainer.emplace_or_replace<PlaneComponent>(TreeNode, plane);

	//obj check
	std::vector<SplitedMeshOBJ> frontObjIDs{};
	std::vector<SplitedMeshOBJ> behindObjIDs{};
	//ENTT& reg = scene->getENTT();
	int frontTriangleNum{ 0 };
	int behindTriangleNum{ 0 };
	glm::vec3 frontCenter{};
	glm::vec3 behindCenter{};
	glm::mat4 IdentityMatrix = glm::scale(glm::mat4(1.0f), glm::vec3{ 1.f });
	for (auto& o : ObjIDs) {
		std::vector<MeshData> res{ PlaneMeshSplit(Plane{ plane }, IdentityMatrix, o.splitedData) };
		if (o.isSplited == false) {
			if (res[0].idxDatas.size() > 2 && res[1].idxDatas.size() > 2) {
				o.isSplited = true;
			}
		}
		if (res[0].idxDatas.size() > 2) {
			frontObjIDs.push_back({ o.objID,o.isSplited,res[0] });
			for (auto& point : res[0].idxDatas) {
				frontCenter += res[0].VertexDatas[point];
			}
			frontTriangleNum += (int)res[0].idxDatas.size() / 3;
		}
		if (res[1].idxDatas.size() > 2) {
			behindObjIDs.push_back({ o.objID,o.isSplited,res[1] });
			for (auto& point : res[1].idxDatas) {
				behindCenter += res[1].VertexDatas[point];
			}
			behindTriangleNum += (int)res[1].idxDatas.size() / 3;
		}
	}
	frontCenter /= frontTriangleNum * 3;
	behindCenter /= behindTriangleNum * 3;
	{
		//plane choose
		glm::vec3 fpn{ RanGenerator(-1.f,1.f),RanGenerator(-1.f,1.f) ,RanGenerator(-1.f,1.f) };
		fpn = glm::normalize(fpn);
		glm::vec3 bpn{ RanGenerator(-1.f,1.f),RanGenerator(-1.f,1.f) ,RanGenerator(-1.f,1.f) };
		bpn = glm::normalize(bpn);
		glm::vec4 fp{ fpn, glm::dot(fpn,frontCenter)};
		glm::vec4 bp{bpn, glm::dot(bpn,behindCenter) };
		ID front{ hierarchyContainer.create() };
		ID behind{ hierarchyContainer.create() };
		hierarchyContainer.emplace_or_replace<BSPLeafComponent>(TreeNode, front, behind);
		ComputeBSPtree(front, frontObjIDs, fp, nowLevel + 1, frontTriangleNum);
		ComputeBSPtree(behind, behindObjIDs, bp, nowLevel + 1, behindTriangleNum);
	}
}

SpatialManager::SpatialManager(Scene* sc): scene(sc)
{

}

SpatialManager::~SpatialManager()
{
	hierarchyContainer.clear();
}

void SpatialManager::BuildOctree()
{
	std::vector<ID> ObjIDs{};
	ENTT& reg = scene->getENTT();
	auto v = reg.view<ObjectMatrixComponent, BoundingBox>();
	glm::vec3 Min{ FLT_MAX };
	glm::vec3 Max{ -FLT_MAX };
	for (auto& e : v) {
		ObjIDs.push_back(e);
		glm::vec3 center = reg.get<ObjectMatrixComponent>(e).objectMatrix * glm::vec4(reg.get<BoundingBox>(e).Center, 1.f);
		glm::vec3 BVMin{ center - reg.get<BoundingBox>(e).HalfExtend };
		glm::vec3 BVMax{ center + reg.get<BoundingBox>(e).HalfExtend };

		Min.x = std::min(Min.x, BVMin.x);
		Min.y = std::min(Min.y, BVMin.y);
		Min.z = std::min(Min.z, BVMin.z);

		Max.x = std::max(Max.x, BVMax.x);
		Max.y = std::max(Max.y, BVMax.y);
		Max.z = std::max(Max.z, BVMax.z);
	}
	OctreeHeadNode = hierarchyContainer.create();

	glm::vec3 center{ (Min + Max) / 2.f };
	ComputeOctree(OctreeHeadNode, ObjIDs, center, Max - center, 0);

}
void SpatialManager::BuildBSPtree()
{
	std::vector<SplitedMeshOBJ> ObjIDs{};
	ENTT& reg = scene->getENTT();
	auto v = reg.view<ObjectMatrixComponent, BoundingBox>();
	glm::vec3 Min{ FLT_MAX };
	glm::vec3 Max{ -FLT_MAX };
	int triangles{};
	for (auto& e : v) {
		glm::mat4 Matrix = reg.get<ObjectMatrixComponent>(e).objectMatrix;
		ObjIDs.push_back({e,false, scene->getMeshManager()->getMeshDataRef(reg.get<MeshIDComponent>(e).id) });
		glm::vec3 center = reg.get<ObjectMatrixComponent>(e).objectMatrix * glm::vec4(reg.get<BoundingBox>(e).Center, 1.f);
		glm::vec3 BVMin{ center - reg.get<BoundingBox>(e).HalfExtend };
		glm::vec3 BVMax{ center + reg.get<BoundingBox>(e).HalfExtend };

		Min.x = std::min(Min.x, BVMin.x);
		Max.x = std::max(Max.x, BVMax.x);
		triangles += (int)scene->getMeshManager()->getMeshDataRef(reg.get<MeshIDComponent>(e).id).idxDatas.size()/3;
	}
	BSPtreeHeadNode = hierarchyContainer.create();

	glm::vec3 center{ (Min + Max) / 2.f };
	ComputeBSPtree(BSPtreeHeadNode, ObjIDs, glm::vec4{-1.f,0.f,0.f,-center.x}, 0, triangles);
}

void SpatialManager::Draw()
{
	glDisable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(0.5f);
	glm::mat4 WTC{ scene->get_WTC() };
	scene->get_DebugShader().get()->sendUniformMatrix4fv("WorldToCameraMat", WTC);

	//ENTT& reg = scene->getENTT();
	if (ShouldDraw_Octree) {
		auto v = hierarchyContainer.view<BoundingBox, LevelComponent>();
		const glm::mat4 IdentityMatrix = glm::scale(glm::mat4(1.0f), glm::vec3{ 1.f });
		for (auto& e : v) {
			int lv{ v.get<LevelComponent>(e).level };
			if (DrawAllLevel || DrawLevel == lv) {
				const glm::mat4 Translate_mat = glm::translate(glm::mat4(1.0f), v.get<BoundingBox>(e).Center);
				const glm::mat4 scale_mat = glm::scale(glm::mat4(1.0f), v.get<BoundingBox>(e).HalfExtend);
				const glm::mat4 Matrix = Translate_mat * scale_mat;

				const MeshData& temp = scene->getMeshManager()->getMeshData(v.get<BoundingBox>(e).DebugMeshID);

				scene->get_DebugShader().get()->sendUniformMatrix4fv("modelToWorldMat", Matrix);
				scene->get_DebugShader().get()->sendUniform4fv("u_color", glm::vec4(0.5f, 1.f, 0.5f, 1.f));
				scene->get_DebugShader().get()->useProgram();
				glBindVertexArray(temp.vao);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, temp.ibo);
				glDrawElements(temp.primitive_type, static_cast<GLsizei>(temp.idxDatas.size()), GL_UNSIGNED_INT, nullptr);
				scene->get_DebugShader().get()->unuseProgram();
			}
			if (ShouldDraw_DebugMesh && hierarchyContainer.any_of<OctreeObjectDataComponent>(e) == true) {
				scene->get_DebugShader().get()->sendUniform4fv("u_color", glm::vec4(levelColor(lv), 1.f));
				scene->get_DebugShader().get()->sendUniformMatrix4fv("modelToWorldMat", IdentityMatrix);
				scene->get_DebugShader().get()->useProgram();
				for (auto& m : hierarchyContainer.get<OctreeObjectDataComponent>(e).IDs) {
					const MeshData& tempMesh = scene->getMeshManager()->getMeshData(m.meshID);
					glBindVertexArray(tempMesh.vao);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tempMesh.ibo);
					glDrawElements(tempMesh.primitive_type, static_cast<GLsizei>(tempMesh.idxDatas.size()), GL_UNSIGNED_INT, nullptr);
				}
				scene->get_DebugShader().get()->unuseProgram();
			}
		}
	}
	if (ShouldDraw_BSPtree) {
		auto v = hierarchyContainer.view<BSPtreeObjectDataComponent, LevelComponent>();
		const glm::mat4 IdentityMatrix = glm::scale(glm::mat4(1.0f), glm::vec3{ 1.f });
		int ranCol{};
		for (auto& e : v) {
			//int lv{ v.get<LevelComponent>(e).level };
			if (ShouldDraw_DebugMesh) {
				scene->get_DebugShader().get()->sendUniform4fv("u_color", glm::vec4(levelColor(ranCol%MAX_LEVEL), 1.f));
				scene->get_DebugShader().get()->sendUniformMatrix4fv("modelToWorldMat", IdentityMatrix);
				scene->get_DebugShader().get()->useProgram();
				for (auto& m : hierarchyContainer.get<BSPtreeObjectDataComponent>(e).IDs) {
					const MeshData& tempMesh = scene->getMeshManager()->getMeshData(m.meshID);
					glBindVertexArray(tempMesh.vao);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tempMesh.ibo);
					glDrawElements(tempMesh.primitive_type, static_cast<GLsizei>(tempMesh.idxDatas.size()), GL_UNSIGNED_INT, nullptr);
				}
				scene->get_DebugShader().get()->unuseProgram();
			}++ranCol;
		}
	}

	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}


glm::vec3 levelColor(int level)
{
	glm::vec3 color{ 1.f };
	switch (level)
	{
	case 0: color = glm::vec3{ 1.f,0.f,0.f }; break;
	case 1: color = glm::vec3{ 0.f,0.f,1.f }; break;
	case 2: color = glm::vec3{ 0.f,1.f,0.f }; break;
	case 3: color = glm::vec3{ 1.f,1.f,0.f }; break;
	case 4: color = glm::vec3{ 1.f,0.f,1.f }; break;
	case 5: color = glm::vec3{ 0.f,1.f,1.f }; break;
	case 6: color = glm::vec3{ 1.f,0.5f,0.f }; break;
	case 7: color = glm::vec3{ 0.5f,5.f,1.f }; break;
	default:
		color = glm::vec3{ 1.f };
		break;
	}
	return color;
}