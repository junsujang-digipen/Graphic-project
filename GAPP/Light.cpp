/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Light.cpp
Purpose: For storing Entity datas
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 3 - Spatial Partitioning
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 11/03/2022
End Header --------------------------------------------------------*/

#include "pch.h"
#include "Light.h"
#include "Shader.h"

//void Light::load()
//{
//}
//
//void Light::unload()
//{
//}
//
void Light::update(double dt)
{
	Entity::update(dt);
	//if (shouldSendUniformData == true) {
	//	shouldSendUniformData = false;
	//	sendLightDataUniform();
	//}
	lightData.position = pos;
}
//
void Light::draw()
{
	objShader->sendUniform3fv("OBJColor", lightData.diffuse);
	Entity::draw();

}

void Light::sendLightDataUniform(std::shared_ptr<Shader> shader, const std::string uniformName)
{
	shader->sendUniform1iv((uniformName + ".type").c_str(),lightData.type);
	shader->sendUniform3fv((uniformName + ".direction").c_str(),lightData.direction);
	/*sendLightPositionDataUniform(objShader,uniformName);*/
	shader->sendUniform3fv((uniformName + ".position").c_str(), lightData.position);
	shader->sendUniform1fv((uniformName + ".constant").c_str(), lightData.constant);
	shader->sendUniform1fv((uniformName + ".linear").c_str(), lightData.linear);
	shader->sendUniform1fv((uniformName + ".quadratic").c_str(), lightData.quadratic);
	shader->sendUniform1fv((uniformName + ".innerCut").c_str(), lightData.innerCut);
	shader->sendUniform1fv((uniformName + ".outerCut").c_str(), lightData.outerCut);
	shader->sendUniform1fv((uniformName + ".fallOff").c_str(), lightData.fallOff);
	shader->sendUniform3fv((uniformName + ".ambient").c_str(), lightData.ambient);
	shader->sendUniform3fv((uniformName + ".diffuse").c_str(), lightData.diffuse);
	shader->sendUniform3fv((uniformName + ".specular").c_str(), lightData.specular);

}

void Light::sendLightPositionDataUniform(std::shared_ptr<Shader> shader, const std::string uniformName)
{
	shader->sendUniform3fv((uniformName + ".position").c_str(), lightData.position);

}

void Light::resetLightData()
{
	lightData = LightData{};
}

void Light::setLightDirection(glm::vec3 dir)
{
	lightData.direction = dir;
}

LightData Light::getLightData() { return lightData; }
LightData& Light::refLightData() {
	
	return lightData;
}
