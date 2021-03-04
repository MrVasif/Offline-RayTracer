#pragma once

#include "Ray.h"
#include "aabb.h"
class Material;

struct HitRecord
{
	Point3 point;
	Vector3 normal;
	double t;
	double u;
	double v;
	bool isFrontFace;
	std::shared_ptr<Material> materialPtr;

	inline void SetFaceNormal(const Ray& r, const Vector3& outwardNormal)
	{
		isFrontFace = Dot(r.GetDirection(), outwardNormal) < 0;
		normal = isFrontFace ? outwardNormal : -outwardNormal;
	}
};

class Hittable
{
public:
	virtual bool Hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const = 0;
	virtual bool BoundingBox(double time0, double time1, AABB& outputBox) const = 0;
};