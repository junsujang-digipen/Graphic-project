/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: BoundingVolumeManager.h
Purpose: Bounding volume management and generate hierarchy
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 1 - Hybrid Rendering
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 03/04/2023
End Header --------------------------------------------------------*/
#pragma once

class Scene;
class BoundingVolumeManager 	
{
	Scene* scene{};
public:
	BoundingVolumeManager(Scene *sc);
	void Update(double dt);
	void Draw();

	bool ShouldDrawBoundingSphere{false};
	bool ShouldDrawBoundingBox{false};
};
