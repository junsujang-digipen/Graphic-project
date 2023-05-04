/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: RenderTextureCamera.h
Purpose: Scene for testing objs and loader and shaders
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 3 - Spatial Partitioning
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 11/26/2022
End Header --------------------------------------------------------*/
#pragma once
#include "Camera.h"

class TextureManager;
class ShaderManager;
class RenderTextureCamera:public Camera {
	GLuint FramebufferName{};
	GLuint renderedTexture{};
	GLuint depthrenderbuffer{};

	TextureManager* textureManager{};
	std::string textureName{};
public:
	RenderTextureCamera(TextureManager* tm, std::string texName);
	~RenderTextureCamera();
	void startMakeRenderTargetTexture();
	void endMakeRenderTargetTexture();
	void bindTexture(std::string shaderName, ShaderManager* shaderManager);
};