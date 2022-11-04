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
    int type{};
    //directional light == 0
    glm::vec3 direction{};
    //point light == 1
    glm::vec3 position{};
    float constant{};
    float linear{};
    float quadratic{};
    //spot light == 2
    float angle{};

    glm::vec3 ambient{};
    glm::vec3 diffuse{};
    glm::vec3 specular{};
    glm::vec3 emissive{};
};

class Light :public Entity {

    LightData lightData{};


public:
    //void load()override;
    //void unload()override;
    //void update(double dt)override;
    //void draw()override;

    LightData getLightData() { return lightData; };
    LightData& refLightData() { return lightData; };
};