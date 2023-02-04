/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: FSQ.h
Purpose: Full screen quad
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 1 - Hybrid Rendering
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 02/03/2023
End Header --------------------------------------------------------*/
#pragma once
#include "Entity.h"
class FSQ :public Entity {

public:
	FSQ();
	void draw() override;
};