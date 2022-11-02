/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Main.cpp
Purpose: Main cpp file
Language: c++
Platform: x64
Project: junsu.jang, CS300, Assignment 1 - Render an OBJ file
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 09/26/2022
End Header --------------------------------------------------------*/
#include "GAPP.h"
#include "TestScene.h"

int main() {
	GAPP app{};
	app.Init();


	TestScene ttscene{};
	app.SetScene(0,ttscene);
	app.SetNextScene(0);

	app.Update();
	app.APPOff();
	return 0;
}