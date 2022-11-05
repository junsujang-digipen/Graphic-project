/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: GAPP.cpp
Purpose: For setting engine and window
Language: c++
Platform: x64
Project: junsu.jang, CS300, Assignment 1 - Render an OBJ file
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 09/26/2022
End Header --------------------------------------------------------*/
#include "pch.h"
#include "framework.h"


#include "GAPP.h"
#include "Scene.h"
#include "SceneManager.h"

void GAPP::Init()
{	
	if (!glfwInit())
	{
		std::cout << "GLFW init Failed"<<std::endl;
		glfwTerminate();

		return;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	window = glfwCreateWindow(windowWidth, windowHeight,"GAPP",
		nullptr, // which monitor (if full-screen mode)
		nullptr); // if sharing context with another window
	if (!window) {
		std::cout << "GLFW can't make window";
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	GLenum err = glewInit();
	if (err != GLEW_OK) {
		std::cerr << "Unable to initialize GLEW - error: "<< glewGetErrorString(err) << " abort program" << std::endl;
		glfwTerminate();
		return;
	}
	if (GLEW_VERSION_4_0) {
		std::cout << "Using glew version: " << glewGetString(GLEW_VERSION) << std::endl;
		std::cout << "Driver supports OpenGL 4.0\n" << std::endl;
	}
	else {
		std::cerr << "Driver doesn't support OpenGL 4.0 - abort program" << std::endl;
		return;
	}

	sceneManager = new SceneManager{};

}

void GAPP::SetScene(int i, Scene& scene)
{
	//set scene number in S.M
	sceneManager->BindScene(i, scene);
}

void GAPP::SetNextScene(int i)
{
	sceneManager->setNextScene(i);
}

void GAPP::Update()
{
	{
		// Setup ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		// Setup Platform/Renderer bindings
		ImGui::StyleColorsDark();
		const char* glsl_version = "#version 400";
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(glsl_version);
	}
	sceneManager->Init();

	bool engineLoop{true};

	do
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) != 0) {
			sceneManager->exit();
			engineLoop = false;
		}
		sceneManager->Update(0.1);

		glfwSwapBuffers(window);
		glfwPollEvents();

	} 
	while (engineLoop);
}

void GAPP::APPOff()
{
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
	glfwTerminate();
}
