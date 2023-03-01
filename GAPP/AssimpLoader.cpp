/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: AssimpLoader.cpp
Purpose: For load obj file
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 1 - Hybrid Rendering
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 02/26/2023
End Header --------------------------------------------------------*/
#include "pch.h"
#include "AssimpLoader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

void processNode(AssimpLoader& assimp , aiNode* node, const aiScene* scene);
void processMesh(AssimpLoader& assimp, aiMesh* mesh, const aiScene* scene);

void AssimpLoader::LoadModel(std::string path)
{
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }
    processNode(*this, scene->mRootNode, scene);
}

MeshData AssimpLoader::MakeMeshData(const glm::vec3& objScale)
{
    return MeshData();
}


void processNode(AssimpLoader& assimp, aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(assimp, mesh, scene);
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(assimp, node->mChildren[i], scene);
    }
}

void processMesh(AssimpLoader& assimp, aiMesh* mesh, const aiScene* scene)
{
    assimp.primitive_type = mesh->mPrimitiveTypes;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        glm::vec3 vertex{ mesh->mVertices[i].x,mesh->mVertices[i].y,mesh->mVertices[i].z};
        assimp.VertexDatas.push_back(vertex);
    }
    // indices 贸府
    // material 贸府
    if (mesh->mMaterialIndex >= 0)
    {
    }
}