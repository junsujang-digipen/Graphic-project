/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Input.cpp
Purpose: Input
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 3 - Spatial Partitioning
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 03/05/2023
End Header --------------------------------------------------------*/
#include "pch.h"
#include "Input.h"
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

std::map<int, bool> Keyboard::keyDown;
std::map<int, bool> Keyboard::wasKeyDown;
std::map<int, bool> Mouse::buttonDown;
std::map<int, bool> Mouse::wasButtonDown;
glm::dvec2 Mouse::mouseScrollVal;
glm::dvec2 Mouse::mousePosition;


Keyboard::Keyboard()
{
}

bool Keyboard::IskeyClicked(int key)
{
	return keyDown[key]== true && wasKeyDown[key] == false;
}

bool Keyboard::IsKeyDown(int key) 
{
	return keyDown[key] == true;
}

bool Keyboard::IsKeyReleased(int key) 
{
	return keyDown[key] == false && wasKeyDown[key] == true;
}

void Keyboard::SetKeyDown(int key, bool value)
{
	//std::cout << key << value << std::endl;
	keyDown[key] = value;
}

void Keyboard::Key_callback_func(GLFWwindow* /*pwin*/, int key, int /*scancode*/, int action, int /*mod*/)
{
	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		SetKeyDown(key, true);
	}
	else if (action == GLFW_RELEASE) {
		SetKeyDown(key, false);
	}
}

void Keyboard::update()
{
	wasKeyDown = keyDown;
}

Mouse::Mouse()
{
}

bool Mouse::IsButtonClicked(int key)
{
	return buttonDown[key] == true && wasButtonDown[key] == false;
}

bool Mouse::IsButtonDown(int key)
{
	return buttonDown[key] == false ;
}

bool Mouse::IsButtonReleased(int key)
{
	return buttonDown[key] == false && wasButtonDown[key] == true;
}

void Mouse::SetButtonDown(int button, bool value)
{
	//std::cout << button << value << std::endl;
	buttonDown[button] = value;
}

glm::dvec2 Mouse::getMouseScrollVal()
{
	return mouseScrollVal;
}

glm::dvec2 Mouse::getMousePosition()
{
	return mousePosition;
}

void Mouse::MouseButton_callback_func(GLFWwindow*, int button, int action, int /*mods*/)
{
	if (action == GLFW_PRESS) {
		SetButtonDown(button, true);
	}
	else if (action == GLFW_RELEASE) {
		SetButtonDown(button, false);
	}
}

void Mouse::MouseScroll_callback_func(GLFWwindow*, double xoffset, double yoffset)
{
	mouseScrollVal = glm::dvec2{ xoffset,yoffset };
}

void Mouse::MousePosition_callback_func(GLFWwindow*, double x, double y)
{
	mousePosition = glm::dvec2{x,y};
}

void Mouse::update()
{
	wasButtonDown = buttonDown;
}

Input::Input(GLFWwindow* winPtr):winPtr(winPtr)
{
	glfwSetKeyCallback(winPtr, Keyboard::Key_callback_func);
	glfwSetMouseButtonCallback(winPtr, Mouse::MouseButton_callback_func);
	glfwSetScrollCallback(winPtr, Mouse::MouseScroll_callback_func);
	glfwSetCursorPosCallback(winPtr, Mouse::MousePosition_callback_func);
}

Input::~Input()
{
}

void Input::Update()
{
	keyBoard.update();
	mouse.update();
}

Keyboard Input::getKeboard()
{
	return keyBoard;
}

Mouse Input::getMouse()
{
	return mouse;
}
