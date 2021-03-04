#pragma once

#include <cmath>
#include <iostream>
#include "Essentials.h"
class Vector3
{
public:
	double e[3];

public:
	Vector3() : e{0,0,0} {}
	Vector3(double e0, double e1,double e2) : e{e0,e1,e2} {}

	double GetX() const { return e[0]; }
	double GetY() const { return e[1]; }
	double GetZ() const { return e[2]; }

	Vector3 operator-() const { return Vector3(-e[0], -e[1], -e[2]); }
	double operator[] (int i) const { return e[i]; }
	double& operator[] (int i) { return e[i]; }

	Vector3& operator+=(const Vector3& rV)
	{
		e[0] += rV.e[0];
		e[1] += rV.e[1];
		e[2] += rV.e[2];
		return *this;
	}

	Vector3& operator*=(const double t)
	{
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
		return *this;
	}

	Vector3& operator/=(const double t)
	{
		return *this *= 1 / t;
	}

	double GetLengthSquared() const
	{
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}

	double GetLength() const
	{
		return std::sqrt(GetLengthSquared());		
	}

	inline static Vector3 Random()
	{
		return Vector3(RandomDouble(), RandomDouble(), RandomDouble());
	}

	inline static Vector3 Random(double min,double max)
	{
		return Vector3(RandomDouble(min,max), RandomDouble(min,max), RandomDouble(min,max));
	}



	inline bool isNearZero() const
	{
		const auto s = 0.00001;
		return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
	}
};

using Point3 = Vector3;
using Color = Vector3;

inline std::ostream& operator<<(std::ostream& out, const Vector3& v)
{
	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline Vector3 operator+(const Vector3& u, const Vector3& v)
{
	return Vector3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline Vector3 operator-(const Vector3& u, const Vector3& v)
{
	return Vector3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline Vector3 operator*(const Vector3& u, const Vector3& v)
{
	return Vector3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline Vector3 operator*(const Vector3& v, double t)
{
	return Vector3(t * v.e[0], t * v.e[1], t * v.e[2]);
}
/*
inline Vector3 operator*(const Vector3 &v, double t)
{
	return v * t;
}
*/
inline Vector3 operator/ (Vector3 v, double t)
{
	return v * (1 / t);
}

inline double Dot(const Vector3& u, const Vector3& v)
{
	return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

inline Vector3 Cross(const Vector3& u, const Vector3& v)
{
	return Vector3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
				   u.e[2] * v.e[0] - u.e[0] * v.e[2],
				   u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline Vector3 GetUnitVector(Vector3 v)
{
	return v / v.GetLength();
}

Vector3 RandominUnitSphere()
{
	while (true)
	{
		auto p = Vector3::Random(-1, 1);
		if (p.GetLengthSquared() >= 1) continue;
		return p;
	}
}

Vector3 RandomUnitVector()
{
	return GetUnitVector(RandominUnitSphere());
}

Vector3 RandominHemisphere(const Vector3& normal)
{
	Vector3 inUnitSphere = RandominUnitSphere();
	if (Dot(inUnitSphere, normal) > 0.0)
		return inUnitSphere;
	else
		return -inUnitSphere;
}

Vector3 Reflect(const Vector3& v, const Vector3& n)
{
	return v - n * Dot(v, n) * 2;
}

Vector3 Refract(const Vector3& uv, const Vector3& n, double etaRatio)
{
	auto cosTheta = fmin(Dot(-uv, n), 1.0);
	Vector3 outRayPerp = (uv +  n * cosTheta) * etaRatio;
	Vector3 outRayParallel = n * sqrt(fabs(1.0 - outRayPerp.GetLengthSquared())) * -1;
	return outRayPerp + outRayParallel;
}

Vector3 RandominUnitDisk()
{
	while (true)
	{
		auto p = Vector3(RandomDouble(-1, 1), RandomDouble(-1, 1), 0);
		if (p.GetLengthSquared() >= 1) continue;
		return p;
	}
}

