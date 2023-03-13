/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Input.h
Purpose: Input
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 2 - Bounding Volumes
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 03/05/2023
End Header --------------------------------------------------------*/

#pragma once
#include <map>
#include <string>
#include "glm/glm.hpp"
#include <vector>

//class InputKey {
//public:
//	InputKey(int button);
//	void ReSetKey(int b);
//	bool IsKeyDown() const;
//	bool IsKeyReleased() const;
//	int get_key();
//private:
//	int button;
//};
struct GLFWwindow;
class Keyboard {
	static std::map<int,bool> keyDown;
	static std::map<int,bool> wasKeyDown;
public:
	Keyboard();
	bool IskeyClicked(int key);
	bool IsKeyDown(int key) ;
	bool IsKeyReleased(int key) ;
	static void SetKeyDown(int key, bool value);
	static void Key_callback_func(GLFWwindow* pwin, int key, int scancode, int action, int mod);
	void update();
};

class Mouse {
	static std::map<int, bool> buttonDown;
	static std::map<int, bool> wasButtonDown;
	static glm::dvec2 mouseScrollVal;
	static glm::dvec2 mousePosition;
public:
	Mouse();
	bool IsButtonClicked(int key);
	bool IsButtonDown(int key);
	bool IsButtonReleased(int key);
	static void SetButtonDown(int button, bool value);
	glm::dvec2 getMouseScrollVal();
	glm::dvec2 getMousePosition();
	static void MouseButton_callback_func(GLFWwindow*, int, int, int);
	static void MouseScroll_callback_func(GLFWwindow*, double, double);
	static void MousePosition_callback_func(GLFWwindow*, double, double);
	void update();
};

class Input {
public:
	Input(GLFWwindow* );
	~Input();

	void Update();
	Keyboard getKeboard();
	Mouse getMouse();
private:
	GLFWwindow* winPtr;
	Keyboard keyBoard{};
	Mouse mouse{};
};