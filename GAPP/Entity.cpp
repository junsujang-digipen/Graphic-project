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

void Entity::GetDataForOBJLoader(OBJLoader& objLoader)
{
	VertexDatas = objLoader.VertexDatas;
	FaceNormalDatas = objLoader.FaceNormalDatas;
	VertexNormalDatas = objLoader.VertexNormalDatas;
	VertexTextureDatas = objLoader.VertexTextureDatas;
	idxDatas = objLoader.idxDatas;
	primitive_type = objLoader.primitive_type;
	boundBoxMax = objLoader.boundBoxMax;
	boundBoxMin = objLoader.boundBoxMin;

	makeNormalDrawVec(objLoader.VertexDatas, objLoader.VertexNormalDatas, VertexNormalDrawVec,scale, 2.f);
	if (objLoader.FaceNormalDatas.size() > 0)
	{
		std::vector<glm::vec3> vertexArrayData{ objLoader.VertexArrayData() };
		makeFaceNormalDrawVec(vertexArrayData, objLoader.FaceNormalDatas, FaceNormalDrawVec, scale, 1.f);
	}
}

void Entity::load()
{
	objMatrixUpdate();

	Renderer renderer{};
	renderer.genVertexArray(&vao);
	//Buffers
	renderer.genBuffer(&vbo);
	renderer.BufferData(GL_ARRAY_BUFFER, vbo, VertexDatas);
	renderer.PointBufferGlmVec3(vao, vbo, 0);
	//vertex normals
	renderer.genBuffer(&vno);
	renderer.BufferData(GL_ARRAY_BUFFER, vno, VertexNormalDatas);
	renderer.PointBufferGlmVec3(vao, vno, 1);
	//normal draw vectors
	renderer.genBuffer(&vboForLine);
	renderer.BufferData(GL_ARRAY_BUFFER, vboForLine, VertexNormalDrawVec);
	renderer.PointBufferGlmVec3(vao, vboForLine, 2);
	//index
	renderer.genBuffer(&ibo);
	renderer.BufferData(GL_ELEMENT_ARRAY_BUFFER, ibo, idxDatas);
	//UV
	VertexUVDatas.resize(VertexDatas.size());
	renderer.genBuffer(&uvbo);
	renderer.BufferData(GL_ARRAY_BUFFER, uvbo, VertexUVDatas);
	renderer.PointBufferGlmVec2(vao, uvbo, 3);

}

void Entity::unload()
{
	Renderer renderer{};
	renderer.deleteVertexArray(&vao);
	renderer.deleteBuffer(GL_ARRAY_BUFFER, &vbo);
	renderer.deleteBuffer(GL_ARRAY_BUFFER, &vno);
	renderer.deleteBuffer(GL_ARRAY_BUFFER, &vboForLine);
	renderer.deleteBuffer(GL_ELEMENT_ARRAY_BUFFER, &ibo);
	renderer.deleteBuffer(GL_ARRAY_BUFFER, &uvbo);
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
	objShader->sendUniformMatrix4fv("modelToWorldMat", objectMatrix);

	objShader->useProgram();
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glLineWidth(1.f);
	glDrawElements(primitive_type, static_cast<GLsizei>(idxDatas.size()), GL_UNSIGNED_INT, nullptr);
	objShader->unuseProgram();
}

void Entity::drawNormal(int num)
{
	normalVectorShader->sendUniformMatrix4fv("modelToWorldMat", objectMatrix);

	normalVectorShader->useProgram();
	glBindVertexArray(vao);
	glLineWidth(1.f);
	Renderer renderer{};
	if (num == 1 or num == 3) {
		renderer.BufferData(GL_ARRAY_BUFFER, vboForLine, VertexNormalDrawVec);
		glDrawArrays(GL_LINES, 0, (GLsizei)(VertexNormalDrawVec.size()));
	}
	if (num == 2 or num == 3) {
		renderer.BufferData(GL_ARRAY_BUFFER, vboForLine, FaceNormalDrawVec);
		glDrawArrays(GL_LINES, 0, (GLsizei)(FaceNormalDrawVec.size()));
	}
	normalVectorShader->unuseProgram();
}

void Entity::calcSphereTexCoord()
{
	glm::vec3 center = (boundBoxMin + boundBoxMax) / 2.f;
	for (int i = 0; i < VertexDatas.size();++i) {
		glm::vec3 vpos{};
		if (IsPositionEntity == true) {
			vpos = VertexDatas[i];
		}
		else {
			vpos = VertexNormalDatas[i];
		}
		glm::vec3 alignedPoint = vpos - center;
		float theta = atan(alignedPoint.z/alignedPoint.x);
		float alignedMiny = boundBoxMin.y - center.y;
		float alignedMaxy = boundBoxMax.y - center.y;
		float y = (alignedPoint.y - alignedMiny) / (alignedMaxy - alignedMiny);
		VertexUVDatas[i] = { theta / 6.14f, y };
	}
	Renderer renderer{};
	renderer.BufferData(GL_ARRAY_BUFFER, uvbo, VertexUVDatas);
}

void Entity::calcCylindricalTexCoord()
{
	glm::vec3 center = (boundBoxMin + boundBoxMax) / 2.f;
	for (int i = 0; i < VertexDatas.size(); ++i) {
		glm::vec3 vpos{};
		if (IsPositionEntity == true) {
			vpos = VertexDatas[i];
		}
		else {
			vpos = VertexNormalDatas[i];
		}
		glm::vec3 alignedPoint = vpos - center;
		float theta = atan(alignedPoint.z / alignedPoint.x);
		float r = sqrt(alignedPoint.x * alignedPoint.x + alignedPoint.y * alignedPoint.y + alignedPoint.z * alignedPoint.z);
		float latitude = acos(alignedPoint.y / r);
		VertexUVDatas[i] = glm::vec2(theta / 6.14f, 3.14f - latitude / 3.14f);
	}
	Renderer renderer{};
	renderer.BufferData(GL_ARRAY_BUFFER, uvbo, VertexUVDatas);
}

void Entity::calcPlanarTexCoord()
{
	glm::vec3 center = (boundBoxMin + boundBoxMax) / 2.f;
	glm::vec3 alignedPointMin = boundBoxMin - center;
	glm::vec3 alignedPointMax = boundBoxMax - center;
	for (int i = 0; i < VertexDatas.size(); ++i) {
		glm::vec3 vpos{};
		if (IsPositionEntity == true) {
			vpos = VertexDatas[i];
		}
		else {
			vpos = VertexNormalDatas[i];
		}
		glm::vec3 alignedPoint = vpos - center;
		VertexUVDatas[i] = (alignedPoint - alignedPointMin) / (alignedPointMax - alignedPointMin);
	}
	Renderer renderer{};
	renderer.BufferData(GL_ARRAY_BUFFER, uvbo, VertexUVDatas);
}

void Entity::calcCubeMapTexCoord()
{
	glm::vec3 center = (boundBoxMin + boundBoxMax) / 2.f;
	for (int i = 0; i < VertexDatas.size(); ++i) {
		glm::vec3 vpos{};
		if (IsPositionEntity == true) {
			vpos = VertexDatas[i];
		}
		else {
			vpos = VertexNormalDatas[i];
		}
		glm::vec3 alignedPoint = vpos - center;
		glm::vec3 absAlignedP = abs(alignedPoint);
		bool isXPositive = alignedPoint.x >= 0;
		bool isYPositive = alignedPoint.y >= 0;
		bool isZPositive = alignedPoint.z >= 0;

		glm::vec2 uv{};
		if (absAlignedP.x >= absAlignedP.y && absAlignedP.x >= absAlignedP.z) {
			if (isXPositive == true) {
				uv.x = -absAlignedP.z / absAlignedP.x;
				uv.y = absAlignedP.y / absAlignedP.x;
			}
			else {
				uv.x = absAlignedP.z / absAlignedP.x;
				uv.y = absAlignedP.y / absAlignedP.x;
			}
		}
		else if (absAlignedP.y >= absAlignedP.x && absAlignedP.y >= absAlignedP.z) {
			if (isYPositive == true) {
				uv.x = absAlignedP.x / absAlignedP.y;
				uv.y = -absAlignedP.z / absAlignedP.y;
			}
			else {
				uv.x = absAlignedP.x / absAlignedP.y;
				uv.y = absAlignedP.z / absAlignedP.y;
			}
		}
		else if (absAlignedP.z >= absAlignedP.y && absAlignedP.z >= absAlignedP.x) {
			if (isZPositive == true) {
				uv.x = absAlignedP.x / absAlignedP.z;
				uv.y = absAlignedP.y / absAlignedP.z;
			}
			else {
				uv.x = -absAlignedP.x / absAlignedP.z;
				uv.y = absAlignedP.y / absAlignedP.z;
			}
		}

		VertexUVDatas[i] = (uv - glm::vec2(1.)) * 0.5f;
	}
	Renderer renderer{};
	renderer.BufferData(GL_ARRAY_BUFFER, uvbo, VertexUVDatas);
}
