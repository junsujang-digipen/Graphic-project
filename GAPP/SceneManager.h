/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: SceneManager.h
Purpose: For managing scenes
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 3 - Spatial Partitioning
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 09/27/2022
End Header --------------------------------------------------------*/
#pragma once
#include <map>


class Scene;
class SceneManager {

	std::map<int, Scene*> scenes{};

	enum class SceneManagerState {
		start,
		Load,
		Update,
		Unload,
		Exit,
	};

	Scene* nowScene{};
	Scene* nextScene{};
	Scene* prevScene{};
	SceneManagerState nowState{};

public:
	SceneManager() = default;
	
	void BindScene(int i,Scene& scene);
	void Init();
	void Update(double dt);
	void setNextScene(int i);
	void exit() {
		nowState = SceneManagerState::Exit;
	}
};