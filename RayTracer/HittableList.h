#pragma once

#include "Hittable.h"

#include <memory>
#include <vector>

class HittableList : public Hittable
{
public:
	std::vector<std::shared_ptr<Hittable>> objects;

public:
	HittableList() {}
	HittableList(std::shared_ptr<Hittable> object) { AddObject(object); }

	void AddObject(std::shared_ptr<Hittable> object) { objects.push_back(object); }
	void ClearAllObjects() { objects.clear(); }

	virtual bool BoundingBox(double time0, double time1, AABB& outputBox) const override;
	virtual bool Hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const override;
};

bool HittableList::Hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const
{
	HitRecord tempRecord;
	bool hitAnything = false;
	auto closestSoFar = tMax;

	for (const auto& object : objects)
	{
		if (object->Hit(r, tMin, closestSoFar, tempRecord))
		{
			hitAnything = true;
			closestSoFar = tempRecord.t;
			rec = tempRecord;
		}
	}

	return hitAnything;
}

bool HittableList::BoundingBox(double time0, double time1, AABB& outputBox) const
{
	if (objects.empty())
		return false;

	AABB tempBox;
	bool firstBox = true;

	for (const auto& object : objects)
	{
		if (!object->BoundingBox(time0, time1, tempBox)) return false;
		outputBox = firstBox ? tempBox : SurroundingBox(outputBox, tempBox);
		firstBox = false;
	}

	return false;
}