#pragma once

#include<iostream>
#include <limits>
#include <memory>
#include <random>
#include <vector>
#include <algorithm>
#include <cmath>
const double infinity = std::numeric_limits<double>::infinity();
const double PI = 3.1415926535897932385;

inline double DegreeToRadian(double degree)
{
	return degree * PI / 180.0;
}

inline double RandomDouble()
{
	/*
	static std::uniform_real_distribution<double> distribution(0.0, 1.0);
	static std::mt19937 generatorSeed;
	return distribution(generatorSeed);
	*/

	return rand() / (RAND_MAX + 1.0);
}

inline double RandomDouble(double min, double max)
{
	return min + (max - min) * RandomDouble();
}

inline int RandomInt(int min, int max)
{
	return static_cast<int>(RandomDouble(min, max + 1));
}

inline double Clamp(double x, double min, double max)
{
	if (x < min) return min;
	if (x > max) return max;
	return x;
}

