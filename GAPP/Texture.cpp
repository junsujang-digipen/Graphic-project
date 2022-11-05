/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Texture.cpp
Purpose: For storing Entity datas
Language: c++
Platform: x64
Project: junsu.jang, CS300, Assignment 2 - Implementing Phong Illumination Model
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 11/04/2022
End Header --------------------------------------------------------*/

#include "pch.h"
#include "Texture.h"
#include <stb_image.h>
#include "Shader.h"

static int CurrTexNum{ -1 };

Texture::Texture(std::string fileName)
{
    
    image = stbi_load(fileName.c_str(), &w, &h, &comp, STBI_rgb);

    if (image == nullptr) {
        std::cout<<"Failed to load image: " << fileName<<std::endl;
    }
    else {
        glGenTextures(1, &textureID);

        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

        glBindTexture(GL_TEXTURE_2D, 0);

        stbi_image_free(image);
        ///////////////////////////
        ++CurrTexNum;
        textureNumber = CurrTexNum;
        //////////////////////////
    }
}

Texture::~Texture()
{
    //--CurrTexNum;
    unbindTexture();
    glDeleteTextures(1,&textureID);
}

void Texture::bindTexture()
{
    glActiveTexture(GL_TEXTURE0+ textureNumber);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::unbindTexture()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::sendTextureDataUniform(std::shared_ptr<Shader> shader, const std::string uniformName)
{
    shader->sendUniform1iv(uniformName.c_str(),textureNumber);
}
