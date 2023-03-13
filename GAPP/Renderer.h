/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Renderer.h
Purpose: For setting buffers
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 2 - Bounding Volumes
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 09/29/2022
End Header --------------------------------------------------------*/
#pragma once
#include "glm/glm.hpp"
#include <vector>
typedef unsigned int GLuint;

class Renderer {
public:
	void genVertexArray(GLuint* vao);//
	void deleteVertexArray(GLuint *vao);
	void PointBufferGlmVec3(GLuint vao, GLuint vbo, GLuint layerIndex);//
	void PointBufferGlmVec2(GLuint vao, GLuint vbo, GLuint layerIndex);//
	void genBuffer(GLuint* vbo);//
	void deleteBuffer(unsigned bufferType,GLuint* vbo);
	void BufferData(unsigned bufferType,GLuint vbo,  const std::vector<glm::vec3>& data);//
	void BufferData(unsigned bufferType, GLuint vbo, const std::vector<glm::vec2>& data);//
	void BufferData(unsigned bufferType, GLuint vbo, const std::vector<GLuint>& data);//
	
	
};