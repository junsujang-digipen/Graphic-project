/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: AssimpLoader.cpp
Purpose: For load obj file
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 2 - Bounding Volumes
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 02/26/2023
End Header --------------------------------------------------------*/
#include "pch.h"
#include "AssimpLoader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Texture.h"
#include "OBJLoader.h"

void processNode(AssimpLoader& assimp , aiNode* node, const aiScene* scene);
void processMesh(AssimpLoader& assimp, aiMesh* mesh, const aiScene* scene);
void loadMaterialTextures(AssimpLoader& assimp, aiMaterial* mat, aiTextureType type, std::string typeName);

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
    MeshData temp{};
    temp.VertexDatas = VertexDatas;
    temp.FaceNormalDatas = FaceNormalDatas;
    temp.VertexNormalDatas = VertexNormalDatas;

    temp.VertexTextureDatas = VertexTextureDatas;
    temp.idxDatas = idxDatas;
    temp.primitive_type = primitive_type;
    temp.boundBoxMax = boundBoxMax;
    temp.boundBoxMin = boundBoxMin;

    makeNormalDrawVec(VertexDatas, VertexNormalDatas, temp.VertexNormalDrawVec, objScale, 2.f);
    if (FaceNormalDatas.size() > 0)
    {
        std::vector<glm::vec3> vad{};
        unsigned indxSize{ static_cast<unsigned>(idxDatas.size()) };
        for (unsigned i = 0; i < indxSize; ++i) {
            vad.push_back(VertexDatas[idxDatas[i]]);
        }
        makeFaceNormalDrawVec(vad, FaceNormalDatas, temp.FaceNormalDrawVec, objScale, 1.f);
    }
    return temp;
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
    int size = static_cast<int>(assimp.VertexDatas.size());
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        glm::vec3 vertex{ mesh->mVertices[i].x,mesh->mVertices[i].y,mesh->mVertices[i].z};
        assimp.VertexDatas.push_back(vertex);
        assimp.boundBoxMin.x = std::min(vertex.x, assimp.boundBoxMin.x);
        assimp.boundBoxMin.y = std::min(vertex.y, assimp.boundBoxMin.y);
        assimp.boundBoxMin.z = std::min(vertex.z, assimp.boundBoxMin.z);

        assimp.boundBoxMax.x = std::max(vertex.x, assimp.boundBoxMax.x);
        assimp.boundBoxMax.y = std::max(vertex.y, assimp.boundBoxMax.y);
        assimp.boundBoxMax.z = std::max(vertex.z, assimp.boundBoxMax.z);

        glm::vec3 normal{ mesh->mNormals[i].x,mesh->mNormals[i].y,mesh->mNormals[i].z };
        assimp.VertexNormalDatas.push_back(normal);
        if (mesh->mTextureCoords[0]){
            glm::vec2 UV{ mesh->mTextureCoords[0][i].x ,mesh->mTextureCoords[0][i].y };
            assimp.VertexUVDatas.push_back(UV);
        }
        else {
        }

    }
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            assimp.idxDatas.push_back(size + face.mIndices[j]);
    }
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        loadMaterialTextures(assimp, material,aiTextureType_DIFFUSE, "texture_diffuse");
        loadMaterialTextures(assimp, material,aiTextureType_SPECULAR, "texture_specular");
    }
}

void loadMaterialTextures(AssimpLoader& assimp, aiMaterial* mat, aiTextureType type, std::string typeName)
{
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        assimp.textures.push_back(str.C_Str());
        //bool skip = false;
        //for (unsigned int j = 0; j < assimp.textures_loaded.size(); j++)
        //{
        //    if (std::strcmp(assimp.textures_loaded[j].data(), str.C_Str()) == 0)
        //    {
        //        textures.push_back(assimp.textures_loaded[j]);
        //        skip = true;
        //        break;
        //    }
        //}
        //if (!skip)
        //{   
        //    Texture texture;
        //    texture.id = TextureFromFile(str.C_Str(), directory);
        //    texture.type = typeName;
        //    texture.path = str.C_Str();
        //    textures.push_back(texture);
        //    textures_loaded.push_back(texture); 
        //}
    }
    //return textures;
}