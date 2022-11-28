/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Entity.h
Purpose: For storing Entity datas
Language: c++
Platform: x64
Project: junsu.jang, CS300, Assignment 3 - Dynamic Environment Mapping
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 09/30/2022
End Header --------------------------------------------------------*/
#pragma once
#include "glm/glm.hpp"

#include <vector>
#include <GL/glew.h>
#include <memory>

class Shader;
class OBJLoader;
class Entity {
	bool shouldOBJMatrixUpdate{true};

	std::vector<glm::vec3> VertexDatas{};
	std::vector<glm::vec3> FaceNormalDatas{};
	std::vector<glm::vec3> VertexNormalDatas{};
	std::vector<glm::vec3> VertexTextureDatas{};
	std::vector<GLuint> idxDatas{};
	std::vector<glm::vec3> VertexNormalDrawVec{};
	std::vector<glm::vec3> FaceNormalDrawVec{};
	unsigned int primitive_type{ GL_TRIANGLES };

	std::vector<glm::vec2> VertexUVDatas{};

	GLuint vao{}, vboForLine{}, vbo{}, vno{}, ibo{},uvbo;
public:
	std::shared_ptr<Shader> objShader{};
	std::shared_ptr<Shader> normalVectorShader{};

	glm::vec3 boundBoxMax{ 1.f };
	glm::vec3 boundBoxMin{ -1.f };

	glm::vec3 pos{};
	glm::vec3 scale{1.};
	glm::vec3 rotate{};
	glm::mat4 objectMatrix{};

	void objMatrixUpdate();
	void setPos(const glm::vec3 &p);
	void setScale(const glm::vec3 &s);
	void setRotate(const glm::vec3 &r);
	void GetDataForOBJLoader(OBJLoader& objLoader);

	virtual void load();
	virtual void unload();
	virtual void update(double dt);
	virtual void draw();
	virtual void drawNormal(int num);

	bool IsPositionEntity{true};
	void calcSphereTexCoord();
	void calcCylindricalTexCoord();
	void calcPlanarTexCoord();
	void calcCubeMapTexCoord();
};