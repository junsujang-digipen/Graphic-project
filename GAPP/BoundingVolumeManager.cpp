/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: BoundingVolumeManager.cpp
Purpose: Bounding volume management and generate hierarchy
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 1 - Hybrid Rendering
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
BoundingVolumeManager::BoundingVolumeManager(Scene* sc):scene(sc)
{
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
			scene->get_DebugShader().get()->useProgram();
			glBindVertexArray(temp.vao);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, temp.ibo);
			glDrawElements(temp.primitive_type, static_cast<GLsizei>(temp.idxDatas.size()), GL_UNSIGNED_INT, nullptr);
			scene->get_DebugShader().get()->unuseProgram();
		}
	}
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
