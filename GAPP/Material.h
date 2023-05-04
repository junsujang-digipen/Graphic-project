/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Material.h
Purpose: For storing Entity datas
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 3 - Spatial Partitioning
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 11/04/2022
End Header --------------------------------------------------------*/

#pragma once
#include "glm/glm.hpp"

struct MaterialData {
    glm::vec3 ambient{0.5f};
    glm::vec3 diffuse{0.5f};
    glm::vec3 specular{0.5f};
    float shininess{10.f};
    glm::vec3 emissive{0.f};
};


class Material {

    MaterialData materialData{};

public:

};