/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Entity.cpp
Purpose: For storing Entity datas
Language: c++
Platform: x64
Project: junsu.jang, CS300, Assignment 1 - Render an OBJ file
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 09/30/2022
End Header --------------------------------------------------------*/
#include "pch.h"
#include "Entity.h"
#include "glm/ext/matrix_transform.hpp"
#include "Renderer.h"
#include "Shader.h"
#include "OBJLoader.h"


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
}

void Entity::unload()
{
	Renderer renderer{};
	renderer.deleteVertexArray(&vao);
	renderer.deleteBuffer(GL_ARRAY_BUFFER, &vbo);
	renderer.deleteBuffer(GL_ARRAY_BUFFER, &vno);
	renderer.deleteBuffer(GL_ARRAY_BUFFER, &vboForLine);
	renderer.deleteBuffer(GL_ELEMENT_ARRAY_BUFFER, &ibo);
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
	glm::mat3 u_normal = glm::transpose(glm::inverse(objectMatrix));
	objShader->sendUniformMatrix3fv("u_normalMatrix", u_normal);
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
