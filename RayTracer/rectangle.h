#pragma once

#include "Essentials.h"
#include "Hittable.h"
#include "Vector3.h"
class RectangleXY : public Hittable
{
public:
	std::shared_ptr<Material> material;
	double x0, x1, y0, y1, k; // k = z

public:

	RectangleXY() {}

	RectangleXY(double _x0, double _x1, double _y0, double _y1, double _k, std::shared_ptr<Material> _material)
		: x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), material(_material) {};

	virtual bool Hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const override
	{
		auto t = (k - r.GetOrigin().GetZ()) / r.GetDirection().GetZ();
		if (t < tMin || t > tMax)
			return false;
		
		auto x = r.GetOrigin().GetX() + t * r.GetDirection().GetX();
		auto y = r.GetOrigin().GetY() + t * r.GetDirection().GetY();
		if (x < x0 || x > x1 || y < y0 || y > y1)
			return false;
		
		rec.u = (x - x0) / (x1 - x0);
		rec.v = (y - y0) / (y1 - y0);
		rec.t = t;

		auto outwardNormal = Vector3(0, 0, 1);
		rec.SetFaceNormal(r, outwardNormal);
		rec.materialPtr = material;
		rec.point = r.At(t);
		return true;
	}

	virtual bool BoundingBox(double time0, double time1, AABB& outputBox) const override
	{
		outputBox = AABB(Point3(x0, y0, k - 0.0001), Point3(x1, y1, k + 0.0001));
		return true;
	}

};

class RectangleXZ : public Hittable
{
public:
	std::shared_ptr<Material> material;
	double x0, x1, z0, z1, k;

public:

	RectangleXZ() {};

	RectangleXZ(double _x0, double _x1, double _z0, double _z1, double _k, std::shared_ptr<Material> _material)
		: x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), material(_material) {};

	virtual bool BoundingBox(double time0, double time1, AABB& outputBox) const override
	{
		outputBox = AABB(Point3(x0,k - 0.0001, z0), Point3(x1,k + 0.0001, z1));
		return true;
	}

	virtual bool Hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const override
	{
		auto t = (k - r.GetOrigin().GetY()) / r.GetDirection().GetY();

		if (t < tMin || t > tMax)
			return false;

		auto x = r.GetOrigin().GetX() + t * r.GetDirection().GetX();
		auto z = r.GetOrigin().GetZ() + t * r.GetDirection().GetZ();

		if (x < x0 || x > x1 || z < z0 || z > z1)
			return false;

		rec.u = (x - x0) / (x1 - x0);
		rec.v = (z - z0) / (z1 - z0);
		rec.t = t;

		auto outwardNormal = Vector3(1, 0, 0);
		rec.SetFaceNormal(r, outwardNormal);
		rec.materialPtr = material;
		rec.point = r.At(t);
		return true;
	}

};

class RectangleYZ : public Hittable
{
public:
	std::shared_ptr<Material> material;
	double z0, z1, y0, y1, k; // k = x

public:
	RectangleYZ() {}

	RectangleYZ(double _y0, double _y1, double _z0, double _z1, double _k, std::shared_ptr<Material> _material)
		: y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), material(_material) {};

	virtual bool BoundingBox(double time0, double time1, AABB& outputBox) const override
	{
		outputBox = AABB(Point3(k - 0.0001, y0, z0), Point3(k + 0.0001, y1, z1));
		return true;
	}

	virtual bool Hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const override
	{
		auto t = (k - r.GetOrigin().GetX()) / r.GetDirection().GetX();

		if (t < tMin || t > tMax)
			return false;

		auto y = r.GetOrigin().GetY() + t * r.GetDirection().GetY();
		auto z = r.GetOrigin().GetZ() + t * r.GetDirection().GetZ();

		if (y < y0 || y > y1 || z < z0 || z > z1)
			return false;
		rec.u = (y - y0) / (y1 - y0);
		rec.v = (z - z0) / (z1 - z0);
		rec.t = t;

		auto outwardNormal = Vector3(1, 0, 0);
		rec.SetFaceNormal(r, outwardNormal);
		rec.materialPtr = material;
		rec.point = r.At(t);
		return true;
	}

};