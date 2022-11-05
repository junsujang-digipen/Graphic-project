/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: TestScene.cpp
Purpose: Scene for testing objs and loader and shaders
Language: c++
Platform: x64
Project: junsu.jang, CS300, Assignment 1 - Render an OBJ file 
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 09/27/2022
End Header --------------------------------------------------------*/
#include "pch.h"
#include "TestScene.h"
#include "OBJLoader.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Camera.h"
#include "Shader.h"
#include "Renderer.h"
#include "Entity.h"
#include "Light.h"
#include <stb_image.h>
#include "Texture.h"

GLuint shdrProgram{};
GLuint shdrProgramNormal{};
Camera camera{};
glm::mat4 WTC{};

TestScene::TestScene(): Scene(),SpCurrNum(1)
{
}

void TestScene::Load()
{
	camera.init();
	camera.setPosition({ 0,130,150 });
	camera.setRotate({ glm::radians(-45.f),glm::radians(/*36.f*/0.f),0.f });
	camera.Update(0.01);

	
	const std::string Vshdr{ShaderHelper::getShaderSourceFromFile("../Shaders/C_DiffuseShader.vert")};
	const std::string Fshdr{ ShaderHelper::getShaderSourceFromFile("../Shaders/C_DiffuseShader.frag")};
	const std::string VshdrNormal{ ShaderHelper::getShaderSourceFromFile("../Shaders/C_BasicShader.vert")};
	const std::string FshdrNormal{ ShaderHelper::getShaderSourceFromFile("../Shaders/C_BasicShader.frag")};

	//lightings
		const std::string vPhongLightingshdr{ ShaderHelper::getShaderSourceFromFile("../Shaders/F_PhongLightingShader.vert") };
		const std::string fPhongLightingshdr{ ShaderHelper::getShaderSourceFromFile("../Shaders/F_PhongLightingShader.frag") };

		const std::string vPhongShadingshdr{ ShaderHelper::getShaderSourceFromFile("../Shaders/F_PhongShadingShader.vert") };
		const std::string fPhongShadingshdr{ ShaderHelper::getShaderSourceFromFile("../Shaders/F_PhongShadingShader.frag") };

		const std::string vBlinnShadingshdr{ ShaderHelper::getShaderSourceFromFile("../Shaders/F_BlinnShadingShader.vert") };
		const std::string fBlinnShadingshdr{ ShaderHelper::getShaderSourceFromFile("../Shaders/F_BlinnShadingShader.frag") };

		const std::string commonFunctionShdr{ ShaderHelper::getShaderSourceFromFile("../Shaders/FF_CommonFunctions.glsl") };
		const std::string shaderVersion{ ShaderHelper::getShaderSourceFromFile("../Shaders/FB_Version.glsl") };
		const std::string LightSturctShdr{ ShaderHelper::getShaderSourceFromFile("../Shaders/FS_LightStruct.glsl") };
		const std::string MaterialStructShdr{ ShaderHelper::getShaderSourceFromFile("../Shaders/FS_MaterialStruct.glsl") };

	//

	diffuseShader = std::make_shared<Shader>();
	NormalShdrProgram = std::make_shared<Shader>();
	//Shader
	{
		GLuint vshdr = diffuseShader->compileShader(GL_VERTEX_SHADER, { shaderVersion, commonFunctionShdr, vBlinnShadingshdr });
		GLuint fshdr = diffuseShader->compileShader(GL_FRAGMENT_SHADER, { shaderVersion, LightSturctShdr, MaterialStructShdr, commonFunctionShdr,fBlinnShadingshdr });
		diffuseShader->attachShader(vshdr);
		diffuseShader->attachShader(fshdr);
		diffuseShader->linkProgram();
		shdrProgram = diffuseShader->getHandle();
	}
	//shader normal
	{
		GLuint vshdr = NormalShdrProgram->compileShader(GL_VERTEX_SHADER, { VshdrNormal });
		GLuint fshdr = NormalShdrProgram->compileShader(GL_FRAGMENT_SHADER, {FshdrNormal});
		NormalShdrProgram->attachShader(vshdr);
		NormalShdrProgram->attachShader(fshdr);
		NormalShdrProgram->linkProgram();
		shdrProgramNormal = NormalShdrProgram->getHandle();
	}
	//Uniform data
	{
		WTC = { camera.getWorldToCameraToNDC() };
		////WTC = { glm::perspective <float > (glm::radians(45.f),800 / 600, 0.001f,1000.f) };

		diffuseShader->sendUniformMatrix4fv("WorldToCameraMat", WTC);
	}
	//uniform data for normal draw
	{
		WTC = { camera.getWorldToCameraToNDC() };
		NormalShdrProgram->sendUniformMatrix4fv("WorldToCameraMat", WTC);
	}
	//Obj load
	{
		const char* filepaths[5]{ 
			"./4Sphere.obj" ,
			"./bunny_high_poly.obj" ,
			"./cube2.obj" ,
			"./sphere.obj" ,
			"./sphere_modified.obj" };
		for (int i = 0; i < 5;++i) {
			OBJLoader objLoader{};
			objLoader.FileLoad(filepaths[i]);
			Obj.push_back(std::make_shared<Entity>());
			Obj[i]->setScale(glm::vec3(50.f));
			Obj[i]->GetDataForOBJLoader(objLoader);
			Obj[i]->load();

			Obj[i]->objShader = diffuseShader;
			Obj[i]->normalVectorShader = NormalShdrProgram;
		}

		{//texture
			textures.push_back(std::make_shared<Texture>("metal_roof_diff_512x512.png"));
			textures.push_back(std::make_shared<Texture>("metal_roof_spec_512x512.png"));
			textures[0]->sendTextureDataUniform(diffuseShader,"MatDiffuse");
			textures[1]->sendTextureDataUniform(diffuseShader,"MatSpecular");
			textures[0]->bindTexture();
			textures[1]->bindTexture();
		}
	}
	//plane
	{
		ObjPlane = std::make_shared<Entity>();
		OBJLoader Temp{};
		Temp.FileLoad("cube2.obj");
		ObjPlane->setPos(glm::vec3(0.f, -50.f, 0.f));
		ObjPlane->setScale(glm::vec3(1000.f,10.f, 1000.f));
		ObjPlane->GetDataForOBJLoader(Temp);
		ObjPlane->load();

		ObjPlane->objShader = diffuseShader;
		ObjPlane->normalVectorShader = NormalShdrProgram;
	}
	//Orbit
	{
		ObjCircleLine = std::make_shared<Entity>();
		OBJLoader Temp{};
		MakeCircleLineData(100, Temp.VertexDatas, Temp.VertexNormalDatas, Temp.FaceNormalDatas, Temp.idxDatas);
		Temp.primitive_type = GL_LINES;
		ObjCircleLine->setPos(glm::vec3(0.f, 0.f, 0.f));
		ObjCircleLine->setScale(glm::vec3(200.f));
		ObjCircleLine->GetDataForOBJLoader(Temp);
		ObjCircleLine->load();

		ObjCircleLine->objShader = diffuseShader;
		ObjCircleLine->normalVectorShader = NormalShdrProgram;
	}
	//sphere
	{
		const int SpMax{ 16 };
		for (int i = 0; i < SpMax; ++i) {
			ObjSpheres.push_back(std::make_shared<Light>());
			OBJLoader Temp{};
			MakeSphereData(20, 20, Temp.VertexDatas, Temp.VertexNormalDatas, Temp.FaceNormalDatas, Temp.idxDatas);
			float now = (float)i / SpMax;
			float alpha = now * 3.14f * 2.f;
			glm::vec3 pos = glm::vec3{ ObjCircleLine->scale.x/2.f * sin(alpha),0.f,ObjCircleLine->scale.x/2.f * cos(alpha) };
			ObjSpheres[i]->setPos(pos);
			ObjSpheres[i]->setScale(glm::vec3(10.f));
			ObjSpheres[i]->GetDataForOBJLoader(Temp);
			ObjSpheres[i]->load();
			std::string lightLniformName{ "lightSources[" + std::to_string(i) + ("]") };
			dynamic_cast<Light*>(ObjSpheres[i].get())->sendLightDataUniform(diffuseShader, lightLniformName);
			ObjSpheres[i]->objShader = NormalShdrProgram;
			ObjSpheres[i]->normalVectorShader = NormalShdrProgram;
			
		}
	}
}

void TestScene::Update(double dt)
{
	const int SpMax{ 16 };
	static const float maxForRandom{ 1000 };
	static bool isRotation{true};
	if (isRotation == true) {
		time += dt;
	}
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	{
		//scenario
		if (ImGui::Button("Scenario 1")) {
			IsScenario3 = false;
			for (int i = 0; i < SpMax; ++i) {
				dynamic_cast<Light*>(ObjSpheres[i].get())->refLightData().ambient = glm::vec3{1.};
			}
		}
		if (ImGui::Button("Scenario 2")) {
			IsScenario3 = false;
			for (int i = 0; i < SpMax; ++i) {
				const float randR{ ((int)(time * (i * i + 10)) % (int)maxForRandom) / maxForRandom };
				const float randG{ ((int)(time * (i * i + 700)) % (int)maxForRandom) / maxForRandom };
				const float randB{ ((int)(time * (i * i + 250)) % (int)maxForRandom) / maxForRandom };

				dynamic_cast<Light*>(ObjSpheres[i].get())->refLightData().ambient = glm::vec3{ randR,randG,randB };
			}
		}
		if (ImGui::Button("Scenario 3")) {
			IsScenario3 = true;
			for (int i = 0; i < SpMax; ++i) {
				const float randR{ ((int)(time * (i * i + 10)) % (int)maxForRandom) / maxForRandom };
				const float randG{ ((int)(time * (i * i + 700)) % (int)maxForRandom) / maxForRandom };
				const float randB{ ((int)(time * (i * i + 250)) % (int)maxForRandom) / maxForRandom };

				dynamic_cast<Light*>(ObjSpheres[i].get())->refLightData().ambient = glm::vec3{ randR,randG,randB };
			}
		}
	}
	{
		static char* currItem{};
		if (ImGui::BeginCombo("Camera choose", currItem) == true) {
			bool one{ false };
			bool two{ false };

			ImGui::Selectable("Free", &one);
			ImGui::Selectable("Ball", &two);
			//static float saveHeight{10.f};
			if (one == true) {

			}
			if (two == true) {

			}

			ImGui::EndCombo();
		}
	}
	if (ImGui::Button("Reload shader")) {
		//shader reload
	}
	if (ImGui::BeginMenu("Obj control")) {
		ImGui::SliderInt("Normal", &normalDrawState, 0, 3);
		ImGui::SliderInt("OBJ Number", &nowObj, 0, 4);
		ImGui::SliderFloat("OBJ rotate", &ObjRotate, 0.f, 6.28f);
		ImGui::EndMenu();

	}
	if (ImGui::BeginMenu("Light control")) {
		ImGui::SliderFloat("Orbit speed", &orbitSpeed, 0.f, 2.f);
		// toggle rotation on off
		ImGui::Checkbox("Rotation on off", &isRotation);

		ImGui::SliderInt("Orbit number", &SpCurrNum, 1, SpMax);
		ImGui::EndMenu();
	}

	for (int i = 0; i < SpCurrNum; ++i) {
		float now = (float)i / SpCurrNum;
		float alpha = now * 3.14f * 2.f + (float)(time)* orbitSpeed;
		glm::vec3 pos = glm::vec3{ ObjCircleLine->scale.x/2.f * sin(alpha),0.f,ObjCircleLine->scale.x/2.f * cos(alpha) };
		ObjSpheres[i]->setPos(pos);
		ObjSpheres[i]->update(dt);
	}
	Obj[nowObj]->setRotate({0.f,ObjRotate,0.f});
	Obj[nowObj]->update(dt);
	ObjCircleLine->update(dt);
	ObjPlane->update(dt);
}

void TestScene::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.3f, 0.3f, 0.3f, 1);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);


	diffuseShader->sendUniform1iv("numCurLights", SpCurrNum);
	for (int i = 0; i < SpCurrNum;++i) {
		std::string lightLniformName{ "lightSources[" + std::to_string(i) + ("]") };
		dynamic_cast<Light*>(ObjSpheres[i].get())->sendLightDataUniform(diffuseShader, lightLniformName);
		ObjSpheres[i]->draw();
		if (normalDrawState != 0) {
			ObjSpheres[i]->drawNormal(normalDrawState);
		}
	}
	Obj[nowObj]->draw();
	ObjCircleLine->draw();

	if (normalDrawState != 0) {
		Obj[nowObj]->drawNormal(normalDrawState);
	}

	ObjPlane->draw();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void TestScene::Unload()
{
	for (auto& o : Obj) {
		o->unload();
	}
	for (auto& ObjSphere : ObjSpheres) {
		ObjSphere->unload();
	}
	ObjCircleLine->unload();
	ObjPlane->unload();
}
