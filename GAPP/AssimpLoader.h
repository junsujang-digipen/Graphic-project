/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: AssimpLoader.h
Purpose: For load obj file
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 1 - Hybrid Rendering
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 02/26/2023
End Header --------------------------------------------------------*/
#pragma once
#include "MeshData.h"

class AssimpLoader {
public:
	std::vector<glm::vec3> VertexDatas{};
	std::vector<glm::vec3> FaceNormalDatas{};
	std::vector<glm::vec3> VertexNormalDatas{};
	std::vector<glm::vec3> VertexTextureDatas{};
	std::vector<GLuint>	idxDatas{};
	std::vector<glm::vec3> VertexNormalDrawVec{};
	std::vector<glm::vec3> FaceNormalDrawVec{};
	std::vector<glm::vec2> VertexUVDatas{};
	
	unsigned int primitive_type{ GL_TRIANGLES };

	glm::vec3 boundBoxMax{ 1.f };
	glm::vec3 boundBoxMin{ -1.f };

	void LoadModel(std::string path);
	MeshData MakeMeshData(const glm::vec3& objScale);
};