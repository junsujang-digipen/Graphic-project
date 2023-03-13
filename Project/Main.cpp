/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Main.cpp
Purpose: Main cpp file
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 2 - Bounding Volumes
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 09/26/2022
End Header --------------------------------------------------------*/
#include "GAPP.h"
#include "TestScene.h"
#include "TestScene2.h"

int main() {
	GAPP app{};
	app.Init();


	//TestScene ttscene{};
	TestScene2 ttscene2{};
	app.SetScene(0, ttscene2);
	app.SetNextScene(0);

	app.Update();
	app.APPOff();
	return 0;
}