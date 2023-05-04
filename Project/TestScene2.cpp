/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: TestScene2.cpp
Purpose: Scene for testing objs and loader and shaders
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 3 - Spatial Partitioning
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

#include <MeshManager.h>

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
#include <AssimpLoader.h>
#include <glm/gtx/string_cast.hpp>
#include <fstream>

#include <SpatialManager.h>
#include <BoundingVolumeManager.h>
#include <ObjManager.h>
#include <Input.h>

TestScene2::TestScene2() : Scene(), SpCurrNum(1)
{
}
TestScene2::~TestScene2() {
	delete camera;
}

void TestScene2::Load()
{
	// Setup ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	// Setup Platform/Renderer bindings
	ImGui::StyleColorsDark();
	const char* glsl_version = "#version 460";
	ImGui_ImplGlfw_InitForOpenGL(const_cast<GLFWwindow *>(engine->GetWindow()), true);
	ImGui_ImplOpenGL3_Init(glsl_version);


	camera = {new Camera};
	camera->init();
	//camera->setPosition({ 0,130,150 });
	camera->setPosition({ 0,40000,140000 });
	//camera->setRotate({ glm::radians(-45.f),glm::radians(/*36.f*/0.f),0.f });
	camera->Update(0.01);
	WTC = { camera->getWorldToCameraToNDC() };
	////WTC = { glm::perspective <float > (glm::radians(45.f),800 / 600, 0.001f,1000.f) };

	const std::string Vshdr{ ShaderHelper::getShaderSourceFromFile("../Shaders/C_DiffuseShader.vert") };
	const std::string Fshdr{ ShaderHelper::getShaderSourceFromFile("../Shaders/C_DiffuseShader.frag") };
	
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
	
	//normal shader 
	const std::string VshdrNormal{ ShaderHelper::getShaderSourceFromFile("../Shaders/C_BasicShader.vert") };
	const std::string FshdrNormal{ ShaderHelper::getShaderSourceFromFile("../Shaders/C_BasicShader.frag") };
	shaderManager->makeShader("Normal Shader");
	{
		GLuint vshdr = shaderManager->getShader("Normal Shader")->compileShader(ShaderType::VERTEX_SHADER, { VshdrNormal });
		GLuint fshdr = shaderManager->getShader("Normal Shader")->compileShader(ShaderType::FRAGMENT_SHADER, { FshdrNormal });
		shaderManager->getShader("Normal Shader")->attachShader(vshdr);
		shaderManager->getShader("Normal Shader")->attachShader(fshdr);
		shaderManager->getShader("Normal Shader")->linkProgram();
	}

	{//scene set up 
		for (int n = 4; n <7; ++n) {
			std::ifstream file{ "../Objects/Section" + std::to_string(n) + ".txt", std::ios::in };
			if (!file) {
				std::cout << "Unable to open file: " << "../Objects/Section" + std::to_string(n) + ".txt" << "\n";
				exit(EXIT_FAILURE);
			}
			else {
				std::cout << "File opened." << std::endl;
				std::string text{};
				while (file >> text) {
					std::shared_ptr<Entity> temp = objManager->GenObj();
					objManager->SetSceneComponent(temp->getID());
					temp->setScale(glm::vec3(1.f));
					AssimpLoader assimp{};
					assimp.LoadModel("../Objects/" + text);
					MeshData tempMD{ assimp.MakeMeshData(temp->getScale()) };
					//temp->MeshID = meshManager->push_MeshData(tempMD);
					objManager->SetMeshID(temp->getID(), meshManager->push_MeshData(tempMD));
					entityContainer.emplace<BoundingBox>(temp->getID(), meshManager->make_BoundingBox(temp->MeshID));
					entityContainer.emplace<BoundingSphere>(temp->getID(), meshManager->make_BoundingSphere(temp->MeshID));
					temp->objShader = shaderManager->getShader("Geometry stage Shader");
					temp->normalVectorShader = shaderManager->getShader("Normal Shader");
				}
			}
		}

		bvManager->calcHierarchy();
		spatialManager->BuildOctree();
		spatialManager->BuildBSPtree();
	}

	//Obj load
	{
		const char* filepaths[5]{
			"./4Sphere.obj" ,
			"./bunny_high_poly.obj" ,
			"../Objects/cube2.obj" ,
			"./sphere.obj" ,
			"./sphere_modified.obj" };
		for (int i = 0; i < 5; ++i) {
			Obj.push_back(std::make_shared<BasicObject>(this));

			Obj[i]->setScale(glm::vec3(10000.f));
			OBJLoader objLoader{};
			objLoader.FileLoad(filepaths[i]);
			MeshData tempMD{ objLoader.makeMeshData(Obj[i]->getScale()) };

			Obj[i]->MeshID = meshManager->push_MeshData(tempMD);
			Obj[i]->objShader = shaderManager->getShader("Geometry stage Shader");
			Obj[i]->normalVectorShader = shaderManager->getShader("Normal Shader");

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
		screenRect = std::make_shared<FSQ>(this, ID{});

		screenRect->objShader = shaderManager->getShader("Lighting stage Shader");
	}

	//Orbit
	{
		ObjCircleLine = std::make_shared<BasicObject>(this);
		OBJLoader Temp{};
		MakeCircleLineData(100, Temp.VertexDatas, Temp.VertexNormalDatas, Temp.FaceNormalDatas, Temp.idxDatas);
		Temp.primitive_type = GL_LINES;
		ObjCircleLine->setPos(glm::vec3(0.f, 0.f, 0.f));
		ObjCircleLine->setScale(glm::vec3(100000.f));

		MeshData tempMD{ Temp.makeMeshData(glm::vec3{100.f}) };
		ObjCircleLine->MeshID = meshManager->push_MeshData(tempMD);

		ObjCircleLine->objShader = LightShader;
		ObjCircleLine->normalVectorShader = shaderManager->getShader("Normal Shader");
	}
	//sphere
	{
		const int SpMax{ 16 };
		//OBJLoader Temp{};
		//MakeSphereData(20, 20, Temp.VertexDatas, Temp.VertexNormalDatas, Temp.FaceNormalDatas, Temp.idxDatas);
		//MeshData tempMD{ Temp.makeMeshData(glm::vec3(10.f)) };
		//int tempMeshID{ meshManager->push_MeshData(tempMD) };
		int tempMeshID{ Cast<int>(BasicMeshType::Sphere)};
		for (int i = 0; i < SpMax; ++i) {
			ObjSpheres.push_back(std::make_shared<Light>(this));
			float now = (float)i / SpMax;
			float alpha = now * 3.14f * 2.f;
			glm::vec3 pos = glm::vec3{ ObjCircleLine->scale.x / 2.f * sin(alpha),0.f,ObjCircleLine->scale.x / 2.f * cos(alpha) };
			ObjSpheres[i]->setPos(pos);
			ObjSpheres[i]->setScale(glm::vec3(5000.f));
			ObjSpheres[i]->MeshID = tempMeshID;
			std::string lightLniformName{ "lightSources[" + std::to_string(i) + ("]") };
			dynamic_cast<Light*>(ObjSpheres[i].get())->sendLightDataUniform(shaderManager->getShader("Lighting stage Shader"), lightLniformName);
			ObjSpheres[i]->objShader = LightShader;
			ObjSpheres[i]->normalVectorShader = shaderManager->getShader("Normal Shader");

		}
		dynamic_cast<Light*>(ObjSpheres[0].get())->refLightData().type = 0;
	}
}


void TestScene2::Update(double dt)
{
	Scene::Update(dt);
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
				ImGui::SliderFloat(std::string("Fog near").c_str(), &camera->getNear(), 0.f, camera->getFar());
				shaderManager->getShader("Lighting stage Shader")->sendUniform1fv("cameraNear", camera->getNear());
				ImGui::SliderFloat(std::string("Fog far").c_str(), &camera->getFar(), camera->getNear(), 500000.f);
				shaderManager->getShader("Lighting stage Shader")->sendUniform1fv("cameraFar", camera->getFar());
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
				meshManager->getMeshDataRef(o->MeshID).IsPositionEntity = IsPositionEntityMapping;
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
					meshManager->getMeshDataRef(Obj[nowObj]->MeshID).calcSphereTexCoord();
				}
				if (two == true) {
					OBJtextureMappingNum = 1;
					meshManager->getMeshDataRef(Obj[nowObj]->MeshID).calcCylindricalTexCoord();
				}
				if (three == true) {
					OBJtextureMappingNum = 2;
					meshManager->getMeshDataRef(Obj[nowObj]->MeshID).calcPlanarTexCoord();
				}
				if (four == true) {
					OBJtextureMappingNum = 3;
					meshManager->getMeshDataRef(Obj[nowObj]->MeshID).calcCubeMapTexCoord();
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
		int objSize{ (int)Obj.size() -1};
		ImGui::SliderInt("OBJ Number", &nowObj, 0, objSize);
		ImGui::SliderFloat("OBJ rotate", &ObjRotate, 0.f, 6.28f);
	}
	 
	if (ImGui::CollapsingHeader("Camera control")) {
		static glm::vec3 cameraRotation{ camera->getAngle() };
		ImGui::SliderFloat("Camera Yaw rotate", &cameraRotation.y, -3.14f, 3.14f);
		camera->updateRotate(cameraRotation - camera->getAngle());
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
		//ImGui::Text("Gbuffer depth");
		//ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(m_depthTexture)), { 400,300}, ImVec2{ 0,1 }, ImVec2{ 1,0 });
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
	if (ImGui::CollapsingHeader("Bounding Volume control")) {
		if (ImGui::Button("Calculate Ritter's bounding sphere")) {
			objManager->NewBoundingSphere(BoundingVolumeType::RITTER);
		}
		if (ImGui::Button("Calculate Larsson's bounding sphere")) {
			objManager->NewBoundingSphere(BoundingVolumeType::LARSSON);
		}
		if (ImGui::Button("Calculate PCA-based bounding sphere")) {
			objManager->NewBoundingSphere(BoundingVolumeType::PCA);
		}
		ImGui::Checkbox("Bounding Sphere draw on off", &bvManager->ShouldDrawBoundingSphere);
		ImGui::Checkbox("Bounding Box draw on off", &bvManager->ShouldDrawBoundingBox);
		ImGui::Checkbox("Bottom_Up Bounding Box Hierarchy draw on off", &bvManager->ShouldDraw_BU_BoundingBoxHierarchy);
		ImGui::Checkbox("Bottom_Up Bounding Sphere Hierarchy draw on off", &bvManager->ShouldDraw_BU_BoundingSphereHierarchy);
		ImGui::Checkbox("Top_Down Bounding Box Hierarchy draw on off", &bvManager->ShouldDraw_TD_BoundingBoxHierarchy);
		ImGui::Checkbox("Top_Down Bounding Sphere Hierarchy draw on off", &bvManager->ShouldDraw_TD_BoundingSphereHierarchy);
		ImGui::Checkbox("Draw all level", &bvManager->DrawAllLevel);
		ImGui::SliderInt("Draw level", &bvManager->DrawLevel, 0, 20);
	}//spatialManager
	if (ImGui::CollapsingHeader("Spatial manager control")) {
		ImGui::Checkbox("Octree draw on off", &spatialManager->ShouldDraw_Octree);
		ImGui::Checkbox("BSPtree draw on off", &spatialManager->ShouldDraw_BSPtree);
		ImGui::Checkbox("debug mesh draw on off", &spatialManager->ShouldDraw_DebugMesh);
		ImGui::Checkbox("Draw all level", &spatialManager->DrawAllLevel);
		ImGui::SliderInt("Draw level", &spatialManager->DrawLevel, 0, 8); 
	}
	if (ImGui::BeginMainMenuBar())
	{
		//camera control
		Keyboard kb{};
		float moveSpeed{ 1000.f * static_cast<float>(dt) };
		if (kb.IsKeyDown(GLFW_KEY_LEFT_SHIFT) == true) {
			camera->setPosition(camera->getCameraPos() - glm::vec3{ 0,1,0 } *moveSpeed);
		}
		
		if (kb.IsKeyDown(GLFW_KEY_SPACE) == true) {
			camera->setPosition(camera->getCameraPos() + glm::vec3{ 0,1,0 } *moveSpeed);
		}
		if (kb.IsKeyDown(GLFW_KEY_W) == true) {
			camera->setPosition(camera->getCameraPos() + camera->getViewDirec() * moveSpeed);
		}
		if (kb.IsKeyDown(GLFW_KEY_S) == true) {
			camera->setPosition(camera->getCameraPos() - camera->getViewDirec() * moveSpeed);
		}
		if (kb.IsKeyDown(GLFW_KEY_D) == true) {
			camera->setPosition(camera->getCameraPos() + camera->getRightDirec() * moveSpeed);
		}if (kb.IsKeyDown(GLFW_KEY_A) == true) {
			camera->setPosition(camera->getCameraPos() - camera->getRightDirec() * moveSpeed);
		}
		float CameraAngleSpeed{ static_cast<float>(dt)/20.f };
		if (kb.IsKeyDown(GLFW_KEY_Q) == true) {
			camera->updateRotate(-glm::vec3{0.f,CameraAngleSpeed,0.f});
		}
		if (kb.IsKeyDown(GLFW_KEY_E) == true) {
			camera->updateRotate(glm::vec3{ 0.f,CameraAngleSpeed,0.f });
		}
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
	//entityContainer.get<ObjectMatrixComponent>(Obj[nowObj]->getID()).objectMatrix;
	ObjCircleLine->update(dt);
	screenRect->update(dt);
	camera->Update(dt);
	WTC = camera->getWorldToCameraToNDC();
	shaderManager->getShader("Geometry stage Shader")->sendUniform3fv("cameraPos", camera->getCameraPos());
	shaderManager->getShader("Lighting stage Shader")->sendUniform3fv("cameraPos", camera->getCameraPos());
	shaderManager->getShader("Lighting stage Shader")->sendUniform1fv("cameraNear", camera->getNear());
	shaderManager->getShader("Lighting stage Shader")->sendUniform1fv("cameraFar", camera->getFar());
	shaderManager->getShader("Geometry stage Shader")->sendUniformMatrix4fv("WorldToCameraMat", WTC);
	shaderManager->getShader("Normal Shader")->sendUniformMatrix4fv("WorldToCameraMat", WTC);
	LightShader->sendUniformMatrix4fv("WorldToCameraMat", WTC);
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
		shaderManager->getShader("Geometry stage Shader")->sendUniform3fv("boundMax", meshManager->getMeshData(Obj[nowObj]->MeshID).boundBoxMax);
		shaderManager->getShader("Geometry stage Shader")->sendUniform3fv("boundMin", meshManager->getMeshData(Obj[nowObj]->MeshID).boundBoxMin);
		Obj[nowObj]->draw();
		if (spatialManager->ShouldDraw_DebugMesh == false) {
			objManager->draw();
		}
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
		shaderManager->getShader("Normal Shader")->sendUniformMatrix4fv("WorldToCameraMat", WTC);
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
		//Scene::Draw();
		bvManager->Draw();
		spatialManager->Draw();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void TestScene2::Unload()
{

	Obj.clear();

	ObjSpheres.clear();

	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
}
