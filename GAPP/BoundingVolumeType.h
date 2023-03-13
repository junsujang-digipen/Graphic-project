/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: BoundingVolumeType.h
Purpose: For storing Entity datas
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 2 - Bounding Volumes
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 03/03/2023
End Header --------------------------------------------------------*/
#pragma once



enum class BoundingVolumeType
{
	AABB,
	RITTER,
	LARSSON,
	PCA,
	COUNT
};