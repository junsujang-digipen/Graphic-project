/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: MeshData.h
Purpose: For load obj file
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 3 - Spatial Partitioning
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 02/27/2023
End Header --------------------------------------------------------*/
#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "Declare.h"
#include "BoundingVolumeType.h"

struct MeshData { // TODO: Separate normal debug draw data later
	std::vector<glm::vec3> VertexDatas{};
	std::vector<glm::vec3> FaceNormalDatas{};
	std::vector<glm::vec3> VertexNormalDatas{};
	std::vector<glm::vec3> VertexTextureDatas{};
	std::vector<GLuint> idxDatas{};
	std::vector<glm::vec3> VertexNormalDrawVec{};
	std::vector<glm::vec3> FaceNormalDrawVec{};
	std::vector<glm::vec2> VertexUVDatas{};
	GLuint vao{}, vboForLine{}, vbo{}, vno{}, ibo{}, uvbo{};

	unsigned int primitive_type{ GL_TRIANGLES };

	glm::vec3 boundBoxMax{ 1.f };
	glm::vec3 boundBoxMin{ -1.f };
	
	void load();
	void unload();

	bool IsPositionEntity{ true };
	void calcSphereTexCoord();
	void calcCylindricalTexCoord();
	void calcPlanarTexCoord();
	void calcCubeMapTexCoord();
};


