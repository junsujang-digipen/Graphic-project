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

FSQ::FSQ():Entity()
{
	VertexDatas.push_back({ 1.f,  1.f, 0.0f });
	VertexDatas.push_back({ -1.f, -1.f, 0.0f });
	VertexDatas.push_back({ 1.f, -1.f, 0.0f });
	VertexDatas.push_back({ -1.f,  1.f, 0.0f });
	VertexDatas.push_back({ -1.f, -1.f, 0.0f });
	VertexDatas.push_back({ 1.f,  1.f, 0.0f });

	VertexUVDatas.push_back({ 1.0f, 1.0f });
	VertexUVDatas.push_back({ 0.0f, 0.0f });
	VertexUVDatas.push_back({ 1.0f, 0.0f });
	VertexUVDatas.push_back({ 0.0f, 1.0f });
	VertexUVDatas.push_back({ 0.0f, 0.0f });
	VertexUVDatas.push_back({ 1.0f, 1.0f });

	idxDatas.push_back(0);
	idxDatas.push_back(1);
	idxDatas.push_back(2);
	idxDatas.push_back(3);
	idxDatas.push_back(4);
	idxDatas.push_back(5);

}

void FSQ::draw()
{

	objShader->useProgram();
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glLineWidth(1.f);
	glDrawElements(primitive_type, static_cast<GLsizei>(idxDatas.size()), GL_UNSIGNED_INT, nullptr);
	objShader->unuseProgram();
}
