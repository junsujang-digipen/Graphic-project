/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Shader.h
Purpose: For compile and linking shaders
Language: c++
Platform: x64
Project: junsu.jang, CS300, Assignment 1 - Render an OBJ file
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 09/29/2022
End Header --------------------------------------------------------*/
#pragma once
#include <vector>
#include "glm/glm.hpp"

typedef unsigned int GLuint;
typedef char GLchar;
typedef float GLfloat;
class Shader {
	GLuint shdrProgram{};
	std::vector<GLuint> shdrs{};
public:
	Shader();
	~Shader();

	const std::string getShaderSourceFromFile(const char* path);

	GLuint compileShader(GLuint ShaderType, const std::vector<std::string> source);
	void attachShader(GLuint shdr);
	void linkProgram();
	GLuint getHandle();
	void useProgram();
	void unuseProgram();


	void sendUniform1iv(const GLchar* name, const int& data);
	void sendUniform1fv(const GLchar* name, const float& data);
	void sendUniform3fv(const GLchar* name, const glm::vec3& data);
	void sendUniformMatrix4fv(const GLchar* name, const glm::mat4& data);
	void sendUniformMatrix3fv(const GLchar* name, const glm::mat3& data);

};

namespace ShaderHelper {
	const std::string getShaderSourceFromFile(const char* path);
}