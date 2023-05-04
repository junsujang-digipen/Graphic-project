/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: MeshData.cpp
Purpose: For load obj file
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 3 - Spatial Partitioning
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 02/27/2023
End Header --------------------------------------------------------*/
#pragma once
#include "pch.h"
#include "MeshData.h"
#include "Renderer.h"
#include <GL/glew.h>


void MeshData::load()
{
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

void MeshData::unload()
{

	Renderer renderer{};
	renderer.deleteVertexArray(&vao);
	renderer.deleteBuffer(GL_ARRAY_BUFFER, &vbo);
	renderer.deleteBuffer(GL_ARRAY_BUFFER, &vno);
	renderer.deleteBuffer(GL_ARRAY_BUFFER, &vboForLine);
	renderer.deleteBuffer(GL_ELEMENT_ARRAY_BUFFER, &ibo);
	renderer.deleteBuffer(GL_ARRAY_BUFFER, &uvbo);
}

void MeshData::calcSphereTexCoord()
{
	MeshData& temp = *this;
	glm::vec3 center = (temp.boundBoxMin + temp.boundBoxMax) / 2.f;
	for (int i = 0; i < temp.VertexDatas.size(); ++i) {
		glm::vec3 vpos{};
		if (IsPositionEntity == true) {
			vpos = temp.VertexDatas[i];
		}
		else {
			vpos = temp.VertexNormalDatas[i];
		}
		glm::vec3 alignedPoint = vpos - center;
		float theta = atan(alignedPoint.z / alignedPoint.x);
		float alignedMiny = temp.boundBoxMin.y - center.y;
		float alignedMaxy = temp.boundBoxMax.y - center.y;
		float y = (alignedPoint.y - alignedMiny) / (alignedMaxy - alignedMiny);
		temp.VertexUVDatas[i] = { theta / 6.14f, y };
	}
	Renderer renderer{};
	renderer.BufferData(GL_ARRAY_BUFFER, temp.uvbo, temp.VertexUVDatas);
}

void MeshData::calcCylindricalTexCoord()
{
	MeshData& temp = *this;

	glm::vec3 center = (temp.boundBoxMin + temp.boundBoxMax) / 2.f;
	for (int i = 0; i < temp.VertexDatas.size(); ++i) {
		glm::vec3 vpos{};
		if (IsPositionEntity == true) {
			vpos = temp.VertexDatas[i];
		}
		else {
			vpos = temp.VertexNormalDatas[i];
		}
		glm::vec3 alignedPoint = vpos - center;
		float theta = atan(alignedPoint.z / alignedPoint.x);
		float r = sqrt(alignedPoint.x * alignedPoint.x + alignedPoint.y * alignedPoint.y + alignedPoint.z * alignedPoint.z);
		float latitude = acos(alignedPoint.y / r);
		temp.VertexUVDatas[i] = glm::vec2(theta / 6.14f, 3.14f - latitude / 3.14f);
	}
	Renderer renderer{};
	renderer.BufferData(GL_ARRAY_BUFFER, temp.uvbo, temp.VertexUVDatas);
}

void MeshData::calcPlanarTexCoord()
{
	MeshData& temp = *this;

	glm::vec3 center = (temp.boundBoxMin + temp.boundBoxMax) / 2.f;
	glm::vec3 alignedPointMin = temp.boundBoxMin - center;
	glm::vec3 alignedPointMax = temp.boundBoxMax - center;
	for (int i = 0; i < temp.VertexDatas.size(); ++i) {
		glm::vec3 vpos{};
		if (IsPositionEntity == true) {
			vpos = temp.VertexDatas[i];
		}
		else {
			vpos = temp.VertexNormalDatas[i];
		}
		glm::vec3 alignedPoint = vpos - center;
		temp.VertexUVDatas[i] = (alignedPoint - alignedPointMin) / (alignedPointMax - alignedPointMin);
	}
	Renderer renderer{};
	renderer.BufferData(GL_ARRAY_BUFFER, temp.uvbo, temp.VertexUVDatas);
}

void MeshData::calcCubeMapTexCoord()
{
	MeshData& temp = *this;

	glm::vec3 center = (temp.boundBoxMin + temp.boundBoxMax) / 2.f;
	for (int i = 0; i < temp.VertexDatas.size(); ++i) {
		glm::vec3 vpos{};
		if (IsPositionEntity == true) {
			vpos = temp.VertexDatas[i];
		}
		else {
			vpos = temp.VertexNormalDatas[i];
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

		temp.VertexUVDatas[i] = (uv - glm::vec2(1.)) * 0.5f;
	}
	Renderer renderer{};
	renderer.BufferData(GL_ARRAY_BUFFER, temp.uvbo, temp.VertexUVDatas);
}
