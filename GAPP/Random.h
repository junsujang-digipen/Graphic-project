/* Start Header -------------------------------------------------------
Copyright (C) <current year in format 2022> DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Random.h
Purpose: Scene for testing objs and loader and shaders
Language: c++
Platform: x64
Project: junsu.jang, CS350, Assignment 2 - Bounding Volumes
Author: Junsu Jang, junsu.jang, 0055891
Creation date: 11/25/2022
End Header --------------------------------------------------------*/
#pragma once
#include <random>
#include <chrono>



inline double RanGenerator(double min, double max)
{
	static std::chrono::system_clock::time_point zero = std::chrono::system_clock::from_time_t(0);
	static std::chrono::duration<double> t{ std::chrono::system_clock::now() - zero };
	static int Seed{ (int)t.count() };
	static std::knuth_b knuthrand(Seed);
	std::uniform_real_distribution<double> distribution(min, max);
	return distribution(knuthrand);
}
inline float random(double min = 0.0, double max = 1.0) {
	return (float)RanGenerator(min, max);
}
