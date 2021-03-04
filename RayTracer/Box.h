#pragma once

#include "Essentials.h"
#include "rectangle.h"
#include "Vector3.h"
#include "HittableList.h"

class Box : public Hittable
{
public:
	Point3 minBox;
	Point3 maxbox;
	HittableList sides;

public:
	
	Box() {};

	Box(const Point3& p0, const Point3& p1, std::shared_ptr<Material> material)
	{
		minBox = p0;
		maxbox = p1;

		sides.AddObject(std::make_shared<RectangleXY>(p0.GetX(), p1.GetX(), p0.GetY(), p1.GetY(), p1.GetZ(),material));
		sides.AddObject(std::make_shared<RectangleXY>(p0.GetX(), p1.GetX(), p0.GetY(), p1.GetY(), p0.GetZ(),material));
	
		sides.AddObject(std::make_shared<RectangleXZ>(p0.GetX(), p1.GetX(), p0.GetZ(), p1.GetZ(), p1.GetY(),material));
		sides.AddObject(std::make_shared<RectangleXZ>(p0.GetX(), p1.GetX(), p0.GetZ(), p1.GetZ(), p0.GetY(),material));
	
		sides.AddObject(std::make_shared<RectangleYZ>(p0.GetY(), p1.GetY(), p0.GetZ(), p1.GetZ(), p1.GetX(),material));
		sides.AddObject(std::make_shared<RectangleYZ>(p0.GetY(), p1.GetY(), p0.GetZ(), p1.GetZ(), p0.GetX(),material));
	}

	virtual bool Hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const override
	{
		return sides.Hit(r, tMin, tMax,rec);
	}
	virtual bool BoundingBox(double time0, double time1, AABB& outputBox) const override
	{
		outputBox = AABB(minBox, maxbox);
		return true;
	}

};

class YRotated : public Hittable
{
public:
	AABB box;
	bool hasBox;
	double sinTheta;
	double cosTheta;
	std::shared_ptr<Hittable> ptr;

public:
	YRotated(std::shared_ptr<Hittable> p, double angle) : ptr(p)
	{
		auto radians = DegreeToRadian(angle);
		sinTheta = sin(radians);
		cosTheta = cos(radians);
		hasBox = ptr->BoundingBox(0, 1, box);

		Point3 min(infinity, infinity, infinity);
		Point3 max(-infinity, -infinity, -infinity);

		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				for (int k = 0; k < 2; k++)
				{
					auto x = i * box.GetMax().GetX() + (1 - i) * box.GetMin().GetX();
					auto y = j * box.GetMax().GetY() + (1 - j) * box.GetMin().GetY();
					auto z = k * box.GetMax().GetZ() + (1 - k) * box.GetMin().GetZ();

					auto newX = cosTheta * x + sinTheta * z;
					auto newZ = -sinTheta * x + cosTheta * z;

					Vector3 vec3(newX, y, newZ);

					for (int c = 0; c < 3; c++)
					{
						min[c] = fmin(min[c], vec3[c]);
						max[c] = fmax(max[c], vec3[c]);
					}
				}
			}
		}

		box = AABB(min, max);
	}

	virtual bool BoundingBox(double time0, double time1, AABB& outputBox) const override
	{
		outputBox = box;
		return true;
	}

	virtual bool Hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const override
	{
		auto origin = r.GetOrigin();
		auto direction = r.GetDirection();

		origin[0] = cosTheta * r.GetOrigin()[0] - sinTheta * r.GetOrigin()[2];
		origin[2] = sinTheta * r.GetOrigin()[0] + cosTheta * r.GetOrigin()[2];

		direction[0] = cosTheta * r.GetDirection()[0] - sinTheta * r.GetDirection()[2];
		direction[2] = sinTheta * r.GetDirection()[0] + cosTheta * r.GetDirection()[2];

		Ray rotatedRay(origin, direction, r.GetTime());

		if (!ptr->Hit(rotatedRay, tMin, tMax, rec))
			return false;

		auto point = rec.point;
		auto normal = rec.normal;

		point[0] = cosTheta * rec.point[0] + sinTheta * rec.point[2];
		point[2] = -sinTheta * rec.point[0] + cosTheta * rec.point[2];

		normal[0] = cosTheta * rec.normal[0] + sinTheta * rec.normal[2];
		normal[2] = -sinTheta * rec.normal[0] + cosTheta * rec.normal[2];

		rec.point = point;
		rec.SetFaceNormal(rotatedRay, normal);

		return true;
	}

};

class Translated : public Hittable
{
public:
	std::shared_ptr<Hittable> ptr;
	Vector3 offset;

	Translated(std::shared_ptr<Hittable> _ptr,const Vector3 _offset) : ptr(_ptr), offset(_offset) {}

	virtual bool Hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const override
	{
		Ray movedRay(r.GetOrigin() - offset, r.GetDirection(), r.GetTime());

		if (!ptr->Hit(movedRay, tMin, tMax, rec))
			return false;

		rec.point += offset;
		rec.SetFaceNormal(movedRay, rec.normal);

		return true;
	}

	virtual bool BoundingBox(double time0, double time1, AABB& outputBox) const override
	{
		if (!ptr->BoundingBox(time0, time1, outputBox))
			return false;

		outputBox = AABB(outputBox.GetMin() + offset, outputBox.GetMax() + offset);

		return true;
	}
};