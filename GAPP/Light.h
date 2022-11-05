/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Light.h
Purpose: For storing Entity datas
Language: c++
Platform: x64
Project: junsu.jang, CS300, Assignment 2 - Implementing Phong Illumination Model
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 11/03/2022
End Header --------------------------------------------------------*/
#pragma once
#include "Entity.h"
struct LightData {
    int type{1};
    //directional light == 0
    glm::vec3 direction{0.,1.f,0.};
    //point light == 1
    glm::vec3 position{0.f,0.f,0.f};
    float constant{1.f,};
    float linear{0.009f};
    float quadratic{0.0005f};
    //spot light == 2
    float innerCut{0.6f};
    float outerCut{ 0.5f};

    glm::vec3 ambient{ 0.5f,0.5f,0.5f };
    glm::vec3 diffuse{ 1.f,1.f,1.f };
    glm::vec3 specular{ 0.5f,0.5f,0.5f };
};

class Light :public Entity {

    LightData lightData{};
    //bool shouldSendUniformData{true};

    //std::shared_ptr<Shader> lightingShader{};

public:
    //void load()override;
    //void unload()override;
    void update(double dt)override;
    void draw()override;
    void sendLightDataUniform(std::shared_ptr<Shader> shader, const std::string uniformName);
    void sendLightPositionDataUniform(std::shared_ptr<Shader> shader, const std::string uniformName);
    
    void resetLightData();
    void setLightDirection(glm::vec3 dir);
    LightData getLightData();
    LightData& refLightData();
};