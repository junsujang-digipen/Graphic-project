/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Renderer.cpp
Purpose: For setting buffers
Language: c++
Platform: x64
Project: junsu.jang, CS300, Assignment 3 - Dynamic Environment Mapping
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 09/29/2022
End Header --------------------------------------------------------*/
#include "pch.h"
#include "Renderer.h"

void Renderer::genVertexArray(GLuint* vao)
{
	glGenVertexArrays(1, vao);
}

void Renderer::deleteVertexArray(GLuint* vao)
{
	glBindVertexArray(0);
	glDeleteVertexArrays(1,vao);
}

void Renderer::PointBufferGlmVec3(GLuint vao, GLuint vbo,GLuint layerIndex)
{
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(layerIndex);
	glVertexAttribPointer(layerIndex, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Renderer::PointBufferGlmVec2(GLuint vao, GLuint vbo, GLuint layerIndex)
{
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(layerIndex);
	glVertexAttribPointer(layerIndex, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Renderer::genBuffer(GLuint* vbo)
{
	glGenBuffers(1, vbo);
}

void Renderer::deleteBuffer(unsigned bufferType,GLuint* vbo)
{
	glBindBuffer(bufferType, 0);
	glDeleteBuffers(1,vbo);
}

void Renderer::BufferData(unsigned bufferType, GLuint vbo, const std::vector<glm::vec3>& data)
{
	glBindBuffer(bufferType, vbo);
	glBufferData(bufferType, sizeof(glm::vec3) * data.size(), data.data(), GL_STATIC_DRAW);
	glBindBuffer(bufferType, 0);
}
void Renderer::BufferData(unsigned bufferType, GLuint vbo, const std::vector<glm::vec2>& data)
{
	glBindBuffer(bufferType, vbo);
	glBufferData(bufferType, sizeof(glm::vec2) * data.size(), data.data(), GL_STATIC_DRAW);
	glBindBuffer(bufferType, 0);
}
void Renderer::BufferData(unsigned bufferType, GLuint vbo, const std::vector<GLuint>& data)
{
	glBindBuffer(bufferType, vbo);
	glBufferData(bufferType, sizeof(GLuint) * data.size(), data.data(), GL_STATIC_DRAW);
	glBindBuffer(bufferType, 0);
}
