/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: FSQ.cpp
Purpose: Full screen quad
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 1 - Hybrid Rendering
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 02/03/2023
End Header --------------------------------------------------------*/
#include "pch.h"
#include "FSQ.h"
#include <GL/glew.h>
#include "Shader.h"
#include "Scene.h"
#include "MeshManager.h"
#include "OBJLoader.h"



FSQ::FSQ(Scene* sc, ID id) : Entity(sc, id) {
	MeshData temp{};
	MakeScreenData(temp.VertexDatas, temp.VertexUVDatas, temp.idxDatas);
	//temp.VertexDatas.push_back({ 1.f,  1.f, 0.0f });
	//temp.VertexDatas.push_back({ -1.f, -1.f, 0.0f });
	//temp.VertexDatas.push_back({ 1.f, -1.f, 0.0f });
	//temp.VertexDatas.push_back({ -1.f,  1.f, 0.0f });
	//temp.VertexDatas.push_back({ -1.f, -1.f, 0.0f });
	//temp.VertexDatas.push_back({ 1.f,  1.f, 0.0f });

	//temp.VertexUVDatas.push_back({ 1.0f, 1.0f });
	//temp.VertexUVDatas.push_back({ 0.0f, 0.0f });
	//temp.VertexUVDatas.push_back({ 1.0f, 0.0f });
	//temp.VertexUVDatas.push_back({ 0.0f, 1.0f });
	//temp.VertexUVDatas.push_back({ 0.0f, 0.0f });
	//temp.VertexUVDatas.push_back({ 1.0f, 1.0f });

	//temp.idxDatas.push_back(0);
	//temp.idxDatas.push_back(1);
	//temp.idxDatas.push_back(2);
	//temp.idxDatas.push_back(3);
	//temp.idxDatas.push_back(4);
	//temp.idxDatas.push_back(5);
	MeshID = sc->getMeshManager()->push_MeshData(temp);
}



void FSQ::draw()
{
	const MeshData& temp =  scene->getMeshManager()->getMeshData(MeshID);
	objShader->useProgram();
	glBindVertexArray(temp.vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, temp.ibo);
	glLineWidth(1.f);
	glDrawElements(temp.primitive_type, static_cast<GLsizei>(temp.idxDatas.size()), GL_UNSIGNED_INT, nullptr);
	objShader->unuseProgram();
}
