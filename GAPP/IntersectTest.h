/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: IntersectTest.h
Purpose: Bounding volume management and generate hierarchy
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 3 - Spatial Partitioning
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 04/13/2023
End Header --------------------------------------------------------*/
#pragma once
#include "EnttComponentStructures.h"

struct Point4D
{
	glm::vec4 m_coordinates;
};

struct Triangle
{
	glm::vec3 v1{};
	glm::vec3 v2{};
	glm::vec3 v3{};
};

struct Plane
{
	glm::vec4 m_Normal{};
	const float DistofPoint(const glm::vec3& p) const {
		return glm::dot(glm::vec4{p,-1.f}, m_Normal);
	}
};
struct Box3D
{
	Plane p1{};// +x
	Plane p2{};// -x
	Plane p3{};// +y
	Plane p4{};// -y
	Plane p5{};// +z
	Plane p6{};// -z
	Box3D(const BoundingBox& boundingBox);

};

float GetLinePlaneIntersectT(glm::vec3 p1, glm::vec3 p2, Plane P);

enum class PlaneIntersectType {
	Coplanar,
	Front,
	Behind,
	Straddling,
};
PlaneIntersectType TriandPlaneIntersect(const Triangle& tri, const Plane& p);

std::vector<glm::vec3> GetTriandPlaneIntersectPoint(const Triangle& tri, const Plane& p);

std::vector<Triangle> GetInsideTriangleInBox(const Triangle& tri, const BoundingBox& box);

bool PointBoxIntersect(const glm::vec3& point, const Box3D& box);

bool TriandBoxIntersect(const Triangle& tri, const Box3D& box);

PlaneIntersectType PlaneBoxIntersect(const Plane& p, const BoundingBox& box);
struct MeshData;
std::vector<MeshData> PlaneMeshSplit(const Plane& p, const glm::mat4& objMat, const MeshData& meshData);
PlaneIntersectType PlaneMeshIntersect(const Plane& p, const MeshData& meshData);

bool BoxBoxIntersect(const BoundingBox& boxA, const BoundingBox& boxB);