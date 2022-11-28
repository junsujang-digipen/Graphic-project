/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: TestScene.cpp
Purpose: Scene for testing objs and loader and shaders
Language: c++
Platform: x64
Project: junsu.jang, CS300, Assignment 3 - Dynamic Environment Mapping 
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
#include "BasicObject.h"
#include "Light.h"
#include <stb_image.h>
#include "Texture.h"
#include "Random.h"

TestScene::TestScene(): Scene(),SpCurrNum(1)
{
}
TestScene::~TestScene() {
	
}

void TestScene::Load()
{
	Camera camera{};
	camera.init();
	camera.setPosition({ 0,130,150 });
	camera.setRotate({ glm::radians(-45.f),glm::radians(/*36.f*/0.f),0.f });
	camera.Update(0.01);
	WTC = { camera.getWorldToCameraToNDC() };
	////WTC = { glm::perspective <float > (glm::radians(45.f),800 / 600, 0.001f,1000.f) };
	
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

		const std::string vLightshdr{ ShaderHelper::getShaderSourceFromFile("../Shaders/F_LightingShaderForLight.vert") };
		const std::string fLightshdr{ ShaderHelper::getShaderSourceFromFile("../Shaders/F_LightingShaderForLight.frag") };

		const std::string commonFunctionShdr{ ShaderHelper::getShaderSourceFromFile("../Shaders/FF_CommonFunctions.glsl") };
		const std::string shaderVersion{ ShaderHelper::getShaderSourceFromFile("../Shaders/FB_Version.glsl") };
		const std::string LightSturctShdr{ ShaderHelper::getShaderSourceFromFile("../Shaders/FS_LightStruct.glsl") };
		const std::string MaterialStructShdr{ ShaderHelper::getShaderSourceFromFile("../Shaders/FS_MaterialStruct.glsl") };
	//

	diffuseShader = std::make_shared<Shader>("Diffuse Shader");
	NormalShdrProgram = std::make_shared<Shader>("Normal Shader");
	LightShader = std::make_shared<Shader>("Light source Shader");
	//Shader
	{
		GLuint vshdr = diffuseShader->compileShader(GL_VERTEX_SHADER, { shaderVersion, commonFunctionShdr, vPhongShadingshdr });
		GLuint fshdr = diffuseShader->compileShader(GL_FRAGMENT_SHADER, { shaderVersion, LightSturctShdr, MaterialStructShdr, commonFunctionShdr,fPhongShadingshdr });
		diffuseShader->attachShader(vshdr);
		diffuseShader->attachShader(fshdr);
		diffuseShader->linkProgram();
		diffuseShader->sendUniformMatrix4fv("WorldToCameraMat", WTC);
	}
	//shader normal
	{
		GLuint vshdr = NormalShdrProgram->compileShader(GL_VERTEX_SHADER, { VshdrNormal });
		GLuint fshdr = NormalShdrProgram->compileShader(GL_FRAGMENT_SHADER, {FshdrNormal});
		NormalShdrProgram->attachShader(vshdr);
		NormalShdrProgram->attachShader(fshdr);
		NormalShdrProgram->linkProgram();
		NormalShdrProgram->sendUniformMatrix4fv("WorldToCameraMat", WTC);
	}
	//light shader 
	{
		GLuint vshdr = LightShader->compileShader(GL_VERTEX_SHADER, { shaderVersion, vLightshdr });
		GLuint fshdr = LightShader->compileShader(GL_FRAGMENT_SHADER, { shaderVersion,fLightshdr });
		LightShader->attachShader(vshdr);
		LightShader->attachShader(fshdr);
		LightShader->linkProgram();
		LightShader->sendUniformMatrix4fv("WorldToCameraMat", WTC);
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
			Obj.push_back(std::make_shared<BasicObject>());
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
		ObjPlane = std::make_shared<BasicObject>();
		OBJLoader Temp{};
		Temp.FileLoad("cube2.obj");
		ObjPlane->setPos(glm::vec3(0.f, -75.f, 0.f));
		ObjPlane->setScale(glm::vec3(350.f,10.f, 350.f));
		ObjPlane->GetDataForOBJLoader(Temp);
		ObjPlane->load();

		ObjPlane->objShader = diffuseShader;
		ObjPlane->normalVectorShader = NormalShdrProgram;
	}
	//Orbit
	{
		ObjCircleLine = std::make_shared<BasicObject>();
		OBJLoader Temp{};
		MakeCircleLineData(100, Temp.VertexDatas, Temp.VertexNormalDatas, Temp.FaceNormalDatas, Temp.idxDatas);
		Temp.primitive_type = GL_LINES;
		ObjCircleLine->setPos(glm::vec3(0.f, 0.f, 0.f));
		ObjCircleLine->setScale(glm::vec3(200.f));
		ObjCircleLine->GetDataForOBJLoader(Temp);
		ObjCircleLine->load();

		ObjCircleLine->objShader = LightShader;
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
			ObjSpheres[i]->objShader = LightShader;
			ObjSpheres[i]->normalVectorShader = NormalShdrProgram;
			
		}
	}
}


void TestScene::Update(double dt)
{
	const int SpMax{ 16 };
	static const float maxForRandom{ 50 };
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
			nowScenario = 0;
			for (int i = 0; i < SpMax; ++i) {
				dynamic_cast<Light*>(ObjSpheres[i].get())->resetLightData();
			}
		}
		if (ImGui::Button("Scenario 2")) {
			nowScenario = 1;

			for (int i = 0; i < SpMax; ++i) {
				dynamic_cast<Light*>(ObjSpheres[i].get())->resetLightData();
				
				const float randR{ (sin(random() + ObjSpheres[i]->pos.x) +1.f)/2.f };
				const float randG{ (sin(random() +ObjSpheres[i]->pos.y) + 1.f) / 2.f };
				const float randB{ (sin(random()+ ObjSpheres[i]->pos.z ) + 1.f) / 2.f };
				//std::cout << randR << " " << randG << " " << randB << std::endl;
				dynamic_cast<Light*>(ObjSpheres[i].get())->refLightData().diffuse = glm::vec3{ randR,randG,randB };
			}
		}
		if (ImGui::Button("Scenario 3")) {
			nowScenario = 2;
			for (int i = 0; i < SpMax; ++i) {
				const float randR{ (sin(random() + ObjSpheres[i]->pos.x) + 1.f) / 2.f };
				const float randG{ (sin(random() + ObjSpheres[i]->pos.y) + 1.f) / 2.f };
				const float randB{ (sin(random() + ObjSpheres[i]->pos.z) + 1.f) / 2.f };

				dynamic_cast<Light*>(ObjSpheres[i].get())->refLightData().diffuse = glm::vec3{ randR,randG,randB };
			}
		}
	}
	{
		if (ImGui::BeginMenu("Shader control")) {
			static char* currItem{};
			bool one{ false };
			bool two{ false };
			bool three{ false };
			static int choosenNum{2};

			if (ImGui::BeginCombo("Shader Choose", currItem) == true) {

				ImGui::Selectable("Phong Lighting", &one);
				ImGui::Selectable("Phong Shading", &two);
				ImGui::Selectable("Blinn Shading", &three);
				if (one == true) { choosenNum = 0; }
				else if (two == true) { choosenNum = 1; }
				else if (three == true) { choosenNum = 2; }
				ImGui::EndCombo();
			}
			if (ImGui::Button("Reload shader")) {
				//shader reload
				std::cout << "Shader reload" << std::endl;
				diffuseShader->initProgram();

				//lightings
				const std::string commonFunctionShdr{ ShaderHelper::getShaderSourceFromFile("../Shaders/FF_CommonFunctions.glsl") };
				const std::string shaderVersion{ ShaderHelper::getShaderSourceFromFile("../Shaders/FB_Version.glsl") };
				const std::string LightSturctShdr{ ShaderHelper::getShaderSourceFromFile("../Shaders/FS_LightStruct.glsl") };
				const std::string MaterialStructShdr{ ShaderHelper::getShaderSourceFromFile("../Shaders/FS_MaterialStruct.glsl") };
				//

				if (choosenNum == 0) {
					const std::string vPhongLightingshdr{ ShaderHelper::getShaderSourceFromFile("../Shaders/F_PhongLightingShader.vert") };
					const std::string fPhongLightingshdr{ ShaderHelper::getShaderSourceFromFile("../Shaders/F_PhongLightingShader.frag") };
					GLuint vshdr = diffuseShader->compileShader(GL_VERTEX_SHADER, { shaderVersion,LightSturctShdr, MaterialStructShdr, commonFunctionShdr, vPhongLightingshdr });
					GLuint fshdr = diffuseShader->compileShader(GL_FRAGMENT_SHADER, { shaderVersion, commonFunctionShdr,fPhongLightingshdr });
					diffuseShader->attachShader(vshdr);
					diffuseShader->attachShader(fshdr);
					diffuseShader->linkProgram();
				}
				else if (choosenNum == 1) {
					const std::string vPhongShadingshdr{ ShaderHelper::getShaderSourceFromFile("../Shaders/F_PhongShadingShader.vert") };
					const std::string fPhongShadingshdr{ ShaderHelper::getShaderSourceFromFile("../Shaders/F_PhongShadingShader.frag") };
					GLuint vshdr = diffuseShader->compileShader(GL_VERTEX_SHADER, { shaderVersion, commonFunctionShdr, vPhongShadingshdr });
					GLuint fshdr = diffuseShader->compileShader(GL_FRAGMENT_SHADER, { shaderVersion, LightSturctShdr, MaterialStructShdr, commonFunctionShdr,fPhongShadingshdr });
					diffuseShader->attachShader(vshdr);
					diffuseShader->attachShader(fshdr);
					diffuseShader->linkProgram();
				}
				else if (choosenNum == 2) {
					const std::string vBlinnShadingshdr{ ShaderHelper::getShaderSourceFromFile("../Shaders/F_BlinnShadingShader.vert") };
					const std::string fBlinnShadingshdr{ ShaderHelper::getShaderSourceFromFile("../Shaders/F_BlinnShadingShader.frag") };
					GLuint vshdr = diffuseShader->compileShader(GL_VERTEX_SHADER, { shaderVersion, commonFunctionShdr, vBlinnShadingshdr });
					GLuint fshdr = diffuseShader->compileShader(GL_FRAGMENT_SHADER, { shaderVersion, LightSturctShdr, MaterialStructShdr, commonFunctionShdr,fBlinnShadingshdr });
					diffuseShader->attachShader(vshdr);
					diffuseShader->attachShader(fshdr);
					diffuseShader->linkProgram();
				}
				diffuseShader->sendUniformMatrix4fv("WorldToCameraMat", WTC);
				textures[0]->sendTextureDataUniform(diffuseShader, "MatDiffuse");
				textures[1]->sendTextureDataUniform(diffuseShader, "MatSpecular");
			}
			ImGui::EndMenu();
		}
	}
	{
		if (ImGui::BeginMenu("Global control")) {
			const float multConst{10000.f};
			static glm::vec3 Attenuation{ 1.f*multConst,0.009f * multConst,0.001f * multConst };
			ImGui::InputFloat3(std::string("Attenuation coefficients").c_str(), &Attenuation.r);
			const glm::vec3 Att{ Attenuation / multConst };
			for (int i = 0; i < SpMax; ++i) {
				dynamic_cast<Light*>(ObjSpheres[i].get())->refLightData().constant = Att.r;
				dynamic_cast<Light*>(ObjSpheres[i].get())->refLightData().linear = Att.g;
				dynamic_cast<Light*>(ObjSpheres[i].get())->refLightData().quadratic = Att.b;
			}
			{
				static glm::vec3 fogCol{ 0.f,0.f,0.f };
				ImGui::SliderFloat3(std::string("Fog color").c_str(), &fogCol.r, 0.0f, 1.f);
				diffuseShader->sendUniform3fv("fogCol", fogCol);
				static float fogNear{ 0.1f };
				static float fogFar{ 1000.f };
				ImGui::SliderFloat(std::string("Fog near").c_str(), &fogNear, 0.f, fogFar);
				diffuseShader->sendUniform1fv("cameraNear", fogNear);
				ImGui::SliderFloat(std::string("Fog far").c_str(), &fogFar, fogNear, 1000.f);
				diffuseShader->sendUniform1fv("cameraFar", fogFar);
			}

			static glm::vec3 GAmbient{ 0.0f,0.0f,0.0f };
			ImGui::SliderFloat3(std::string("Grobal ambient").c_str(), &GAmbient.r, 0.0f, 1.f);
			diffuseShader->sendUniform3fv("GAmbient", GAmbient);

			ImGui::EndMenu();
		}
	}
	{
		if (ImGui::BeginMenu("Texture control")) {

			
			ImGui::SliderFloat3("Texture ambient", &ObjAmbient.x, 0.f, 1.f);
			ImGui::SliderFloat3("Texture emissive", &ObjEmissive.x, 0.f, 1.f);

			ImGui::Checkbox("GPU texture mapping", &IsGPUtextureMapping);
			ImGui::Checkbox("Position Entity mapping", &IsPositionEntityMapping);
			for (auto&o:Obj) {
				o->IsPositionEntity = IsPositionEntityMapping;
			}
			static char* currItem{};
			if (ImGui::BeginCombo("Texture mapping Choose", currItem) == true) {
				bool one{ false };
				bool two{ false };
				bool three{ false };
				bool four{ false };
				bool five{ false };

				ImGui::Selectable("Spherical", &one);
				ImGui::Selectable("Cylindrical", &two);
				ImGui::Selectable("Planar", &three);
				ImGui::Selectable("Planar6", &four);
				ImGui::Selectable("None", &five);

				if (one == true) {
					OBJtextureMappingNum = 0;
					Obj[nowObj]->calcSphereTexCoord();
				}
				if (two == true) {
					OBJtextureMappingNum = 1;
					Obj[nowObj]->calcCylindricalTexCoord();
				}
				if (three == true) {
					OBJtextureMappingNum = 2;
					Obj[nowObj]->calcPlanarTexCoord();
				}
				if (four == true) {
					OBJtextureMappingNum = 3;
					Obj[nowObj]->calcCubeMapTexCoord();
				}
				if (five == true) {
					OBJtextureMappingNum = 4;
				}

				ImGui::EndCombo();
			}
			ImGui::EndMenu();
		}
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
		if (nowScenario == 0) {
			static glm::vec3 ambient{0.1f};
			static glm::vec3 diffuse{1.f};
			static glm::vec3 specular{0.5f};
			static float innerCut{0.6f};
			static float outerCut{0.5f};
			static float fallOff{1.f};
			static int type{1};
			ImGui::SliderFloat3(std::string("light ambient").c_str(), &ambient.r, 0.f, 1.f);
			ImGui::SliderFloat3(std::string("light diffuse").c_str(), &diffuse.r, 0.f, 1.f);
			ImGui::SliderFloat3(std::string("light specular").c_str(), &specular.r, 0.f, 1.f);
			ImGui::SliderFloat(std::string("light innerCut").c_str(), &innerCut, 0.f, 1.f);
			ImGui::SliderFloat(std::string("light outerCut").c_str(), &outerCut, 0.f, innerCut);
			ImGui::SliderFloat(std::string("light fallOff").c_str(), &fallOff, 1.f, 10.f);
			ImGui::SliderInt(std::string("light type").c_str(), &type, 0, 2);

			for (int i = 0; i < SpMax; ++i) {
				dynamic_cast<Light*>(ObjSpheres[i].get())->refLightData().ambient = ambient;
				dynamic_cast<Light*>(ObjSpheres[i].get())->refLightData().diffuse = diffuse;
				dynamic_cast<Light*>(ObjSpheres[i].get())->refLightData().specular = specular;
				dynamic_cast<Light*>(ObjSpheres[i].get())->refLightData().innerCut = innerCut;
				dynamic_cast<Light*>(ObjSpheres[i].get())->refLightData().outerCut = outerCut;
				dynamic_cast<Light*>(ObjSpheres[i].get())->refLightData().fallOff = fallOff;
				dynamic_cast<Light*>(ObjSpheres[i].get())->refLightData().type = type;
			}
		}
		else if (nowScenario == 1) {
			static float innerCut{ 0.6f };
			static float outerCut{ 0.5f };
			static float fallOff{ 1.f };
			static int type{ 1 };
			ImGui::SliderInt(std::string("light type").c_str(), &type, 0, 2);

			ImGui::SliderFloat(std::string("light innerCut").c_str(), &innerCut, 0.f, 1.f);
			ImGui::SliderFloat(std::string("light outerCut").c_str(), &outerCut, 0.f, innerCut);
			ImGui::SliderFloat(std::string("light fallOff").c_str(), &fallOff, 1.f, 10.f);

			for (int i = 0; i < SpMax; ++i) {
				dynamic_cast<Light*>(ObjSpheres[i].get())->refLightData().innerCut = innerCut;
				dynamic_cast<Light*>(ObjSpheres[i].get())->refLightData().outerCut = outerCut;
				dynamic_cast<Light*>(ObjSpheres[i].get())->refLightData().fallOff = fallOff;
				dynamic_cast<Light*>(ObjSpheres[i].get())->refLightData().type = type;
			}

			for (int i = 0; i < SpCurrNum; ++i) {
				if (ImGui::BeginMenu(std::string("light " + std::to_string(i) + " control").c_str())) {
					ImGui::SliderFloat3(std::string("light " + std::to_string(i) + " ambient").c_str(), &dynamic_cast<Light*>(ObjSpheres[i].get())->refLightData().ambient.r, 0.f, 1.f);
					ImGui::SliderFloat3(std::string("light " + std::to_string(i) + " diffuse").c_str(), &dynamic_cast<Light*>(ObjSpheres[i].get())->refLightData().diffuse.r, 0.f, 1.f);
					ImGui::SliderFloat3(std::string("light " + std::to_string(i) + " specular").c_str(), &dynamic_cast<Light*>(ObjSpheres[i].get())->refLightData().specular.r, 0.f, 1.f);
					ImGui::EndMenu();
				}
			}
		}
		else if (nowScenario == 2) {
			for (int i = 0; i < SpCurrNum; ++i) {
				if (ImGui::BeginMenu(std::string("light " + std::to_string(i) + " control").c_str())) {
					ImGui::SliderFloat3(std::string("light " + std::to_string(i) + " ambient").c_str(), &dynamic_cast<Light*>(ObjSpheres[i].get())->refLightData().ambient.r, 0.f, 1.f);
					ImGui::SliderFloat3(std::string("light " + std::to_string(i) + " diffuse").c_str(), &dynamic_cast<Light*>(ObjSpheres[i].get())->refLightData().diffuse.r, 0.f, 1.f);
					ImGui::SliderFloat3(std::string("light " + std::to_string(i) + " specular").c_str(), &dynamic_cast<Light*>(ObjSpheres[i].get())->refLightData().specular.r, 0.f, 1.f);
					ImGui::SliderFloat(std::string("light " + std::to_string(i) + " innerCut").c_str(), &dynamic_cast<Light*>(ObjSpheres[i].get())->refLightData().innerCut, 0.f, 1.f);
					ImGui::SliderFloat(std::string("light " + std::to_string(i) + " outerCut").c_str(), &dynamic_cast<Light*>(ObjSpheres[i].get())->refLightData().outerCut, 0.f, dynamic_cast<Light*>(ObjSpheres[i].get())->refLightData().innerCut);
					ImGui::SliderFloat(std::string("light " + std::to_string(i) + " fallOff").c_str(), &dynamic_cast<Light*>(ObjSpheres[i].get())->refLightData().fallOff, 1.f, 10.f);
					ImGui::SliderInt(std::string("light " + std::to_string(i) + " type").c_str(), &dynamic_cast<Light*>(ObjSpheres[i].get())->refLightData().type, 0, 2);
					ImGui::EndMenu();
				}
			}
		}
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
	for (int i = 0; i < SpCurrNum; ++i) {
		std::string lightLniformName{ "lightSources[" + std::to_string(i) + ("]") };
		dynamic_cast<Light*>(ObjSpheres[i].get())->setLightDirection(Obj[nowObj]->pos - ObjSpheres[i]->pos);
		dynamic_cast<Light*>(ObjSpheres[i].get())->sendLightDataUniform(diffuseShader, lightLniformName);
		ObjSpheres[i]->draw();
		if (normalDrawState != 0) {
			ObjSpheres[i]->drawNormal(normalDrawState);
		}
	}

	{
		diffuseShader->sendUniform1iv("texMappingType", OBJtextureMappingNum);
		diffuseShader->sendUniform3fv("MatAmbient", ObjAmbient);
		diffuseShader->sendUniform3fv("MatEmissive", ObjEmissive);
		diffuseShader->sendUniform1iv("isGPUMapping", IsGPUtextureMapping);
		diffuseShader->sendUniform1iv("isPositionEntity", IsPositionEntityMapping);
		diffuseShader->sendUniform3fv("boundMax", Obj[nowObj]->boundBoxMax);
		diffuseShader->sendUniform3fv("boundMin", Obj[nowObj]->boundBoxMin);
		Obj[nowObj]->draw();
		ObjCircleLine->draw();

		if (normalDrawState != 0) {
			Obj[nowObj]->drawNormal(normalDrawState);
		}
	}

	{
		glm::vec3 PlaneAmbient{ 0.5f };
		glm::vec3 PlaneEmissive{ 0.f };
		diffuseShader->sendUniform1iv("texMappingType", 4);
		diffuseShader->sendUniform3fv("MatAmbient", PlaneAmbient);
		diffuseShader->sendUniform3fv("MatEmissive", PlaneEmissive);
		ObjPlane->draw();
	}
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void TestScene::Unload()
{
	for (auto& o : Obj) {
		o->unload();
	}
	Obj.clear();
	for (auto& ObjSphere : ObjSpheres) {
		ObjSphere->unload();
	}
	ObjSpheres.clear();
	ObjCircleLine->unload();
	ObjPlane->unload();
	textures.clear();
}
