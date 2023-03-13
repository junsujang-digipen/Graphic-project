/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: OBJLoader.cpp
Purpose: For load obj file
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 2 - Bounding Volumes
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 09/29/2022
End Header --------------------------------------------------------*/
#include "pch.h"
#include "OBJLoader.h"
#include <sstream>
#include <fstream>
#include <limits.h>

void OBJLoader::FileLoad(const char* path)
{
	std::ifstream OBJFile{path, std::ios::in};
	//OBJFile.open();
	if (!OBJFile) {
		std::cout << "Unable to open file: "<< path << "\n";
		exit(EXIT_FAILURE);
	}else{
		std::cout << "File opened." << std::endl;
		std::string text{};
		glm::vec3 min{ FLT_MAX }, max{ -FLT_MAX };
		while (OBJFile >> text) {
			if (text == "n") {
				OBJFile >> meshName;
			}
			else if (text == "v") {
				float x{}, y{}, z{};
				OBJFile >> x;
				OBJFile >> y;
				OBJFile >> z;
				
				if (x < min.x) min.x = x;
				if (y < min.y) min.y = y;
				if (z < min.z) min.z = z;

				if (x > max.x) max.x = x;
				if (y > max.y) max.y = y;
				if (z > max.z) max.z = z;

				VertexDatas.push_back(glm::vec3{ x,y,z });
			}
			else if (text == "vt") {
				float x{}, y{}, z{};
				OBJFile >> x;
				OBJFile >> y;
				OBJFile >> z;

				VertexTextureDatas.push_back(glm::vec3{ x,y,z });
			}
			else if (text == "vn") {
				float x{}, y{}, z{};
				OBJFile >> x;
				OBJFile >> y;
				OBJFile >> z;

				VertexNormalDatas.push_back(glm::vec3{ x,y,z });
			}
			else if (text == "f") {
				GLuint idx{};
				std::string indexs{};
				getline(OBJFile, indexs);
				std::istringstream iss{ indexs };
				unsigned numinline{};
				while (iss >> idx) {
					if (numinline >=3) {
						GLuint FanSecondVal = idxDatas.back();
						idxDatas.push_back(idxDatas.front());
						idxDatas.push_back(FanSecondVal);
						++primitive_cnt;
					}
					idxDatas.push_back(idx-1);
					++numinline;
				}
				++primitive_cnt;
			}
			else if (text == "#") {
				getline(OBJFile, text);
			}
		}
		primitive_type = GL_TRIANGLES;
		glm::vec3 minToMax{max.x - min.x,max.y - min.y ,max.z - min.z };
		glm::vec3 center{ (max.x + min.x)/2.f,(max.y + min.y) / 2.f ,(max.z + min.z) / 2.f };
		float maxLength{ minToMax.x};
		
		if (maxLength < minToMax.y) maxLength = minToMax.y;
		if (maxLength < minToMax.z) maxLength = minToMax.z;
		maxLength *= 0.5f;
		for (auto& v:VertexDatas) {
			v -= center;
			v = v / maxLength;
		}
		boundBoxMax = glm::vec3{ max - center } / maxLength;
		boundBoxMin = glm::vec3{ min - center } / maxLength;
		

		//Compute normal
		{
			std::vector<std::vector<int>> forComputeSmoothNormal{};
			forComputeSmoothNormal.resize(VertexDatas.size());
			//Face normal
			unsigned indxSize{static_cast<unsigned>(idxDatas.size())};
			for (unsigned i = 0; i < indxSize;i +=3) {
				glm::vec3 FaceNormal{glm::normalize(glm::cross(VertexDatas[idxDatas[i + 1]] - VertexDatas[idxDatas[i]], VertexDatas[idxDatas[i + 2]] - VertexDatas[idxDatas[i]])) };
				FaceNormalDatas.push_back(FaceNormal);
				FaceNormalDatas.push_back(FaceNormal);
				FaceNormalDatas.push_back(FaceNormal);

				int FaceNormalNum{ static_cast<int>(FaceNormalDatas.size()-1)};
				forComputeSmoothNormal[idxDatas[i]].push_back(FaceNormalNum);
				forComputeSmoothNormal[idxDatas[i+1]].push_back(FaceNormalNum);
				forComputeSmoothNormal[idxDatas[i+2]].push_back(FaceNormalNum);
			}
		
			//Vertex normal
			if (VertexNormalDatas.size() == 0) {
				for (auto& sm : forComputeSmoothNormal) {
					std::vector<glm::vec3> faceNormals{};
					glm::vec3 smoothNormal{};
					for (auto&fn:sm) {
						if (std::find(faceNormals.begin(),faceNormals.end(), FaceNormalDatas[fn]) == faceNormals.end()) {
							faceNormals.push_back(FaceNormalDatas[fn]);
							smoothNormal += FaceNormalDatas[fn];
						}
					}
					VertexNormalDatas.push_back(glm::normalize(smoothNormal));
				}
			}
		}

		std::cout << path<< " file loaded." << std::endl;

		OBJFile.close();
		std::cout << "File close." << std::endl;
	}

}

std::vector<glm::vec3> OBJLoader::VertexArrayData()
{
	std::vector<glm::vec3> vad{};
	unsigned indxSize{ static_cast<unsigned>(idxDatas.size()) };
	for (unsigned i = 0; i < indxSize; ++i) {
		vad.push_back(VertexDatas[idxDatas[i]]);
	}
	return vad;
}

MeshData OBJLoader::makeMeshData(const glm::vec3 &objScale)
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

	makeNormalDrawVec(VertexDatas,VertexNormalDatas, temp.VertexNormalDrawVec, objScale, 2.f);
	if (FaceNormalDatas.size() > 0)
	{
		std::vector<glm::vec3> vertexArrayData{ VertexArrayData() };
		makeFaceNormalDrawVec(vertexArrayData, FaceNormalDatas, temp.FaceNormalDrawVec, objScale, 1.f);
	}
	return temp;
}
