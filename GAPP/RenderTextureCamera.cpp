/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: RenderTextureCamera.cpp
Purpose: Scene for testing objs and loader and shaders
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 3 - Spatial Partitioning
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 11/26/2022
End Header --------------------------------------------------------*/
#include "pch.h"
#include "RenderTextureCamera.h"
#include "TextureManager.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

RenderTextureCamera::RenderTextureCamera(TextureManager* tm, std::string texName)
	:Camera(), textureManager(tm), textureName(texName)
{
	setAr(1.f);
	setFov(3.14f * 0.5f);
	FramebufferName = 0; // handle to framebuffer
	glGenFramebuffers(1, &FramebufferName); // create the FB object handle
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName); // assign the handle to an
	// object of type GL_FRAMEBUFFER, GL_DRAW_FRAMEBUFFER, or GL_READ_FRAMEBUFFER

	glGenTextures(1, &renderedTexture);
	textureManager->EnrollTexture(textureName,renderedTexture, 1024, 1024, GL_CLAMP_TO_EDGE);


	//glGenRenderbuffers(1, &depthrenderbuffer);
	//glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 1024);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);

	//GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	//GLuint numBuffers = 1;
	//glDrawBuffers(numBuffers, DrawBuffers);

	//glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;

	//glBindBuffer(GL_DRAW_FRAMEBUFFER, 0);
	//glDrawBuffer(GL_BACK_LEFT);


	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//GL_COLOR_ATTACHMENT0 = GL_NONE
	//GL_DEPTH_ATTACHMENT = GL_NONE
	//GL_STENCIL_ATTACHMENT = GL_NONE

}

RenderTextureCamera::~RenderTextureCamera()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1,&FramebufferName);
}

void RenderTextureCamera::startMakeRenderTargetTexture()
{

	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
	glViewport(0, 0, 1024, 1024);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glEnable(GL_POLYGON_OFFSET_FILL);
	//glPolygonOffset(polygon_offset, polygon_units);
}

void RenderTextureCamera::endMakeRenderTargetTexture()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderTextureCamera::bindTexture(std::string shaderName, ShaderManager* shaderManager)
{
	textureManager->bindTextures(shaderName, shaderManager);
}


