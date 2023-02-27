/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: TestScene2.cpp
Purpose: Scene for testing objs and loader and shaders
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 1 - Hybrid Rendering
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 11/22/2022
End Header --------------------------------------------------------*/
#include "pch.h"
#include "TestScene2.h"
#include "OBJLoader.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Camera.h"

#include "ShaderManager.h"
#include "Shader.h"

#include "Renderer.h"
#include "BasicObject.h"
#include "SkyBox.h"
#include "Light.h"
#include <Externals/include/stb_image.h>//??

#include "TextureManager.h"
#include "Texture.h"

#include "Random.h"
#include "RenderTextureCamera.h"

#include <GAPP.h>
#include <Externals/include/GL/glew.h>
#include <Externals/include/GLFW/glfw3.h>
#include <FSQ.h>
#include <Imgui/imgui.h>

TestScene2::TestScene2() : Scene(), SpCurrNum(1)
{
}
TestScene2::~TestScene2() {
	delete camera;
}

void TestScene2::Load()
{
	camera = {new Camera};
	camera->init();
	//camera->setPosition({ 0,130,150 });
	camera->setPosition({ 0,0,150 });
	//camera->setRotate({ glm::radians(-45.f),glm::radians(/*36.f*/0.f),0.f });
	camera->Update(0.01);
	WTC = { camera->getWorldToCameraToNDC() };
	////WTC = { glm::perspective <float > (glm::radians(45.f),800 / 600, 0.001f,1000.f) };

	const std::string Vshdr{ ShaderHelper::getShaderSourceFromFile("../Shaders/C_DiffuseShader.vert") };
	const std::string Fshdr{ ShaderHelper::getShaderSourceFromFile("../Shaders/C_DiffuseShader.frag") };
	const std::string VshdrNormal{ ShaderHelper::getShaderSourceFromFile("../Shaders/C_BasicShader.vert") };
	const std::string FshdrNormal{ ShaderHelper::getShaderSourceFromFile("../Shaders/C_BasicShader.frag") };
	
	//Geometry stage shdr
	const std::string vGeometryStageshdr{ ShaderHelper::getShaderSourceFromFile("../Shaders/F_GeometryStageShader.vert") };
	const std::string fGeometryStageshdr{ ShaderHelper::getShaderSourceFromFile("../Shaders/F_GeometryStageShader.frag") };

	//Lighting stage shdr
	const std::string vLightingStageshdr{ ShaderHelper::getShaderSourceFromFile("../Shaders/F_LightingStageShader.vert") };
	const std::string fLightingStageshdr{ ShaderHelper::getShaderSourceFromFile("../Shaders/F_LightingStageShader.frag") };

	const std::string vLightshdr{ ShaderHelper::getShaderSourceFromFile("../Shaders/F_LightingShaderForLight.vert") };
	const std::string fLightshdr{ ShaderHelper::getShaderSourceFromFile("../Shaders/F_LightingShaderForLight.frag") };

	const std::string commonFunctionShdr{ ShaderHelper::getShaderSourceFromFile("../Shaders/FF_CommonFunctions.glsl") };
	const std::string shaderVersion{ ShaderHelper::getShaderSourceFromFile("../Shaders/FB_Version.glsl") };
	const std::string LightSturctShdr{ ShaderHelper::getShaderSourceFromFile("../Shaders/FS_LightStruct.glsl") };
	const std::string MaterialStructShdr{ ShaderHelper::getShaderSourceFromFile("../Shaders/FS_MaterialStruct.glsl") };
	//


	// Create the FBO
	glGenFramebuffers(1, &m_gBufferFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_gBufferFBO);
	// Create the gbuffer textures
	glGenTextures(renderTextureNum, m_textures);
	glGenTextures(1, &m_depthTexture);
	for (unsigned int i = 0; i < renderTextureNum; i++)
	{
		glBindTexture(GL_TEXTURE_2D, m_textures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, (int)engine->GetWindowSize().x, (int)engine->GetWindowSize().y, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_textures[i], 0);
	}
	// Now bind the depth attachment
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT, (int)engine->GetWindowSize().x, (int)engine->GetWindowSize().y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

	unsigned int DrawBuffers[renderTextureNum] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5 };
	glDrawBuffers(renderTextureNum, DrawBuffers);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) !=GL_FRAMEBUFFER_COMPLETE)
	{
		printf("gbuffer is incomplete!");
		return ;
	}
	// restore default FBO
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	shaderManager->makeShader("Geometry stage Shader");
	{
		GLuint vshdr = shaderManager->getShader("Geometry stage Shader")->compileShader(ShaderType::VERTEX_SHADER, { shaderVersion, commonFunctionShdr, vGeometryStageshdr });
		GLuint fshdr = shaderManager->getShader("Geometry stage Shader")->compileShader(ShaderType::FRAGMENT_SHADER, { shaderVersion, MaterialStructShdr, commonFunctionShdr,fGeometryStageshdr });
		shaderManager->getShader("Geometry stage Shader")->attachShader(vshdr);
		shaderManager->getShader("Geometry stage Shader")->attachShader(fshdr);
		shaderManager->getShader("Geometry stage Shader")->linkProgram();
		shaderManager->getShader("Geometry stage Shader")->sendUniformMatrix4fv("WorldToCameraMat", WTC);
	}
	shaderManager->makeShader("Lighting stage Shader");
	{
		GLuint vshdr = shaderManager->getShader("Lighting stage Shader")->compileShader(ShaderType::VERTEX_SHADER, { shaderVersion, vLightingStageshdr });
		GLuint fshdr = shaderManager->getShader("Lighting stage Shader")->compileShader(ShaderType::FRAGMENT_SHADER, { shaderVersion, MaterialStructShdr,LightSturctShdr, commonFunctionShdr,fLightingStageshdr });
		shaderManager->getShader("Lighting stage Shader")->attachShader(vshdr);
		shaderManager->getShader("Lighting stage Shader")->attachShader(fshdr);
		shaderManager->getShader("Lighting stage Shader")->linkProgram();
	}
	
	//shader normal
	NormalShdrProgram = shaderManager->makeShader("Normal Shader");
	{
		GLuint vshdr = NormalShdrProgram->compileShader(ShaderType::VERTEX_SHADER, { VshdrNormal });
		GLuint fshdr = NormalShdrProgram->compileShader(ShaderType::FRAGMENT_SHADER, { FshdrNormal });
		NormalShdrProgram->attachShader(vshdr);
		NormalShdrProgram->attachShader(fshdr);
		NormalShdrProgram->linkProgram();
		NormalShdrProgram->sendUniformMatrix4fv("WorldToCameraMat", WTC);
	}
	
	//light shader 
	LightShader = shaderManager->makeShader("Light source Shader");
	{
		GLuint vshdr = LightShader->compileShader(ShaderType::VERTEX_SHADER, { shaderVersion, vLightshdr });
		GLuint fshdr = LightShader->compileShader(ShaderType::FRAGMENT_SHADER, { shaderVersion,fLightshdr });
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
		for (int i = 0; i < 5; ++i) {
			OBJLoader objLoader{};
			objLoader.FileLoad(filepaths[i]);
			Obj.push_back(std::make_shared<BasicObject>());
			Obj[i]->setScale(glm::vec3(50.f));
			Obj[i]->GetDataForOBJLoader(objLoader);
			Obj[i]->load();

			Obj[i]->objShader = shaderManager->getShader("Geometry stage Shader");
			Obj[i]->normalVectorShader = NormalShdrProgram;
		}

		{//texture
			textureManager->EnrollTexture("metal_roof_diff_512x512", "metal_roof_diff_512x512.png");
			textureManager->EnrollTexture("metal_roof_spec_512x512", "metal_roof_spec_512x512.png");

			textureManager->SetTextureMappingData("Geometry stage Shader", "metal_roof_diff_512x512", "MatDiffuse");
			textureManager->SetTextureMappingData("Geometry stage Shader", "metal_roof_spec_512x512", "MatSpecular");
			textureManager->bindTextures("Geometry stage Shader", shaderManager);
		}
	}
	{//screen rect
		screenRect = std::make_shared<FSQ>();
		screenRect->load();

		screenRect->objShader = shaderManager->getShader("Lighting stage Shader");
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
			glm::vec3 pos = glm::vec3{ ObjCircleLine->scale.x / 2.f * sin(alpha),0.f,ObjCircleLine->scale.x / 2.f * cos(alpha) };
			ObjSpheres[i]->setPos(pos);
			ObjSpheres[i]->setScale(glm::vec3(10.f));
			ObjSpheres[i]->GetDataForOBJLoader(Temp);
			ObjSpheres[i]->load();
			std::string lightLniformName{ "lightSources[" + std::to_string(i) + ("]") };
			dynamic_cast<Light*>(ObjSpheres[i].get())->sendLightDataUniform(shaderManager->getShader("Lighting stage Shader"), lightLniformName);
			ObjSpheres[i]->objShader = LightShader;
			ObjSpheres[i]->normalVectorShader = NormalShdrProgram;

		}
	}
}


void TestScene2::Update(double dt)
{
	const int SpMax{ 16 };
	static const float maxForRandom{ 50 };
	static bool isRotation{ true };
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

				const float randR{ (sin(random() + ObjSpheres[i]->pos.x) + 1.f) / 2.f };
				const float randG{ (sin(random() + ObjSpheres[i]->pos.y) + 1.f) / 2.f };
				const float randB{ (sin(random() + ObjSpheres[i]->pos.z) + 1.f) / 2.f };
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
		if (ImGui::CollapsingHeader("Global control")) {
			const float multConst{ 10000.f };
			static glm::vec3 Attenuation{ 1.f * multConst,0.009f * multConst,0.001f * multConst };
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
				shaderManager->getShader("Lighting stage Shader")->sendUniform3fv("fogCol", fogCol);
				static float fogNear{ 0.1f };
				static float fogFar{ 1000.f };
				ImGui::SliderFloat(std::string("Fog near").c_str(), &fogNear, 0.f, fogFar);
				shaderManager->getShader("Lighting stage Shader")->sendUniform1fv("cameraNear", fogNear);
				ImGui::SliderFloat(std::string("Fog far").c_str(), &fogFar, fogNear, 1000.f);
				shaderManager->getShader("Lighting stage Shader")->sendUniform1fv("cameraFar", fogFar);
			}

			static glm::vec3 GAmbient{ 0.0f,0.0f,0.0f };
			ImGui::SliderFloat3(std::string("Grobal ambient").c_str(), &GAmbient.r, 0.0f, 1.f);
			shaderManager->getShader("Lighting stage Shader")->sendUniform3fv("GAmbient", GAmbient);

		}
	}
	{
		if (ImGui::CollapsingHeader("Texture control")) {


			ImGui::SliderFloat3("Texture ambient", &ObjAmbient.x, 0.f, 1.f);
			ImGui::SliderFloat3("Texture emissive", &ObjEmissive.x, 0.f, 1.f);

			ImGui::Checkbox("GPU texture mapping", &IsGPUtextureMapping);
			ImGui::Checkbox("Position Entity mapping", &IsPositionEntityMapping);
			for (auto& o : Obj) {
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
		}
	}
	if (ImGui::CollapsingHeader("Obj control")) {
		ImGui::SliderInt("Normal", &normalDrawState, 0, 3);
		ImGui::SliderInt("OBJ Number", &nowObj, 0, 4);
		ImGui::SliderFloat("OBJ rotate", &ObjRotate, 0.f, 6.28f);
	}
	 
	if (ImGui::CollapsingHeader("Camera control")) {
		static glm::vec3 cameraPosition{ camera->getCameraPos() };
		static glm::vec3 HSV_Value{ 128.f,128.f,128.f };
		ImGui::SliderFloat3("Camera position ", &cameraPosition.x, -1000.f, 1000.f);
		camera->setPosition(cameraPosition);
		shaderManager->getShader("Lighting stage Shader")->sendUniform3fv("cameraPos", cameraPosition);
		shaderManager->getShader("Geometry stage Shader")->sendUniform3fv("cameraPos", camera->getCameraPos());

		static glm::vec3 cameraRotation{ camera->getAngle() };
		ImGui::SliderFloat("Camera Pitch rotate ", &cameraRotation.x, -1.57f, 1.57f);
		ImGui::SliderFloat("Camera Yaw rotate", &cameraRotation.y, -3.14f, 3.14f);
		//ImGui::SliderFloat("Camera Roll rotate", &cameraRotation.z, -3.14f, 3.14f);
		camera->updateRotate(cameraRotation - camera->getAngle());
		camera->Update(0.01);
		WTC = camera->getWorldToCameraToNDC();
		shaderManager->getShader("Geometry stage Shader")->sendUniformMatrix4fv("WorldToCameraMat", WTC);
		NormalShdrProgram->sendUniformMatrix4fv("WorldToCameraMat", WTC);
		LightShader->sendUniformMatrix4fv("WorldToCameraMat", WTC);

	}
	if (ImGui::CollapsingHeader("Gbuffer")) {
		ImGui::Checkbox("Should depth copy", &shouldDepthCopy);
		ImGui::Text("Gbuffer position");
		ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(m_textures[0])), {400,300}, ImVec2{0,1}, ImVec2{1,0});
		ImGui::Text("Gbuffer normal");
		ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(m_textures[1])), { 400,300 }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
		ImGui::Text("Gbuffer diffuse");
		ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(m_textures[2])), { 400,300 }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
		ImGui::Text("Gbuffer specular");
		ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(m_textures[3])), { 400,300 }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
		ImGui::Text("Gbuffer ambient");
		ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(m_textures[4])), { 400,300 }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
		ImGui::Text("Gbuffer emissive");
		ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(m_textures[5])), { 400,300 }, ImVec2{ 0,1 }, ImVec2{ 1,0 });
		ImGui::Text("Gbuffer depth");
		ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(m_depthTexture)), { 400,300}, ImVec2{ 0,1 }, ImVec2{ 1,0 });
	}
	if (ImGui::CollapsingHeader("Light control")) {
		ImGui::SliderFloat("Orbit speed", &orbitSpeed, 0.f, 2.f);
		// toggle rotation on off
		ImGui::Checkbox("Rotation on off", &isRotation);

		ImGui::SliderInt("Orbit number", &SpCurrNum, 1, SpMax);
		shaderManager->getShader("Lighting stage Shader")->sendUniform1iv("numCurLights", SpCurrNum);
		if (nowScenario == 0) {
			static glm::vec3 ambient{ 0.1f };
			static glm::vec3 diffuse{ 1.f };
			static glm::vec3 specular{ 0.5f };
			static float innerCut{ 0.6f };
			static float outerCut{ 0.5f };
			static float fallOff{ 1.f };
			static int type{ 1 };
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
				if (ImGui::TreeNode(std::string("light " + std::to_string(i) + " control").c_str())) {
					ImGui::SliderFloat3(std::string("light " + std::to_string(i) + " ambient").c_str(), &dynamic_cast<Light*>(ObjSpheres[i].get())->refLightData().ambient.r, 0.f, 1.f);
					ImGui::SliderFloat3(std::string("light " + std::to_string(i) + " diffuse").c_str(), &dynamic_cast<Light*>(ObjSpheres[i].get())->refLightData().diffuse.r, 0.f, 1.f);
					ImGui::SliderFloat3(std::string("light " + std::to_string(i) + " specular").c_str(), &dynamic_cast<Light*>(ObjSpheres[i].get())->refLightData().specular.r, 0.f, 1.f);
					//ImGui::EndMenu();
				}
			}
		}
		else if (nowScenario == 2) {
			for (int i = 0; i < SpCurrNum; ++i) {
				if (ImGui::TreeNode(std::string("light " + std::to_string(i) + " control").c_str())) {
					ImGui::SliderFloat3(std::string("light " + std::to_string(i) + " ambient").c_str(), &dynamic_cast<Light*>(ObjSpheres[i].get())->refLightData().ambient.r, 0.f, 1.f);
					ImGui::SliderFloat3(std::string("light " + std::to_string(i) + " diffuse").c_str(), &dynamic_cast<Light*>(ObjSpheres[i].get())->refLightData().diffuse.r, 0.f, 1.f);
					ImGui::SliderFloat3(std::string("light " + std::to_string(i) + " specular").c_str(), &dynamic_cast<Light*>(ObjSpheres[i].get())->refLightData().specular.r, 0.f, 1.f);
					ImGui::SliderFloat(std::string("light " + std::to_string(i) + " innerCut").c_str(), &dynamic_cast<Light*>(ObjSpheres[i].get())->refLightData().innerCut, 0.f, 1.f);
					ImGui::SliderFloat(std::string("light " + std::to_string(i) + " outerCut").c_str(), &dynamic_cast<Light*>(ObjSpheres[i].get())->refLightData().outerCut, 0.f, dynamic_cast<Light*>(ObjSpheres[i].get())->refLightData().innerCut);
					ImGui::SliderFloat(std::string("light " + std::to_string(i) + " fallOff").c_str(), &dynamic_cast<Light*>(ObjSpheres[i].get())->refLightData().fallOff, 1.f, 10.f);
					ImGui::SliderInt(std::string("light " + std::to_string(i) + " type").c_str(), &dynamic_cast<Light*>(ObjSpheres[i].get())->refLightData().type, 0, 2);
					//ImGui::EndMenu();
				}
			}
		}
	}
	if (ImGui::BeginMainMenuBar())
	{
		//camera control
		
		float moveSpeed{ 100.f * static_cast<float>(dt) };
		if (ImGui::IsKeyDown(ImGuiKey_::ImGuiKey_LeftShift) == true) {
			camera->setPosition(camera->getCameraPos() - glm::vec3{ 0,1,0 } *moveSpeed);
		}
		if (ImGui::IsKeyDown(ImGuiKey_::ImGuiKey_Space) == true) {
			camera->setPosition(camera->getCameraPos() + glm::vec3{ 0,1,0 } *moveSpeed);
		}
		if (ImGui::IsKeyDown(ImGuiKey_::ImGuiKey_W) == true) {
			camera->setPosition(camera->getCameraPos() + camera->getViewDirec() * moveSpeed);
		}
		if (ImGui::IsKeyDown(ImGuiKey_::ImGuiKey_S) == true) {
			camera->setPosition(camera->getCameraPos() - camera->getViewDirec() * moveSpeed);
		}
		if (ImGui::IsKeyDown(ImGuiKey_::ImGuiKey_D) == true) {
			camera->setPosition(camera->getCameraPos() + camera->getRightDirec() * moveSpeed);
		}if (ImGui::IsKeyDown(ImGuiKey_::ImGuiKey_A) == true) {
			camera->setPosition(camera->getCameraPos() - camera->getRightDirec() * moveSpeed);
		}
		camera->Update(0.01);
		shaderManager->getShader("Geometry stage Shader")->sendUniform3fv("cameraPos", camera->getCameraPos());
		shaderManager->getShader("Lighting stage Shader")->sendUniform3fv("cameraPos", camera->getCameraPos());
		shaderManager->getShader("Geometry stage Shader")->sendUniformMatrix4fv("WorldToCameraMat", WTC);
		NormalShdrProgram->sendUniformMatrix4fv("WorldToCameraMat", WTC);
		LightShader->sendUniformMatrix4fv("WorldToCameraMat", WTC);
		ImGui::EndMainMenuBar();
	}

	for (int i = 0; i < SpCurrNum; ++i) {
		float now = (float)i / SpCurrNum;
		float alpha = now * 3.14f * 2.f + (float)(time)*orbitSpeed;
		glm::vec3 pos = glm::vec3{ ObjCircleLine->scale.x / 2.f * sin(alpha),0.f,ObjCircleLine->scale.x / 2.f * cos(alpha) };
		ObjSpheres[i]->setPos(pos);
		ObjSpheres[i]->update(dt);
	}
	Obj[nowObj]->setRotate({ 0.f,ObjRotate,0.f });
	Obj[nowObj]->update(dt);
	ObjCircleLine->update(dt);
	screenRect->update(dt);
}

void TestScene2::Draw()
{
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	
	WTC = camera->getWorldToCameraToNDC();
	
	glViewport(0, 0, 1200, 800);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.3f, 0.3f, 0.3f, 1);


	//textureManager->bindTextures("Diffuse Shader", shaderManager);
	//diffuseShader->sendUniform1iv("numCurLights", SpCurrNum);

	{//geometry
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_gBufferFBO);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		textureManager->bindTextures("Geometry stage Shader", shaderManager);
		shaderManager->getShader("Geometry stage Shader")->sendUniform1iv("texMappingType", OBJtextureMappingNum);
		shaderManager->getShader("Geometry stage Shader")->sendUniform3fv("MatAmbient", ObjAmbient);
		shaderManager->getShader("Geometry stage Shader")->sendUniform3fv("MatEmissive", ObjEmissive);
		shaderManager->getShader("Geometry stage Shader")->sendUniform1iv("isGPUMapping", IsGPUtextureMapping);
		shaderManager->getShader("Geometry stage Shader")->sendUniform1iv("isPositionEntity", IsPositionEntityMapping);
		shaderManager->getShader("Geometry stage Shader")->sendUniform3fv("boundMax", Obj[nowObj]->boundBoxMax);
		shaderManager->getShader("Geometry stage Shader")->sendUniform3fv("boundMin", Obj[nowObj]->boundBoxMin);
		Obj[nowObj]->draw();

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}

	{//lighting
		shaderManager->getShader("Lighting stage Shader")->useProgram();
		
		glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_textures[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_textures[1]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, m_textures[2]);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, m_textures[3]);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, m_textures[4]);
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, m_textures[5]);
		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D, m_depthTexture);


		for (int i = 0; i < SpCurrNum; ++i) {
			std::string lightLniformName{ "lightSources[" + std::to_string(i) + ("]") };
			dynamic_cast<Light*>(ObjSpheres[i].get())->setLightDirection(Obj[nowObj]->pos - ObjSpheres[i]->pos);
			dynamic_cast<Light*>(ObjSpheres[i].get())->sendLightDataUniform(shaderManager->getShader("Lighting stage Shader"), lightLniformName);
		}
		shaderManager->getShader("Lighting stage Shader")->unuseProgram();

		screenRect->draw();
	}
	if (shouldDepthCopy) {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_gBufferFBO);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(0, 0, (int)engine->GetWindowSize().x, (int)engine->GetWindowSize().y, 0, 0, (int)engine->GetWindowSize().x, (int)engine->GetWindowSize().y, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	{//forward shading
		NormalShdrProgram->sendUniformMatrix4fv("WorldToCameraMat", WTC);
		LightShader->sendUniformMatrix4fv("WorldToCameraMat", WTC);

		if (normalDrawState != 0) {
			Obj[nowObj]->drawNormal(normalDrawState);
		}

		ObjCircleLine->draw();
		for (int i = 0; i < SpCurrNum; ++i) {
			std::string lightLniformName{ "lightSources[" + std::to_string(i) + ("]") };
			dynamic_cast<Light*>(ObjSpheres[i].get())->setLightDirection(Obj[nowObj]->pos - ObjSpheres[i]->pos);
			dynamic_cast<Light*>(ObjSpheres[i].get())->sendLightDataUniform(shaderManager->getShader("Lighting stage Shader"), lightLniformName);
			ObjSpheres[i]->draw();
			if (normalDrawState != 0) {
				ObjSpheres[i]->drawNormal(normalDrawState);
			}
		}	
		
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void TestScene2::Unload()
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
	screenRect->unload();
}
