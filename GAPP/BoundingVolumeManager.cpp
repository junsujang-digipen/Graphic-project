/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: BoundingVolumeManager.cpp
Purpose: Bounding volume management and generate hierarchy
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 3 - Spatial Partitioning
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 03/04/2023
End Header --------------------------------------------------------*/
#include "pch.h"
#include "BoundingVolumeManager.h"
#include "EnttComponentStructures.h"
#include "Scene.h"
#include "glm/ext/matrix_transform.hpp"
#include "MeshData.h"
#include "MeshManager.h"
#include <GL/glew.h>
#include "ShaderManager.h"
#include "Shader.h"
#include "glm/gtx/string_cast.hpp"
#define MIN_OBJECTS_AT_LEAF 1


BoundingVolumeManager::BoundingVolumeManager(Scene* sc):scene(sc)
{
 
}

BoundingVolumeManager::~BoundingVolumeManager()
{
	hierarchyContainer.clear();
}

void BoundingVolumeManager::Update(double /*dt*/)
{
}

void BoundingVolumeManager::Draw()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(0.5f);
	glm::mat4 WTC{ scene->get_WTC() };
	scene->get_DebugShader().get()->sendUniformMatrix4fv("WorldToCameraMat", WTC);

	ENTT& reg = scene->getENTT();
	if(ShouldDrawBoundingBox){
		auto v = reg.view<ObjectMatrixComponent, BoundingBox>();
		for (auto& e : v) {
			const glm::mat4 Translate_mat = glm::translate(glm::mat4(1.0f), v.get<BoundingBox>(e).Center);
			const glm::mat4 scale_mat = glm::scale(glm::mat4(1.0f), v.get<BoundingBox>(e).HalfExtend);
			//ObjectMatrixComponent& t = v.get<ObjectMatrixComponent>(e);
			const glm::mat4 Matrix = v.get<ObjectMatrixComponent>(e).objectMatrix * Translate_mat * scale_mat;

			const MeshData& temp = scene->getMeshManager()->getMeshData(v.get<BoundingBox>(e).DebugMeshID);

			scene->get_DebugShader().get()->sendUniformMatrix4fv("modelToWorldMat", Matrix);
			scene->get_DebugShader().get()->sendUniform4fv("u_color", glm::vec4(levelColor(7), 1.f));
			scene->get_DebugShader().get()->useProgram();
			glBindVertexArray(temp.vao);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, temp.ibo);
			glDrawElements(temp.primitive_type, static_cast<GLsizei>(temp.idxDatas.size()), GL_UNSIGNED_INT, nullptr);
			scene->get_DebugShader().get()->unuseProgram();
		}
	
	}
	if (ShouldDrawBoundingSphere) {
		auto g = reg.view<ObjectMatrixComponent, BoundingSphere>();
		for (auto& e : g) {
			const glm::mat4 Translate_mat = glm::translate(glm::mat4(1.0f), g.get<BoundingSphere>(e).Center);
			const glm::mat4 scale_mat = glm::scale(glm::mat4(1.0f), glm::vec3(g.get<BoundingSphere>(e).radius*2.f));
			const glm::mat4 Matrix = g.get<ObjectMatrixComponent>(e).objectMatrix * Translate_mat * scale_mat;
			
			//std::cout << glm::to_string(g.get<ObjectMatrixComponent>(e).objectMatrix)<<std::endl;
			const MeshData& temp = scene->getMeshManager()->getMeshData(g.get<BoundingSphere>(e).DebugMeshID);
			
			scene->get_DebugShader().get()->sendUniformMatrix4fv("modelToWorldMat", Matrix);
			scene->get_DebugShader().get()->sendUniform4fv("u_color", glm::vec4(levelColor(7), 1.f));
			scene->get_DebugShader().get()->useProgram();
			glBindVertexArray(temp.vao);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, temp.ibo);
			glDrawElements(temp.primitive_type, static_cast<GLsizei>(temp.idxDatas.size()), GL_UNSIGNED_INT, nullptr);
			scene->get_DebugShader().get()->unuseProgram();
		}
	}
	if (ShouldDraw_BU_BoundingBoxHierarchy || ShouldDraw_TD_BoundingBoxHierarchy) {
		auto v = hierarchyContainer.view<BoundingBox, BVHierarchyType, LevelComponent>();
		for (auto& e : v) {
			if (DrawAllLevel || DrawLevel == v.get<LevelComponent>(e).level) {
				if (ShouldDraw_BU_BoundingBoxHierarchy && v.get<BVHierarchyType>(e) == BVHierarchyType::BOTTOM_UP
					|| ShouldDraw_TD_BoundingBoxHierarchy && v.get<BVHierarchyType>(e) == BVHierarchyType::TOP_DOWN) {
					const glm::mat4 Translate_mat = glm::translate(glm::mat4(1.0f), v.get<BoundingBox>(e).Center);
					const glm::mat4 scale_mat = glm::scale(glm::mat4(1.0f), v.get<BoundingBox>(e).HalfExtend);
					const glm::mat4 Matrix = Translate_mat * scale_mat;

					const MeshData& temp = scene->getMeshManager()->getMeshData(v.get<BoundingBox>(e).DebugMeshID);

					scene->get_DebugShader().get()->sendUniformMatrix4fv("modelToWorldMat", Matrix);
					scene->get_DebugShader().get()->sendUniform4fv("u_color", glm::vec4(levelColor(v.get<LevelComponent>(e).level), 1.f));
					scene->get_DebugShader().get()->useProgram();
					glBindVertexArray(temp.vao);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, temp.ibo);
					glDrawElements(temp.primitive_type, static_cast<GLsizei>(temp.idxDatas.size()), GL_UNSIGNED_INT, nullptr);
					scene->get_DebugShader().get()->unuseProgram();
				}
			}
		}
	}
	if (ShouldDraw_BU_BoundingSphereHierarchy || ShouldDraw_TD_BoundingSphereHierarchy) {
		auto v = hierarchyContainer.view<BoundingSphere, BVHierarchyType, LevelComponent>();
		for (auto& e : v) {
			if (DrawAllLevel || DrawLevel == v.get<LevelComponent>(e).level) {
				if (ShouldDraw_BU_BoundingSphereHierarchy && v.get<BVHierarchyType>(e) == BVHierarchyType::BOTTOM_UP
					|| ShouldDraw_TD_BoundingSphereHierarchy && v.get<BVHierarchyType>(e) == BVHierarchyType::TOP_DOWN) {
					const glm::mat4 Translate_mat = glm::translate(glm::mat4(1.0f), v.get<BoundingSphere>(e).Center);
					const glm::mat4 scale_mat = glm::scale(glm::mat4(1.0f), glm::vec3(v.get<BoundingSphere>(e).radius * 2.f));
					const glm::mat4 Matrix = Translate_mat * scale_mat;

					const MeshData& temp = scene->getMeshManager()->getMeshData(v.get<BoundingSphere>(e).DebugMeshID);

					scene->get_DebugShader().get()->sendUniformMatrix4fv("modelToWorldMat", Matrix);
					scene->get_DebugShader().get()->sendUniform4fv("u_color", glm::vec4(levelColor(v.get<LevelComponent>(e).level), 1.f));
					scene->get_DebugShader().get()->useProgram();
					glBindVertexArray(temp.vao);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, temp.ibo);
					glDrawElements(temp.primitive_type, static_cast<GLsizei>(temp.idxDatas.size()), GL_UNSIGNED_INT, nullptr);
					scene->get_DebugShader().get()->unuseProgram();
				}
			}
		}
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void BoundingVolumeManager::calcHierarchy()
{
	std::vector<ID> ObjIDs{};
	ENTT& reg = scene->getENTT();
	auto v = reg.view<PositionComponent, BoundingBox>();
	for (auto& e : v) {
		ObjIDs.push_back(e);
	}
	BUHierarchy = calc_BU_Hierarchy(ObjIDs);
	TDBox = hierarchyContainer.create();
	TDSphere = hierarchyContainer.create();
	calc_TD_Hierarchy(TDBox, ObjIDs, 0, static_cast<int>(ObjIDs.size()), 0);
	calc_TD_SphereHierarchy(TDSphere, ObjIDs, 0, static_cast<int>(ObjIDs.size()), 0);
}

void BoundingVolumeManager::findNodesToMerge(std::vector<ID>& HierarchyVolumes, ID &leftNode, ID &rightNode) // TODO
{
	std::pair<float, std::pair<ID, ID>> C_pair = closestPair(HierarchyVolumes);
	leftNode = C_pair.second.first;
	rightNode = C_pair.second.second;
}

void BoundingVolumeManager::mergeBoxNodes(ID TreeNode, std::vector<ID> HierarchyVolumes)
{
	glm::vec3 Min{ FLT_MAX };
	glm::vec3 Max{ -FLT_MAX };
	for (auto& id:HierarchyVolumes) {
		glm::vec3 center =  glm::vec3(hierarchyContainer.get<BoundingBox>(id).Center);
		glm::vec3 BVMin{ center - hierarchyContainer.get<BoundingBox>(id).HalfExtend };
		glm::vec3 BVMax{ center + hierarchyContainer.get<BoundingBox>(id).HalfExtend };

		Min.x = std::min(Min.x, BVMin.x);
		Min.y = std::min(Min.y, BVMin.y);
		Min.z = std::min(Min.z, BVMin.z);

		Max.x = std::max(Max.x, BVMax.x);
		Max.y = std::max(Max.y, BVMax.y);
		Max.z = std::max(Max.z, BVMax.z);
		
	}
	glm::vec3 center{ (Min + Max) / 2.f };
	hierarchyContainer.emplace<BoundingBox>(TreeNode, center, Max - center,  Cast(BasicMeshType::Box));
}

void BoundingVolumeManager::mergeCircleNodes(ID TreeNode, std::vector<ID> HierarchyVolumes)
{
	glm::vec3 center{};
	for (auto& id : HierarchyVolumes) {
		center += hierarchyContainer.get<BoundingSphere>(id).Center;
	}
	center /= HierarchyVolumes.size();
	float rad{};
	for (auto& id : HierarchyVolumes) {
		glm::vec3 direc{ glm::normalize(hierarchyContainer.get<BoundingSphere>(id).Center - center) };
		glm::vec3 point{ hierarchyContainer.get<BoundingSphere>(id).Center + hierarchyContainer.get<BoundingSphere>(id).radius * direc };
		std::vector<glm::vec3> vec{ point };
		CircleGrow(center, rad, vec);
	}

	hierarchyContainer.emplace<BoundingSphere>(TreeNode, center, rad, Cast(BasicMeshType::Sphere));
}

int BoundingVolumeManager::partitioning(std::vector<ID>& ObjIDs, int stIndex, int numObjects)//TODO
{
	ENTT& reg = scene->getENTT();
	glm::vec3 Min{ FLT_MAX };
	glm::vec3 Max{ -FLT_MAX };
	glm::vec3 PosCenter{};
	for (int i = 0; i < numObjects; ++i) {
		ID id = ObjIDs[stIndex+i];
		glm::vec3 center = reg.get<ObjectMatrixComponent>(id).objectMatrix * glm::vec4(reg.get<BoundingBox>(id).Center, 1.f);
		glm::vec3 BVMin{ center - reg.get<BoundingBox>(id).HalfExtend };
		glm::vec3 BVMax{ center + reg.get<BoundingBox>(id).HalfExtend };
		PosCenter += center;

		Min.x = std::min(Min.x, BVMin.x);
		Min.y = std::min(Min.y, BVMin.y);
		Min.z = std::min(Min.z, BVMin.z);

		Max.x = std::max(Max.x, BVMax.x);
		Max.y = std::max(Max.y, BVMax.y);
		Max.z = std::max(Max.z, BVMax.z);
	}
	glm::vec3 center{ (Min + Max) / 2.f };
	PosCenter /= numObjects;

	glm::vec3 var{};
	for (int i = 0; i < numObjects; ++i) {
		ID id = ObjIDs[stIndex + i];
		glm::vec3 objPos = reg.get<ObjectMatrixComponent>(id).objectMatrix * glm::vec4(reg.get<BoundingBox>(id).Center, 1.f);
		var += (objPos - center) * (objPos - center);
	}
	var /= numObjects;
	glm::vec3 axis{1.f,0.f,0.f};
	float maxVal{var.x};
	if (maxVal < var.y) {
		axis = { 0.f,1.f,0.f };
		maxVal = var.y;
	}
	if (glm::dot(axis, glm::vec3{ maxVal }) < var.z) {
		axis = { 0.f,0.f,1.f };
	}
	std::sort(ObjIDs.begin() + stIndex, ObjIDs.begin() + stIndex + numObjects, [&](const ID&a,const ID&b)->bool {
		ENTT& reg = scene->getENTT();
		glm::vec3 A = reg.get<ObjectMatrixComponent>(a).objectMatrix * glm::vec4(reg.get<BoundingBox>(a).Center, 1.f);
		glm::vec3 B = reg.get<ObjectMatrixComponent>(b).objectMatrix * glm::vec4(reg.get<BoundingBox>(b).Center, 1.f);
		return glm::dot(axis,A) < glm::dot(axis, B);
		});
	return numObjects/2;
}

ID BoundingVolumeManager::calc_BU_Hierarchy( std::vector<ID> ObjIDs)
{
	std::vector<ID> BVs{};
	ENTT& reg = scene->getENTT();
	for (auto& o:ObjIDs) {
		glm::vec3 center = reg.get<PositionComponent>(o).pos + (reg.get<BoundingBox>(o).Center);
		glm::vec3 HalfExtend = reg.get<ScaleComponent>(o).scale * (reg.get<BoundingBox>(o).HalfExtend);
		ID tempID = hierarchyContainer.create();
		hierarchyContainer.emplace<BoundingBox>(tempID, center, HalfExtend, Cast(BasicMeshType::Box));
		center = reg.get<PositionComponent>(o).pos + (reg.get<BoundingSphere>(o).Center);
		float maxScal{ std::max(reg.get<ScaleComponent>(o).scale.x,reg.get<ScaleComponent>(o).scale.y)};
		maxScal = std::max(maxScal, reg.get<ScaleComponent>(o).scale.z);
		float radius{ maxScal * reg.get<BoundingSphere>(o).radius };
		hierarchyContainer.emplace<BoundingSphere>(tempID, center, radius, Cast(BasicMeshType::Sphere));
		hierarchyContainer.emplace<LevelComponent>(tempID, 0);
		hierarchyContainer.emplace<BVHierarchyType>(tempID, BVHierarchyType::BOTTOM_UP);
		BVs.push_back(tempID);
	}

	while (BVs.size() > 1) {
		ID left{  };
		ID right{ };
		findNodesToMerge(BVs, left,right);
		BVs.erase(std::find(BVs.begin(), BVs.end(), left));
		BVs.erase(std::find(BVs.begin(), BVs.end(), right));
		ID parent{hierarchyContainer.create()};
		BVs.push_back(parent);
		mergeBoxNodes(parent, std::vector<ID>{left,right});
		mergeCircleNodes(parent, std::vector<ID>{left, right});
		hierarchyContainer.emplace_or_replace<LevelComponent>(parent, 0);
		hierarchyContainer.emplace_or_replace<BVHierarchyType>(parent, BVHierarchyType::BOTTOM_UP);
		hierarchyContainer.emplace_or_replace<LeafComponent>(parent, left, right);
	}
	BU_LevelSet(BVs[0],0);
	return BVs[0];
}

void BoundingVolumeManager::calc_TD_Hierarchy(ID TreeNode, std::vector<ID>& ObjIDs, int stIndex, int numObjects, int nowLevel)
{
	//hierarchyContainer.emplace_or_replace<IncludeBVComponent>(TreeNode, ObjIDs);
	hierarchyContainer.emplace_or_replace<LevelComponent>(TreeNode, nowLevel);
	hierarchyContainer.emplace_or_replace<BVHierarchyType>(TreeNode, BVHierarchyType::TOP_DOWN);

	computeBoundingBox(TreeNode, ObjIDs, stIndex, numObjects);
	if (numObjects <= MIN_OBJECTS_AT_LEAF) {
		//TODO
	}
	else {
		int newN = partitioning(ObjIDs, stIndex, numObjects);
		ID left{ hierarchyContainer.create() };
		ID right{ hierarchyContainer.create() };
		hierarchyContainer.emplace_or_replace<LeafComponent>(TreeNode, left, right);
		calc_TD_Hierarchy(left, ObjIDs, stIndex, newN, nowLevel + 1);
		calc_TD_Hierarchy(right, ObjIDs, stIndex + newN, numObjects - newN, nowLevel + 1);
	}
}

void BoundingVolumeManager::calc_TD_SphereHierarchy(ID TreeNode, std::vector<ID>& ObjIDs, int stIndex, int numObjects, int nowLevel)
{
	hierarchyContainer.emplace_or_replace<LevelComponent>(TreeNode, nowLevel);
	hierarchyContainer.emplace_or_replace<BVHierarchyType>(TreeNode, BVHierarchyType::TOP_DOWN);

	computeBoundingSphere(TreeNode, ObjIDs, stIndex, numObjects);
	if (numObjects <= MIN_OBJECTS_AT_LEAF) {
		//TODO
	}
	else {
		int newN = partitioning(ObjIDs, stIndex, numObjects);
		ID left{ hierarchyContainer.create() };
		ID right{ hierarchyContainer.create() };
		hierarchyContainer.emplace_or_replace<LeafComponent>(TreeNode, left, right);
		calc_TD_SphereHierarchy(left, ObjIDs, stIndex, newN, nowLevel + 1);
		calc_TD_SphereHierarchy(right, ObjIDs, stIndex + newN, numObjects - newN, nowLevel + 1);
	}
}

void BoundingVolumeManager::computeBoundingBox(ID TreeNode, std::vector<ID>& ObjIDs, int stIndex, int numObjects)
{
	//compute
	ENTT& reg = scene->getENTT();
	glm::vec3 Min{ FLT_MAX };
	glm::vec3 Max{ -FLT_MAX };
	
	for (int i = 0; i < numObjects; ++i) {
		ID id = ObjIDs[stIndex + i];
		glm::vec3 center = reg.get<ObjectMatrixComponent>(id).objectMatrix * glm::vec4(reg.get<BoundingBox>(id).Center, 1.f);
		glm::vec3 BVMin{ center - reg.get<BoundingBox>(id).HalfExtend };
		glm::vec3 BVMax{ center + reg.get<BoundingBox>(id).HalfExtend};

		Min.x = std::min(Min.x, BVMin.x);
		Min.y = std::min(Min.y, BVMin.y);
		Min.z = std::min(Min.z, BVMin.z);

		Max.x = std::max(Max.x, BVMax.x);
		Max.y = std::max(Max.y, BVMax.y);
		Max.z = std::max(Max.z, BVMax.z);
	}

	glm::vec3 center{ (Min + Max)/2.f};
	hierarchyContainer.emplace<BoundingBox>(TreeNode, center, Max - center, Cast(BasicMeshType::Box));
}

void BoundingVolumeManager::computeBoundingSphere(ID TreeNode, std::vector<ID>& ObjIDs, int stIndex, int numObjects)//TODO
{
	ENTT& reg = scene->getENTT();
	glm::vec3 center{};
	for (int i = 0; i < numObjects; ++i) {
		ID id = ObjIDs[stIndex + i];
		center += glm::vec3(reg.get<ObjectMatrixComponent>(id).objectMatrix * glm::vec4(reg.get<BoundingBox>(id).Center, 1.f));
	}
	center /= ObjIDs.size();
	float rad{};
	for (int i = 0; i < numObjects; ++i) {
		ID id = ObjIDs[stIndex + i];
		glm::vec3 pos{glm::vec3(reg.get<ObjectMatrixComponent>(id).objectMatrix * glm::vec4(reg.get<BoundingBox>(id).Center, 1.f)) };
		glm::vec3 direc{ glm::normalize(pos - center) };
		glm::vec3 point{ pos + reg.get<ScaleComponent>(id).scale.x * reg.get<BoundingSphere>(id).radius * direc };
		std::vector<glm::vec3> vec{ point };
		CircleGrow(center, rad, vec);
	}
	hierarchyContainer.emplace<BoundingSphere>(TreeNode, center, rad, Cast(BasicMeshType::Sphere));
}

glm::vec3 BoundingVolumeManager::levelColor(int level)
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
		color= glm::vec3{ 1.f };
		break;
	}
	return color;
}

float BoundingVolumeManager::GetDistanceSquare(const ID& a, const ID& b){
	BoundingBox& A{ hierarchyContainer.get<BoundingBox>(a) };
	BoundingBox& B{ hierarchyContainer.get<BoundingBox>(b) };
	return (A.Center.x - B.Center.x) * (A.Center.x - B.Center.x)
		+ (A.Center.y - B.Center.y) * (A.Center.y - B.Center.y)
		+ (A.Center.z - B.Center.z) * (A.Center.z - B.Center.z);
}

std::pair<float,std::pair<ID,ID>> BoundingVolumeManager::closestPair ( std::vector< ID > const& points ) {
	int size = static_cast<int>(points.size());

	if (size==0) throw "zero size subset";
	if (size==1) return std::make_pair( std::numeric_limits<float>::max(), std::make_pair(points[0], points[0]));
	std::vector< ID > indices(points);
	std::sort(indices.begin(), indices.end(), [&](const ID& a, const ID& b)->bool {
		return hierarchyContainer.get<BoundingBox>(a).Center.x < hierarchyContainer.get<BoundingBox>(b).Center.x;
		});

	std::pair<float, std::pair<ID, ID>> returnVal{ closestPair_aux(indices,0,size) };
	returnVal.first = sqrt(returnVal.first);
	return returnVal;
}

void BoundingVolumeManager::BU_LevelSet(ID treeNode, int level)
{
	hierarchyContainer.get<LevelComponent>(treeNode).level = level;
	if (hierarchyContainer.any_of<LeafComponent>(treeNode) == false) {
		return;
	}
	LeafComponent& leaf = hierarchyContainer.get<LeafComponent>(treeNode);
	BU_LevelSet(leaf.left, level + 1);
	BU_LevelSet(leaf.right, level + 1);
}

std::pair<float, std::pair<ID, ID>> BoundingVolumeManager::closestPair_aux ( std::vector< ID > const& indices, int start, int n ) {
	float min_dist=FLT_MAX;
	ENTT& reg = hierarchyContainer;
	if(n == 1){
		return std::make_pair(min_dist, std::make_pair(indices[0], indices[0])); 
	}
	if(n == 2){
		min_dist = GetDistanceSquare(indices[start], indices[start + 1]);
		return std::make_pair(min_dist, std::make_pair(indices[start], indices[start + 1]));
	}
	if(n == 3){
		std::pair<float, std::pair<ID, ID>> returnVal{ 0.f, {indices[start],indices[start + 1]} };
		returnVal.first = GetDistanceSquare(indices[start],indices[start+1]);
		min_dist = std::min(returnVal.first,GetDistanceSquare(indices[start+1],indices[start+2]));
		if (returnVal.first > min_dist) {
			returnVal.first = min_dist;
			returnVal.second = { indices[start + 1],indices[start + 2] };
		}
		min_dist = std::min(returnVal.first,GetDistanceSquare(indices[start],indices[start+2]));
		if (returnVal.first > min_dist) {
			returnVal.first = min_dist;
			returnVal.second = { indices[start + 1],indices[start + 2] };
		}
		return returnVal;
	}

	int leftN = n/2;
	int rightN = n - leftN;
	int right = start + leftN;
	std::pair<float, std::pair<ID, ID>> leftMin = closestPair_aux(indices,start,leftN);
	std::pair<float, std::pair<ID, ID>> rightMin = closestPair_aux(indices,right,rightN);
	min_dist = std::min(leftMin.first,rightMin.first);
	std::pair<float, std::pair<ID, ID>> returnVal{};
	if (leftMin.first < rightMin.first) {
		returnVal = leftMin;
	}
	else {
		returnVal = rightMin;
	}
	float center = (reg.get<BoundingBox>(indices[right]).Center.x + reg.get<BoundingBox>(indices[right-1]).Center.x)/2.f;
	float sqrtMin = sqrt(returnVal.first);
	std::pair<float, std::pair<ID, ID>> intervalMin{ findIntervalMin(indices, start,right,leftN,rightN,center - sqrtMin,center + sqrtMin ,returnVal.first) };
	if (returnVal.first > intervalMin.first) {
		returnVal = intervalMin;
	}
	return returnVal;
}

std::pair<float, std::pair<ID, ID>> BoundingVolumeManager::findIntervalMin( std::vector< ID > const& indices, const int left,const int right,const int leftN,const int rightN,const  float leftInterval,const float rightInterval ,float minDist){
	ENTT& reg = hierarchyContainer;
	std::pair<float, std::pair<ID, ID>> returnVal{ minDist, {} };
	for(int i=0;i<leftN; ++i){
		if(reg.get<BoundingBox>(indices[left + i]).Center.x < leftInterval ){
			continue;
		}else{
			for(int j = 0 ;j < rightN;++j){
				if(reg.get<BoundingBox>(indices[right + j]).Center.x > rightInterval ){
					continue;
				}else{
					float newDist{ GetDistanceSquare(indices[left + i],indices[right + j]) };
					if (returnVal.first > newDist) {
						returnVal = { newDist , {indices[left + i],indices[right + j]} };
					}
				}
			}
		}
	}
	return returnVal;
}