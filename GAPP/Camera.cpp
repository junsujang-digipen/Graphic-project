/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Camera.cpp
Purpose: Camera matrix functions
Language: c++
Platform: x64
Project: junsu.jang, CS300, Assignment 2 - Implementing Phong Illumination Model
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 09/28/2022
End Header --------------------------------------------------------*/
#include "pch.h"
#include "Camera.h"

void Camera::init()
{
	/*int w{};
	int h{};
	glfwGetWindowSize(GraphicEngine::GetWindowPtr(), &w, &h);
	ar = static_cast<float>(w) / h;
	width = tan(fov / 2) * 2 * distance;
	height = width / ar;*/
}

void Camera::Update(double dt) {
	arUpdate();

	cameraMoveFunc(dt);
	
	matrixUpdateFunc();
}

void Camera::resetAngle()
{
	upDirection = { 0,1,0 };
	rightDirection = { 1,0,0 };
	backDirection = { 0,0,1 };
}

void Camera::setRotate(glm::vec3 angle)
{
	glm::mat3 XRotMat{ rotationMat(glm::vec3{1,0,0},angle.x) };
	glm::mat3 YRotMat{ rotationMat(glm::vec3{0,1,0},angle.y) };
	glm::mat3 ZRotMat{ rotationMat(glm::vec3{0,0,1},angle.z) };
	backDirection = glm::normalize((XRotMat * YRotMat * ZRotMat) * glm::vec3{ 0,0,1 });
	rightDirection = glm::normalize(glm::cross(glm::vec3{ 0,1,0 }, backDirection));
	upDirection = glm::normalize(glm::cross(backDirection, rightDirection));
	matrixUpdate = true;
}

void Camera::arUpdate() {
	//int w{};
	//int h{};
	//glfwGetWindowSize(GraphicEngine::GetWindowPtr(), &w, &h);
	//glm::vec2 winSize = { w,h };
	//ar = width / height;
	//if (ar != winSize.x / winSize.y) {
	//	ar = winSize.x / winSize.y;
	//	//width = tan(fov / 2) * 2 * distance;
	//	//height = width / ar;

	//	const float prevHeight = width / ar;
	//	width = ar * height;
	//	matrixUpdate = true;
	//	distance = (near + far) / 2.f;
	//}
}

void Camera::matrixUpdateFunc()
{
	if (matrixUpdate == true) {
		const float nearMinusFar = near - far;
		cameraToWorld = glm::mat4{ glm::vec4{rightDirection,0.f},glm::vec4{upDirection,0.f},glm::vec4{backDirection,0.f},glm::vec4{position,1.f} };
		worldToCamera = glm::inverse(cameraToWorld);
		cameraToNDC = glm::mat4{
			glm::vec4{2.f * distance / width,0.f,0.f,0.f},
			glm::vec4{0.f,2.f * distance / height,0.f,0.f},
			glm::vec4{0.f,0.f,(near + far) / (nearMinusFar),-1.f},
			glm::vec4{0.f,0.f,2.f * near * far / (nearMinusFar),0.f} };
		//glm::frustum(leftPlane, rightPlane, bottomPlane, topPlane, nearPlane, farPlane);
		worldToCameraToNDC = cameraToNDC * worldToCamera;

		matrixUpdate = false;
	}
}
void Camera::cameraMoveFunc(double /*dt*/)
{

}

glm::mat3 rotationMat(glm::vec3 axis, float angle) {
	
	const float cosA{ cos(angle) };
	const float sinA{ sin(angle) };
	const float rCosA{ 1 - cosA };
	const float x{ axis.x };
	const float y{ axis.y };
	const float z{ axis.z };
	return glm::mat3{
		glm::vec3{cosA + x * x * rCosA, y * x * rCosA + z * sinA,z * x * rCosA - y * sinA},
		glm::vec3{x * y * rCosA - z * sinA,cosA + y * y * rCosA,z * y * rCosA + x * sinA},
		glm::vec3{x * z * rCosA + y * sinA,y * z * rCosA - x * sinA,cosA + z * z * rCosA}
	};
}

const glm::vec3 Camera::getCameraPos() const
{
	return position;
}