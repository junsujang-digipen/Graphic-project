/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Camera.h
Purpose: Camera matrix functions
Language: c++
Platform: x64
Project: junsu.jang, CS300, Assignment 1 - Render an OBJ file
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 09/28/2022
End Header --------------------------------------------------------*/
#pragma once
#include "glm/glm.hpp"

class Camera {
	glm::vec3 position{0,0,0};
	glm::vec3 upDirection{ 0,1,0 };
	glm::vec3 rightDirection{ 1,0,0 };
	glm::vec3 backDirection{ 0,0,1 };

	glm::mat4 worldToCamera{};
	glm::mat4 cameraToWorld{};
	glm::mat4 cameraToNDC{};
	glm::mat4 worldToCameraToNDC{};

	bool matrixUpdate{ true };
	float fov{ 0.33f * 3.14f };
	float height{800.f};
	float width{ 1200.f };
	float ar{ width / height };
	float near{ 0.1f };
	float far{ 1000.f };
	float distance{ (near + far) * 0.5f };

	bool isMove{ true };
	glm::dvec3 prevMousePos{};
	glm::dvec3 curMousePos{};

	void arUpdate();
	void matrixUpdateFunc();
	void cameraMoveFunc(double dt);

	float speed{ 5000.f };
public:
	Camera() = default;
	void init();
	const glm::vec3 getCameraPos()const;
	void Update(double dt);
	//rotate function?
	void setPosition(glm::vec3 pos) { position = pos; matrixUpdate = true; }
	const glm::mat4 getWorldToCameraToNDC()const { return worldToCameraToNDC; }
	const glm::mat4 getWorldToCamera()const { return worldToCamera; }
	const glm::mat4 getCameraToNDC()const { return cameraToNDC; }
	const glm::mat4 getCameraToWorld()const { return cameraToWorld; }
	void setCameraHeight(float hei) { height = hei; }
	void updateCameraHeight(float AH) { height += AH; }
	void resetAngle();
	void setisMove(bool TF) { isMove = TF; }
	const float getCameraWidth()const { return width; }
	const float getCameraHeight()const { return height; }
	float& getNear() { return near; }
	float& getFar() { return far; }
	float& getFov() { return fov; }
	float& getAr() { return ar; }
	float getheight() { return height; }
	float getFarWidth() { return width / distance * far; }
	glm::vec3 getViewDirec() { return backDirection * -1.f; }

	void setDistance(float dist) { distance = dist; matrixUpdate = true; }
	void setNear(float Near) { near = Near; matrixUpdate = true; }
	void setFar(float Far) { far = Far; matrixUpdate = true; }
	void setFov(float Fov) { fov = Fov; matrixUpdate = true; }
	void setAr(float Ar) { ar = Ar; matrixUpdate = true; }
	void setIsUpdate(bool TF) { matrixUpdate = TF; };
	void setRotate(glm::vec3 angle);
	void setSpeed(float sp) { speed = sp; }
};
glm::mat3 rotationMat(glm::vec3 axis, float angle);