/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Scene.h
Purpose: Base of scenes
Language: c++
Platform: x64
Project: junsu.jang, CS300, Assignment 2 - Implementing Phong Illumination Model
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 09/27/2022
End Header --------------------------------------------------------*/
#pragma once

class GAPP;
class Scene {

public:
	Scene() = default;
	virtual ~Scene() = default;
	virtual void Load() = 0;
	virtual void Update(double dt) = 0;
	//virtual void ImGUIUpdate() = 0;
	virtual void Draw() = 0;
	virtual void Unload() = 0;
};