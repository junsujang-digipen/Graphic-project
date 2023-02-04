/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Texture.cpp
Purpose: For storing Entity datas
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 1 - Hybrid Rendering
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 11/04/2022
End Header --------------------------------------------------------*/

#include "pch.h"
#include "Texture.h"
#include <stb_image.h>
#include "Shader.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

Texture::Texture(std::string fileName,unsigned WM): filePath(fileName), wrapMethod(WM)
{
    load();
}

Texture::Texture(std::string fileName, unsigned char* img, int width, int height, unsigned WM)
    : filePath(fileName), w( width), h( height), wrapMethod(WM), image(img)
{
    if (image == nullptr) {
        std::cout << "Weird image: " << filePath << std::endl;
    }
    else {
        glGenTextures(1, &textureID);

        glBindTexture(GL_TEXTURE_2D, textureID);
        //GL_REPEAT
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMethod);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMethod);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

        glBindTexture(GL_TEXTURE_2D, 0);

        stbi_image_free(image);
    }
}

Texture::Texture(unsigned int texID, int width, int height, unsigned WM)
    :textureID(texID), w(width), h(height), wrapMethod(WM)
{
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMethod);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMethod);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
    unload();
}

void Texture::load()
{
    image = stbi_load(filePath.c_str(), &w, &h, &comp, STBI_rgb);

    if (image == nullptr) {
        std::cout << "Failed to load image: " << filePath << std::endl;
    }
    else {
        glGenTextures(1, &textureID);

        glBindTexture(GL_TEXTURE_2D, textureID);
        //GL_REPEAT
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMethod);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMethod);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

        glBindTexture(GL_TEXTURE_2D, 0);

        stbi_image_free(image);
    }
}

void Texture::unload()
{
    unbindTexture();
    glDeleteTextures(1, &textureID);
}

void Texture::setTextureNumber(int num)
{
    textureNumber = num;
}

void Texture::bindTexture()
{
    glActiveTexture(GL_TEXTURE0+ textureNumber);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::unbindTexture()
{
    glActiveTexture(GL_TEXTURE0 + textureNumber);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::sendTextureDataUniform(std::shared_ptr<Shader> shader, const std::string uniformName)
{
    shader->sendUniform1iv(uniformName.c_str(),textureNumber);
}
