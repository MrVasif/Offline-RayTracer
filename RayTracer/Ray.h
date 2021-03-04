#pragma once

#include "Vector3.h"

class Ray
{
public:
	Point3 origin;
	Vector3 direction;
	double time;
public:
	Ray() {};
	Ray(const Point3& _origin,const Vector3& _direction, double _time = 0.0) : origin(_origin), direction(_direction),time(_time)
	{}

	Point3 GetOrigin() const { return origin; }
	Vector3 GetDirection() const { return direction; }
	double GetTime() const { return time; }
	Point3 At(double t) const
	{
		return origin + direction * t;
	}
};