/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Shader.h
Purpose: For compile and linking shaders
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 1 - Hybrid Rendering
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 09/29/2022
End Header --------------------------------------------------------*/
#pragma once
#include <vector>
#include "glm/glm.hpp"


enum class ShaderType {
	VERTEX_SHADER,
	FRAGMENT_SHADER,
};

typedef unsigned int GLuint;
typedef char GLchar;
typedef float GLfloat;
class Shader {
	GLuint shdrProgram{};
	std::vector<GLuint> shdrs{};

	std::string ShaderFileName{};
public:
	Shader(std::string name);
	~Shader();

	//const std::string getShaderSourceFromFile(const char* path);
	GLuint compileShader(ShaderType ShaderType, const std::vector<std::string> source);
	GLuint compileShader(GLuint ShaderType, const std::vector<std::string> source);
	void attachShader(GLuint shdr);
	void linkProgram();
	GLuint getHandle();
	void useProgram();
	void unuseProgram();

	void initProgram();

	void sendUniform1iv(const GLchar* name, const int& data);
	void sendUniform1fv(const GLchar* name, const float& data);
	void sendUniform3fv(const GLchar* name, const glm::vec3& data);
	void sendUniformMatrix4fv(const GLchar* name, const glm::mat4& data);
	void sendUniformMatrix3fv(const GLchar* name, const glm::mat3& data);

};

namespace ShaderHelper {
	const std::string getShaderSourceFromFile(const char* path);
}