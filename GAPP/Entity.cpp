/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Entity.cpp
Purpose: For storing Entity datas
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 1 - Hybrid Rendering
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 09/30/2022
End Header --------------------------------------------------------*/
#include "pch.h"
#include "Entity.h"
#include "glm/ext/matrix_transform.hpp"
#include "Renderer.h"
#include "Shader.h"
#include "OBJLoader.h"
#include <GL/glew.h>
#include "Scene.h"
#include "MeshManager.h"

Entity::Entity(Scene* sc):scene(sc)
{
}

Entity::Entity(Scene* sc, ID id):scene(sc), thisID(id)
{
}

ID Entity::getID()
{
	return thisID;
}

ENTT& Entity::getENTT()
{
	return scene->getENTT();
}

void Entity::objMatrixUpdate()
{
	const glm::mat4 Translate_mat = glm::translate(glm::mat4(1.0f), pos);
	const glm::mat4 scale_mat = glm::scale(glm::mat4(1.0f), scale);
	const glm::mat4 rotate_x = glm::rotate(glm::mat4(1.0f), rotate.x, glm::vec3{ 1.f,0.f,0.f });
	const glm::mat4 rotate_y = glm::rotate(glm::mat4(1.0f), rotate.y, glm::vec3{ 0.f,1.f,0.f });
	const glm::mat4 rotate_z = glm::rotate(glm::mat4(1.0f), rotate.z, glm::vec3{ 0.f,0.f,1.f });
	const glm::mat4 rotate_mat = rotate_x * rotate_y * rotate_z;
	objectMatrix = Translate_mat * rotate_mat * scale_mat;
}

void Entity::setPos(const glm::vec3 &p)
{
	pos = p;
	shouldOBJMatrixUpdate = true;
}

void Entity::setScale(const glm::vec3 &s)
{
	scale = s;
	shouldOBJMatrixUpdate = true;
}

void Entity::setRotate(const glm::vec3 &r)
{
	rotate = r;
	shouldOBJMatrixUpdate = true;
}


void Entity::update(double /*dt*/)
{
	if (shouldOBJMatrixUpdate == true) {
		objMatrixUpdate();
		shouldOBJMatrixUpdate = false;
	}
}

void Entity::draw()
{
	const MeshData& temp = scene->getMeshManager()->getMeshData(MeshID);

	objShader->sendUniformMatrix4fv("modelToWorldMat", objectMatrix);

	objShader->useProgram();
	glBindVertexArray(temp.vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, temp.ibo);
	glLineWidth(1.f);
	glDrawElements(temp.primitive_type, static_cast<GLsizei>(temp.idxDatas.size()), GL_UNSIGNED_INT, nullptr);
	objShader->unuseProgram();
}

void Entity::drawNormal(int num)
{
	const MeshData& temp = scene->getMeshManager()->getMeshData(MeshID);

	normalVectorShader->sendUniformMatrix4fv("modelToWorldMat", objectMatrix);

	normalVectorShader->useProgram();
	glBindVertexArray(temp.vao);
	glLineWidth(1.f);
	Renderer renderer{};
	if (num == 1 or num == 3) {
		renderer.BufferData(GL_ARRAY_BUFFER, temp.vboForLine, temp.VertexNormalDrawVec);
		glDrawArrays(GL_LINES, 0, (GLsizei)(temp.VertexNormalDrawVec.size()));
	}
	if (num == 2 or num == 3) {
		renderer.BufferData(GL_ARRAY_BUFFER, temp.vboForLine, temp.FaceNormalDrawVec);
		glDrawArrays(GL_LINES, 0, (GLsizei)(temp.FaceNormalDrawVec.size()));
	}
	normalVectorShader->unuseProgram();
}

