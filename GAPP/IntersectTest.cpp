/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: IntersectTest.cpp
Purpose: Bounding volume management and generate hierarchy
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 3 - Spatial Partitioning
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 04/13/2023
End Header --------------------------------------------------------*/
#include "pch.h"
#include "IntersectTest.h"
#include "MeshData.h"

float GetLinePlaneIntersectT(glm::vec3 p1, glm::vec3 p2, Plane P) {
	glm::vec3 lineVec{ p2 - p1 };
	float isParallel = glm::dot(glm::vec3{ P.m_Normal }, lineVec);
	if (isParallel == 0) return FLT_MAX;
	float t = (P.m_Normal.w - glm::dot(glm::vec3{ P.m_Normal }, p1)) / isParallel;
	return t;
}

PlaneIntersectType TriandPlaneIntersect(const Triangle& tri, const Plane& p) {
	const float w1{ p.DistofPoint(tri.v1) };
	const float w2{ p.DistofPoint(tri.v2) };
	const float w3{ p.DistofPoint(tri.v3) };
	const bool bw1{ w1 >= 0 };
	const bool bw2{ w2 >= 0 };
	const bool bw3{ w3 >= 0 };
	const bool bw1InEp{ abs(w1) <= 0.1f };
	const bool bw2InEp{ abs(w2) <= 0.1f };
	const bool bw3InEp{ abs(w3) <= 0.1f };
	int FrontOrBehind{};
	//bool isCoplanar{ true };
	int epNum{ 3 };
	if (bw1InEp == false) {
		//isCoplanar = false;
		FrontOrBehind += bw1 ? 1 : -1;
		epNum -= 1;
	}
	if (bw2InEp == false) {
		//isCoplanar = false;
		FrontOrBehind += bw2 ? 1 : -1;
		epNum -= 1;
	}
	if (bw3InEp == false) {
		//isCoplanar = false;
		FrontOrBehind += bw3 ? 1 : -1;
		epNum -= 1;
	}
	PlaneIntersectType returnVal{ FrontOrBehind < 0 ? PlaneIntersectType::Behind : PlaneIntersectType::Front };

	if (epNum == 3) {
		return PlaneIntersectType::Coplanar;
	}
	else if (epNum == 2) {
		return returnVal;
	}
	return abs(FrontOrBehind) < 2 ? PlaneIntersectType::Straddling : returnVal;
}

std::vector<glm::vec3> GetTriandPlaneIntersectPoint(const Triangle& tri, const Plane& p)
{
	float t1 = GetLinePlaneIntersectT(tri.v1, tri.v2, p);
	float t2 = GetLinePlaneIntersectT(tri.v2, tri.v3, p);
	float t3 = GetLinePlaneIntersectT(tri.v3, tri.v1, p);
	std::vector<glm::vec3> reVal{};
	float one{1.f - FLT_EPSILON };
	if (t1 < one && t1 > FLT_EPSILON) {
		reVal.push_back(tri.v1 + t1*(tri.v2 - tri.v1));
	}
	if (t2 < one && t2 > FLT_EPSILON) {
		reVal.push_back(tri.v2 + t2 * (tri.v3 - tri.v2));
	}
	if (t3 < one && t3 > FLT_EPSILON) {
		reVal.push_back(tri.v3 + t3 * (tri.v1 - tri.v3));
	}
	//std::cout << reVal.size() << std::endl;
	return reVal;
}
std::vector<glm::vec3> GetTriandBoxIntersectPoint(const Triangle& tri, const Box3D& b)
{//hmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm...
	std::vector<Plane> planes{ b.p1,b.p2 ,b.p3 ,b.p4 ,b.p5 ,b.p6 };
	std::vector<glm::vec3> reVal{};
	for (auto& p:planes){
		if (TriandPlaneIntersect(tri, p) == PlaneIntersectType::Straddling) {
			std::vector<glm::vec3> temp = GetTriandPlaneIntersectPoint(tri, p);
			for (auto& point : temp) {
				reVal.push_back(point);
			}
		}
	}
	//std::cout << reVal.size() << std::endl;
	for (int i = 0; i < reVal.size(); ++i) {
		if (PointBoxIntersect(reVal[i], b) == false) {
			reVal.erase(reVal.begin() + i);
			--i;
		}
	}
	return reVal;
}
std::vector<Triangle> GetInsideTriangleInBox(const Triangle& tri, const BoundingBox& box) {
	std::vector<Triangle> returnVal{};
	glm::vec3 normal{ glm::cross(tri.v2 - tri.v1, tri.v3 - tri.v1) };
	//box in check;
	std::vector<glm::vec3> temp = GetTriandBoxIntersectPoint(tri, box);
	if (temp.size() == 0) {
		returnVal.push_back(tri);
		return returnVal;
	}
	Box3D b{ box };
	if (PointBoxIntersect(tri.v1, b) == true) {
		temp.push_back(tri.v1);
	}
	if (PointBoxIntersect(tri.v2, b) == true) {
		temp.push_back(tri.v2);
	}
	if (PointBoxIntersect(tri.v3, b) == true) {
		temp.push_back(tri.v3);
	}
	// make triangle
	for (int i = 3; i < temp.size(); ++i) {
		glm::vec3 iToi{ temp[i-1] - temp[0] };
		glm::vec3 tempNormal(glm::cross(iToi, temp[i] - temp[0]));
		glm::vec3 res{ iToi * tempNormal };
		if (res.x > 0.f && res.y > 0.f && res.z > 0.f) {
			returnVal.push_back(Triangle{ temp[0],temp[i-1],temp[i] });
		}
		else {
			returnVal.push_back(Triangle{ temp[0],temp[i],temp[i-1] });
		}
	}
	return returnVal;
}

bool PointBoxIntersect(const glm::vec3& point, const Box3D& box) {
	std::vector<Plane> planes{ box.p1,box.p2 ,box.p3 ,box.p4 ,box.p5 ,box.p6 };
	for (auto& p : planes) {
		const float w1{ p.DistofPoint(point) };
		if (w1 < -FLT_EPSILON) {
			return false;
		}
	}
	return true;
}
bool TriandBoxIntersect(const Triangle& tri, const Box3D& box) {
	if (TriandPlaneIntersect(tri, box.p1) == PlaneIntersectType::Behind
		|| TriandPlaneIntersect(tri, box.p2) == PlaneIntersectType::Behind
		|| TriandPlaneIntersect(tri, box.p3) == PlaneIntersectType::Behind
		|| TriandPlaneIntersect(tri, box.p4) == PlaneIntersectType::Behind
		|| TriandPlaneIntersect(tri, box.p5) == PlaneIntersectType::Behind
		|| TriandPlaneIntersect(tri, box.p6) == PlaneIntersectType::Behind)
	{
		return false;
	}
	return true;
}

PlaneIntersectType PlaneBoxIntersect(const Plane& p, const BoundingBox& box)
{
	//TODO
	constexpr int size{8};
	std::vector<glm::vec3> points(size, box.Center );
	points[0] += glm::vec3{ box.HalfExtend.x,box.HalfExtend.y,box.HalfExtend.z };
	points[1] += glm::vec3{ -box.HalfExtend.x,box.HalfExtend.y,box.HalfExtend.z };
	points[2] += glm::vec3{ box.HalfExtend.x,-box.HalfExtend.y,box.HalfExtend.z };
	points[3] += glm::vec3{ -box.HalfExtend.x,-box.HalfExtend.y,box.HalfExtend.z };
	points[4] += glm::vec3{ box.HalfExtend.x,box.HalfExtend.y,-box.HalfExtend.z };
	points[5] += glm::vec3{ -box.HalfExtend.x,box.HalfExtend.y,-box.HalfExtend.z };
	points[6] += glm::vec3{ box.HalfExtend.x,-box.HalfExtend.y,-box.HalfExtend.z };
	points[7] += glm::vec3{ -box.HalfExtend.x,-box.HalfExtend.y,-box.HalfExtend.z };
	std::vector<float> dists(size, 0.f);
	
	for (int i = 0; i < size;++i) {
		dists[i] = p.DistofPoint(points[i]);
		if (dists[i] < FLT_EPSILON && dists[i] > FLT_EPSILON) {
			dists[i] = 0.f;
			return PlaneIntersectType::Straddling;
		}
	}
	if (dists[0] > 0.f) {
		for (int i = 1; i < size; ++i) {
			if (dists[i] < 0.f) {
				return PlaneIntersectType::Straddling;
			}
		}
		return PlaneIntersectType::Front;
	}
	else {
		for (int i = 1; i < size; ++i) {
			if (dists[i] > 0.f) {
				return PlaneIntersectType::Straddling;
			}
		}
		return PlaneIntersectType::Behind;
	}
}

std::vector<MeshData> PlaneMeshSplit(const Plane& p, const glm::mat4& objMat, const MeshData& meshData)
{
	std::vector<MeshData> reVal(2);
	int size{ (int)meshData.idxDatas.size() };
	for (int i = 0; i < size; i += 3) {
		Triangle tri{
			glm::vec3{objMat * glm::vec4{meshData.VertexDatas[meshData.idxDatas[i]],1.f} },
			glm::vec3{objMat * glm::vec4{meshData.VertexDatas[meshData.idxDatas[i + 1]],1.f} } ,
			glm::vec3{objMat * glm::vec4{meshData.VertexDatas[meshData.idxDatas[i + 2]],1.f} } };
		glm::vec3 normal{ glm::cross(tri.v2 - tri.v1, tri.v3 - tri.v1) };
		PlaneIntersectType rest{ TriandPlaneIntersect(tri, p) };
		if (rest == PlaneIntersectType::Straddling) {
			std::vector<glm::vec3> frontIntersectPoint = GetTriandPlaneIntersectPoint(tri,p);
			std::vector<glm::vec3> behindIntersectPoint(frontIntersectPoint);
			float d1{ p.DistofPoint(tri.v1) };
			if (d1 > -0.1f) {
				frontIntersectPoint.push_back(tri.v1);
			}
			if (d1 < 0.1f) {
				behindIntersectPoint.push_back(tri.v1);
			}
			float d2{ p.DistofPoint(tri.v2) };
			if (d2 > -0.1f) {
				frontIntersectPoint.push_back(tri.v2);
			}
			if (d2 < 0.1f) {
				behindIntersectPoint.push_back(tri.v2);
			}
			float d3{ p.DistofPoint(tri.v3) };
			if (d3 > -0.1f) {
				frontIntersectPoint.push_back(tri.v3);
			}
			if (d3 < 0.1f) {
				behindIntersectPoint.push_back(tri.v3);
			}
			//front
			for (int idx = 3; idx < frontIntersectPoint.size(); ++idx) {
				glm::vec3 iToi{ frontIntersectPoint[idx - 1] - frontIntersectPoint[0] };
				glm::vec3 tempNormal(glm::cross(iToi, frontIntersectPoint[idx] - frontIntersectPoint[0]));
				glm::vec3 res{ normal * tempNormal };
				int nowidx{ (int)reVal[0].VertexDatas.size() };
				if (res.x > 0.f && res.y > 0.f && res.z > 0.f) {
					reVal[0].VertexDatas.push_back(frontIntersectPoint[0]);
					reVal[0].VertexDatas.push_back(frontIntersectPoint[idx - 1]);
					reVal[0].VertexDatas.push_back(frontIntersectPoint[idx]);
				}
				else {
					reVal[0].VertexDatas.push_back(frontIntersectPoint[0]);
					reVal[0].VertexDatas.push_back(frontIntersectPoint[idx]);
					reVal[0].VertexDatas.push_back(frontIntersectPoint[idx -1]);
				}
				reVal[0].idxDatas.push_back(nowidx);
				reVal[0].idxDatas.push_back(nowidx + 1);
				reVal[0].idxDatas.push_back(nowidx + 2);
			}
			//behind
			for (int idx = 3; idx < behindIntersectPoint.size(); ++idx) {
				glm::vec3 iToi{ behindIntersectPoint[idx - 1] - behindIntersectPoint[0] };
				glm::vec3 tempNormal(glm::cross(iToi, behindIntersectPoint[idx] - behindIntersectPoint[0]));
				glm::vec3 res{ normal * tempNormal };
				int nowidx{ (int)reVal[1].VertexDatas.size() };
				if (res.x > 0.f && res.y > 0.f && res.z > 0.f) {
					reVal[1].VertexDatas.push_back(behindIntersectPoint[0]);
					reVal[1].VertexDatas.push_back(behindIntersectPoint[idx - 1]);
					reVal[1].VertexDatas.push_back(behindIntersectPoint[idx]);
				}
				else {
					reVal[1].VertexDatas.push_back(behindIntersectPoint[0]);
					reVal[1].VertexDatas.push_back(behindIntersectPoint[idx]);
					reVal[1].VertexDatas.push_back(behindIntersectPoint[idx - 1]);
				}
				reVal[1].idxDatas.push_back(nowidx);
				reVal[1].idxDatas.push_back(nowidx + 1);
				reVal[1].idxDatas.push_back(nowidx + 2);
			}
		}
		else if (rest == PlaneIntersectType::Front) 
		{
			int nowidx{ (int)reVal[0].VertexDatas.size() };
			reVal[0].VertexDatas.push_back(tri.v1);
			reVal[0].VertexDatas.push_back(tri.v2);
			reVal[0].VertexDatas.push_back(tri.v3);
			reVal[0].idxDatas.push_back(nowidx);
			reVal[0].idxDatas.push_back(nowidx + 1);
			reVal[0].idxDatas.push_back(nowidx + 2);
		}
		else
		{
			int nowidx{ (int)reVal[1].VertexDatas.size() };
			reVal[1].VertexDatas.push_back(tri.v1);
			reVal[1].VertexDatas.push_back(tri.v2);
			reVal[1].VertexDatas.push_back(tri.v3);
			reVal[1].idxDatas.push_back(nowidx);
			reVal[1].idxDatas.push_back(nowidx + 1);
			reVal[1].idxDatas.push_back(nowidx + 2);
		}
	}
	return reVal;
}

bool BoxBoxIntersect(const BoundingBox& boxA, const BoundingBox& boxB)
{
	glm::vec3 centerToCenter{boxA.Center - boxB.Center};
	glm::vec3 MinDist{ boxA.HalfExtend + boxB.HalfExtend };
	if (MinDist.x > abs(centerToCenter.x) && MinDist.y > abs(centerToCenter.y) && MinDist.z > abs(centerToCenter.z)) {
		return true;
	}
	return false;
}

Box3D::Box3D(const BoundingBox& boundingBox) { // TODO abs???
	glm::vec3 xPoint1{ boundingBox.Center };
	glm::vec3 xPoint2{ boundingBox.Center };
	xPoint1.x += boundingBox.HalfExtend.x;
	xPoint2.x -= boundingBox.HalfExtend.x;

	glm::vec3 yPoint1{ boundingBox.Center };
	glm::vec3 yPoint2{ boundingBox.Center };
	yPoint1.y += boundingBox.HalfExtend.y;
	yPoint2.y -= boundingBox.HalfExtend.y;

	glm::vec3 zPoint1{ boundingBox.Center };
	glm::vec3 zPoint2{ boundingBox.Center };
	zPoint1.z += boundingBox.HalfExtend.z;
	zPoint2.z -= boundingBox.HalfExtend.z;


	p1.m_Normal = glm::vec4{ glm::vec3{-1.f,0.f,0.f},-xPoint1.x };
	p2.m_Normal = glm::vec4{ glm::vec3{1.f,0.f,0.f},xPoint2.x };

	p3.m_Normal = glm::vec4{ glm::vec3{0.f,-1.f,0.f},-yPoint1.y };
	p4.m_Normal = glm::vec4{ glm::vec3{0.f,1.f,0.f},yPoint2.y };

	p5.m_Normal = glm::vec4{ glm::vec3{0.f,0.f,-1.f},-zPoint1.z };
	p6.m_Normal = glm::vec4{ glm::vec3{0.f,0.f,1.f},zPoint2.z };
}