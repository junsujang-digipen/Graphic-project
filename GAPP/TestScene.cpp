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
const char* Vshdr{ R"(
#version 400 core
layout(location = 0) in vec3 modelPosition;

layout(location = 1) in vec3 a_normal;
out vec3 v_normal;
out vec3 v_pos;

uniform mat4 modelToWorldMat;
uniform mat4 WorldToCameraMat;
uniform mat3 u_normalMatrix;

void main()
{
	v_normal = u_normalMatrix * a_normal;
	v_pos = vec3(modelToWorldMat*vec4(modelPosition,1));
	gl_Position.xyzw = WorldToCameraMat*modelToWorldMat*vec4(modelPosition,1);
	//gl_Position.w = 1.0;
}
)" };
const char* Fshdr{ R"(
#version 400 core

uniform vec3 u_light = vec3(0., 100., 100.);
uniform vec3 u_light_col = vec3(1., 1., 1.);
uniform vec3 u_ambient = vec3(0.5);
uniform vec3 u_materialCol = vec3(1.0, 1.0, 1.0);
uniform vec3 cameraPos = vec3(0.,100.,100.);

in vec3 v_normal;
in vec3 v_pos;

out vec4 col;

void main()
{
	float specularStrength = 0.5;
	float shininess = 10.;
	float ambientStrength = 0.1;

	vec3  n  = normalize(v_normal);
    vec3  l  = normalize(u_light - v_pos);
    float nl = max(dot(n, l), 0.);

	vec3 viewDir = normalize(cameraPos - v_pos);
	vec3 reflectDir = reflect(-l, n);  
	float sf = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	
	vec3 diffuse = nl*u_light_col;
	vec3 specular = specularStrength * sf * u_light_col;	
    vec3 ambient = ambientStrength * u_ambient * u_light_col;

	vec3 color = (ambient + diffuse + specular) * u_materialCol;
	col = vec4(color,1.0);
}
)" };

const char* VshdrNormal{ R"(
#version 400 core

layout(location = 2) in vec3 modelPosition;

uniform mat4 modelToWorldMat;
uniform mat4 WorldToCameraMat;

void main()
{
	gl_Position.xyzw = WorldToCameraMat*modelToWorldMat*vec4(modelPosition,1);
}
)" };
const char* FshdrNormal{ R"(
#version 400 core

out vec4 col;

void main()
{
	col = vec4(1.0);
}
)" };

GLuint shdrProgram{};
GLuint shdrProgramNormal{};
Camera camera{};
glm::mat4 WTC{};

void TestScene::Load()
{
	camera.init();
	camera.setPosition({ 0,130,150 });
	camera.setRotate({ glm::radians(-45.f),glm::radians(/*36.f*/0.f),0.f });
	camera.Update(0.01);

	diffuseShader = std::make_shared<Shader>();
	NormalShdrProgram = std::make_shared<Shader>();
	//Shader
	{
		GLuint vshdr = diffuseShader->compileShader(GL_VERTEX_SHADER, Vshdr);
		GLuint fshdr = diffuseShader->compileShader(GL_FRAGMENT_SHADER, Fshdr);
		diffuseShader->attachShader(vshdr);
		diffuseShader->attachShader(fshdr);
		diffuseShader->linkProgram();
		shdrProgram = diffuseShader->getHandle();
	}
	//shader normal
	{
		GLuint vshdr = NormalShdrProgram->compileShader(GL_VERTEX_SHADER, VshdrNormal);
		GLuint fshdr = NormalShdrProgram->compileShader(GL_FRAGMENT_SHADER, FshdrNormal);
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
		for (int i = 0; i < 8; ++i) {
			ObjSpheres.push_back(std::make_shared<Entity>());
			OBJLoader Temp{};
			MakeSphereData(20, 20, Temp.VertexDatas, Temp.VertexNormalDatas, Temp.FaceNormalDatas, Temp.idxDatas);
			float now = (float)i / 8;
			float alpha = now * 3.14f * 2.f;
			glm::vec3 pos = glm::vec3{ ObjCircleLine->scale.x/2.f * sin(alpha),0.f,ObjCircleLine->scale.x/2.f * cos(alpha) };
			ObjSpheres[i]->setPos(pos);
			ObjSpheres[i]->setScale(glm::vec3(10.f));
			ObjSpheres[i]->GetDataForOBJLoader(Temp);
			ObjSpheres[i]->load();

			ObjSpheres[i]->objShader = diffuseShader;
			ObjSpheres[i]->normalVectorShader = NormalShdrProgram;
		}
	}
}

void TestScene::Update(double dt)
{
	time += dt;
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::SliderInt("Normal", &normalDrawState, 0, 3);
	ImGui::SliderInt("OBJ Number", &nowObj, 0, 4);
	ImGui::SliderFloat("OBJ rotate", &ObjRotate, 0.f, 6.28f);
	ImGui::SliderFloat("Orbit speed", &orbitSpeed, 0.f, 2.f);

	for (int i = 0; i < ObjSpheres.size(); ++i) {
		float now = (float)i / ObjSpheres.size();
		float alpha = now * 3.14f * 2.f + (float)(time)* orbitSpeed;
		glm::vec3 pos = glm::vec3{ ObjCircleLine->scale.x/2.f * sin(alpha),0.f,ObjCircleLine->scale.x/2.f * cos(alpha) };
		ObjSpheres[i]->setPos(pos);
		ObjSpheres[i]->update(dt);
	}
	Obj[nowObj]->setRotate({0.f,ObjRotate,0.f});
	Obj[nowObj]->update(dt);
	ObjCircleLine->update(dt);
}

void TestScene::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.3f, 0.3f, 0.3f, 1);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);

	for (auto& ObjSphere : ObjSpheres) {
		ObjSphere->draw();
		if (normalDrawState != 0) {
			ObjSphere->drawNormal(normalDrawState);
		}
	}
	Obj[nowObj]->draw();
	ObjCircleLine->draw();

	if (normalDrawState != 0) {
		Obj[nowObj]->drawNormal(normalDrawState);
	}

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
}
