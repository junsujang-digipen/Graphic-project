/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: OBJLoader.h
Purpose: For load obj file
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 1 - Hybrid Rendering
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 09/29/2022
End Header --------------------------------------------------------*/
#pragma once
#include "glm/glm.hpp"
#include "Declare.h"
#include "MeshData.h"

struct MyStruct;
class OBJLoader{
public:
	std::string meshName{};
	std::vector<glm::vec3> VertexDatas{};
	std::vector<glm::vec3> FaceNormalDatas{};
	std::vector<glm::vec3> VertexNormalDatas{};
	std::vector<glm::vec3> VertexTextureDatas{};
	std::vector<GLuint> idxDatas{};
	unsigned int primitive_type{ GL_TRIANGLES };
	unsigned int primitive_cnt{};

	glm::vec3 boundBoxMax{};
	glm::vec3 boundBoxMin{};

	OBJLoader() = default;
	void FileLoad(const char* path);
	std::vector<glm::vec3> VertexArrayData();
	MeshData makeMeshData(const glm::vec3& objScale);
};

inline bool DegenerateTri(const glm::vec3& v0, const  glm::vec3& v1, const  glm::vec3& v2)
{
	const float  EPSILON = 0.00001f;
	return (glm::distance(v0, v1) < EPSILON ||
		glm::distance(v1, v2) < EPSILON ||
		glm::distance(v2, v0) < EPSILON);
}


inline void BuildIndexBuffer(int stacks, int slices, std::vector<GLuint>& idx_vtx, const std::vector<glm::vec3>& pos)
{
	int p0 = 0, p1 = 0, p2 = 0;
	int p3 = 0, p4 = 0, p5 = 0;

	int stride{ slices + 1 };
	for (int i = 0; i < stacks; ++i)
	{
		int curr_row = i * stride;
		for (int j = 0; j < slices; ++j)
		{
			p0 = curr_row + j;
			p1 = p0 + 1;
			p2 = p1 + stride;

			if (!DegenerateTri(pos[p0], pos[p1], pos[p2]))
			{
				idx_vtx.push_back(p0);
				idx_vtx.push_back(p1);
				idx_vtx.push_back(p2);
			}
			p3 = p2;
			p4 = p0 + stride;
			p5 = p0;
			if (!DegenerateTri(pos[p3], pos[p4], pos[p5]))
			{
				idx_vtx.push_back(p3);
				idx_vtx.push_back(p4);
				idx_vtx.push_back(p5);
			}
		}
	}
}

inline void MakeSphereData(int stacks, int slices, std::vector<glm::vec3>& pos_vtx, std::vector<glm::vec3>& vertex_nrm_vtx, std::vector<glm::vec3>& face_nrm_vtx, std::vector<GLuint>& idx_vtx)
{
	float Radius = 0.5f;
	const float PI = 3.14159f;
	for (int i = 0; i <= stacks; ++i)
	{
		float row = static_cast<float>(i) / stacks;
		float beta = PI * (row - 0.5f);
		for (int j = 0; j <= slices; j++)
		{
			float col = static_cast<float>(j) / slices;
			float alpha = col * PI * 2.f;
			glm::vec3 pos = glm::vec3{
				Radius * sin(alpha) * cos(beta),
				Radius * sin(beta),
				Radius * cos(alpha) * cos(beta) };
			glm::vec3 nrm = pos / Radius;
			pos_vtx.push_back(pos);
			vertex_nrm_vtx.push_back(nrm);
		}
	}
	BuildIndexBuffer(stacks, slices, idx_vtx, pos_vtx);
	//Face normal
	unsigned indxSize{ static_cast<unsigned>(idx_vtx.size()) };
	for (unsigned i = 0; i < indxSize; i += 3) {
		glm::vec3 FaceNormal{ glm::normalize(glm::cross(pos_vtx[idx_vtx[i + 1]] - pos_vtx[idx_vtx[i]], pos_vtx[idx_vtx[i + 2]] - pos_vtx[idx_vtx[i]])) };
		face_nrm_vtx.push_back(FaceNormal);
		face_nrm_vtx.push_back(FaceNormal);
		face_nrm_vtx.push_back(FaceNormal);
	}
}
inline void MakeScreenData( std::vector<glm::vec3>& pos_vtx, std::vector<glm::vec2>& UV_vtx, std::vector<GLuint>& idx_vtx)
{
	pos_vtx.push_back({ 1.f,  1.f, 0.0f });
	pos_vtx.push_back({ -1.f, -1.f, 0.0f });
	pos_vtx.push_back({ 1.f, -1.f, 0.0f });
	pos_vtx.push_back({ -1.f,  1.f, 0.0f });
	pos_vtx.push_back({ -1.f, -1.f, 0.0f });
	pos_vtx.push_back({ 1.f,  1.f, 0.0f });

	UV_vtx.push_back({1.0f, 1.0f});
	UV_vtx.push_back({0.0f, 0.0f});
	UV_vtx.push_back({1.0f, 0.0f});
	UV_vtx.push_back({0.0f, 1.0f});
	UV_vtx.push_back({0.0f, 0.0f});
	UV_vtx.push_back({1.0f, 1.0f});

	idx_vtx.push_back(0);
	idx_vtx.push_back(1);
	idx_vtx.push_back(2);
	idx_vtx.push_back(3);
	idx_vtx.push_back(4);
	idx_vtx.push_back(5);
}
inline void MakeCircleLineData(int num, std::vector<glm::vec3>& pos_vtx, std::vector<glm::vec3>& vertex_nrm_vtx, std::vector<glm::vec3>& /*face_nrm_vtx*/,  std::vector<GLuint>& idx_vtx)
{
	float Radius = 0.5f;
	const float PI = 3.14159f;

	for (int j = 0; j <num; j++)
	{
		float now = (float)j / num;
		float alpha = now * PI * 2.f;
		glm::vec3 pos = glm::vec3{
			Radius * sin(alpha),
			0.f,
			Radius* cos(alpha) };
		glm::vec3 nrm = pos / Radius;
		pos_vtx.push_back(pos);
		vertex_nrm_vtx.push_back(nrm);	
	}
	unsigned posSize{ static_cast<unsigned>(pos_vtx.size()) -1 };
	for (unsigned i = 0; i < posSize; ++i) {
		idx_vtx.push_back(i);
		idx_vtx.push_back(i+1);
	}
	idx_vtx.push_back(posSize);
	idx_vtx.push_back(0);
}
inline void makeNormalDrawVec(std::vector<glm::vec3>& vertex, std::vector<glm::vec3>& vertexNormal, std::vector<glm::vec3>& drawVec, glm::vec3 objScale, float NormalSize = 1.f) {
	const int vdSize{ (int)vertex.size() };
	for (int i = 0; i < vdSize; ++i) {
		drawVec.push_back(vertex[i]);
		drawVec.push_back(vertex[i] + vertexNormal[i]/objScale * NormalSize);
	}
}
inline void makeFaceNormalDrawVec(std::vector<glm::vec3>& vertex, std::vector<glm::vec3>& FaceNormal, std::vector<glm::vec3>& drawVec, glm::vec3 objScale, float NormalSize = 1.f) {
	const int vdSize{ (int)vertex.size() };
	for (int i = 0; i < vdSize; i+=3) {
		glm::vec3 center{ (vertex[i]+ vertex[i+1]+ vertex[i+2])/3.f };
		drawVec.push_back(center);
		drawVec.push_back(center + FaceNormal[i] / objScale * NormalSize);
	}
}