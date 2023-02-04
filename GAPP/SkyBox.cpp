/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: SkyBox.cpp
Purpose: Scene for testing objs and loader and shaders
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 1 - Hybrid Rendering
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 11/26/2022
End Header --------------------------------------------------------*/
#include "pch.h"
#include "SkyBox.h"
#include "Camera.h"
#include "OBJLoader.h"
#include "ShaderManager.h"
#include "Shader.h"
#include "textureManager.h"
#include <GL/glew.h>

void SkyBox::genShader()
{
	std::shared_ptr<Shader> SkyBoxShader{ shaderManager->makeShader(skyBoxShaderName) };
	GLuint vshdr = SkyBoxShader->compileShader(GL_VERTEX_SHADER, { 
		ShaderHelper::getShaderSourceFromFile("../Shaders/FB_Version.glsl") ,
		ShaderHelper::getShaderSourceFromFile("../Shaders/FF_CommonFunctions.glsl"),
		ShaderHelper::getShaderSourceFromFile("../Shaders/F_SkyBoxShader.vert") });
	GLuint fshdr = SkyBoxShader->compileShader(GL_FRAGMENT_SHADER, { 
		ShaderHelper::getShaderSourceFromFile("../Shaders/FB_Version.glsl") ,
		ShaderHelper::getShaderSourceFromFile("../Shaders/FF_CommonFunctions.glsl"),
		ShaderHelper::getShaderSourceFromFile("../Shaders/F_SkyBoxShader.frag") });
	SkyBoxShader->attachShader(vshdr);
	SkyBoxShader->attachShader(fshdr);
	SkyBoxShader->linkProgram();
	objShader = SkyBoxShader;
}

SkyBox::SkyBox(ShaderManager* sm, TextureManager* tm):Entity(),shaderManager(sm),textureManager(tm)
{
	genShader();
}

//SkyBox::SkyBox(ShaderManager* sm, TextureManager* tm, std::string cubeMapPath)
//	: SkyBox(sm,tm)
//{
//}

SkyBox::SkyBox(ShaderManager* sm, TextureManager* tm, std::string left, std::string right, std::string bottom, std::string top, std::string back, std::string front)
	:SkyBox(sm, tm)
{
}

SkyBox::~SkyBox()
{
}

void SkyBox::setTargetCamera(Camera* camera)
{
	targetCamera = camera;
}

void SkyBox::setTexture(std::string left, std::string right, std::string bottom, std::string top, std::string back, std::string front)
{
	textureManager->EnrollTexture("left", left, GL_CLAMP_TO_EDGE);
	textureManager->SetTextureMappingData(skyBoxShaderName, "left", "BGTexture[0]");
	textureManager->EnrollTexture("right", right, GL_CLAMP_TO_EDGE);
	textureManager->SetTextureMappingData(skyBoxShaderName, "right", "BGTexture[1]");
	textureManager->EnrollTexture("bottom", bottom, GL_CLAMP_TO_EDGE);
	textureManager->SetTextureMappingData(skyBoxShaderName, "bottom", "BGTexture[2]");
	textureManager->EnrollTexture("top", top, GL_CLAMP_TO_EDGE);
	textureManager->SetTextureMappingData(skyBoxShaderName, "top", "BGTexture[3]");
	textureManager->EnrollTexture("back", back, GL_CLAMP_TO_EDGE);
	textureManager->SetTextureMappingData(skyBoxShaderName, "back", "BGTexture[4]");
	textureManager->EnrollTexture("front", front, GL_CLAMP_TO_EDGE);
	textureManager->SetTextureMappingData(skyBoxShaderName, "front", "BGTexture[5]");
	textureManager->bindTextures(skyBoxShaderName, shaderManager);
}

void SkyBox::load()
{
	OBJLoader Temp{};
	Temp.FileLoad("../Objects/SkyBox.obj");
	setScale(glm::vec3(1.f, 1.f, 1.f));
	GetDataForOBJLoader(Temp);


	Entity::load();
}

//void SkyBox::unload()
//{
//}

void SkyBox::update(double dt)
{
	if (targetCamera == nullptr) {
		std::cout << "target camera does not exist" << std::endl;
	}
	else {
		setPos(targetCamera->getCameraPos());
	}
	Entity::update(dt);
}

void SkyBox::draw()
{
	glDepthMask(GL_FALSE);
	textureManager->bindTextures(skyBoxShaderName, shaderManager);
	Entity::draw();
	glDepthMask(GL_TRUE);
}

void SkyBox::drawNormal(int /*num*/)
{
}

std::string SkyBox::getShaderName()
{
	return skyBoxShaderName;
}
