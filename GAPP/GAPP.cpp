/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: GAPP.cpp
Purpose: For setting engine and window
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 1 - Hybrid Rendering
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 09/26/2022
End Header --------------------------------------------------------*/
#include "pch.h"
#include "framework.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "GAPP.h"
#include "Scene.h"
#include "SceneManager.h"

void GLAPIENTRY OpenGLMessageCallback(
	GLenum /*source*/,
	GLenum type,
	GLuint /*id*/,
	GLenum severity,
	GLsizei /*length*/,
	const GLchar* message,
	const void* /*userParam*/)
{
	if (GL_DEBUG_TYPE_OTHER == type)
		return;
	fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
		type, severity, message);
}

void Key_callback_func(GLFWwindow* /*pwin*/, int /*key*/, int /*scancode*/, int /*action*/, int /*mod*/)
{
	
}

void GAPP::Init()
{	
	glfwMakeContextCurrent(NULL);
	if (!glfwInit())
	{
		std::cout << "GLFW init Failed"<<std::endl;
		glfwTerminate();

		return;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
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

	glfwSetKeyCallback(window, Key_callback_func);
	//glfwSetMouseButtonCallback(window, Mouse::MouseButton_callback_func);
	//glfwSetScrollCallback(window, Mouse::MouseScroll_callback_func);
	//glfwSetCursorPosCallback(window, Mouse::MousePosition_callback_func);
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

	glEnable(GL_DEBUG_OUTPUT);

	glDebugMessageCallback(OpenGLMessageCallback, nullptr);

	sceneManager = new SceneManager{};

}

void GAPP::SetScene(int i, Scene& scene)
{
	//set scene number in S.M
	scene.setEngine(this);
	sceneManager->BindScene(i, scene);
}

void GAPP::SetNextScene(int i)
{
	sceneManager->setNextScene(i);
}

void GAPP::Update()
{
	
	// Setup ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	// Setup Platform/Renderer bindings
	ImGui::StyleColorsDark();
	const char* glsl_version = "#version 460";
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
		
	
	sceneManager->Init();

	bool engineLoop{true};

	do
	{
		glfwGetWindowSize(window,&windowWidth,&windowHeight);
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

const GLFWwindow* GAPP::GetWindow()const {
	return window;
}

const glm::vec2 GAPP::GetWindowSize() const{
	return {windowWidth,windowHeight};
}