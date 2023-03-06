/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: GAPP.h
Purpose: For setting engine and window
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 1 - Hybrid Rendering
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 09/26/2022
End Header --------------------------------------------------------*/
#pragma once
#include "glm/glm.hpp"

class Scene;
class SceneManager;
struct GLFWwindow;
class Input;
class GAPP {
	//friend Scene;

	//Scene manager
	SceneManager* sceneManager{};
	//Renderer
	//Physics?
	//ImGUI
	Input* inputManager{};
	

	//Event
	GLFWwindow* window;
	int windowWidth{ 1200 }, windowHeight{800};
public:

	GAPP() = default;

	void Init();

	void SetScene(int i,Scene& scene);
	void SetNextScene(int i);//Will changed to Event

	void Update();

	void APPOff();

	const GLFWwindow* GetWindow() const;
	Input* GetInputManager() const;
	const glm::vec2 GetWindowSize() const;
};