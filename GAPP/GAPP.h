/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: GAPP.h
Purpose: For setting engine and window
Language: c++
Platform: x64
Project: junsu.jang, CS300, Assignment 1 - Render an OBJ file
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 09/26/2022
End Header --------------------------------------------------------*/
#pragma once

class Scene;
class SceneManager;
struct GLFWwindow;
class GAPP {
	//friend Scene;

	//Scene manager
	SceneManager* sceneManager{};
	//Renderer
	//Physics?
	//ImGUI
	
	//Event
	GLFWwindow* window;
	int windowWidth{ 800 }, windowHeight{600};
public:

	GAPP() = default;

	void Init();

	void SetScene(int i,Scene& scene);
	void SetNextScene(int i);//Will changed to Event

	void Update();

	void APPOff();
};