#pragma once

#include "Hittable.h"
#include "HittableList.h"
#include "aabb.h"
#include "Essentials.h"

class BVHnode : public Hittable
{
public:
	std::shared_ptr<Hittable> left;
	std::shared_ptr<Hittable> right;
	AABB box;

public:

	BVHnode();

	BVHnode(const HittableList& list, double time0, double time1) {}

	BVHnode(const std::vector<std::shared_ptr<Hittable>>& objects,
		size_t start, size_t end, double time0, double time1);

	virtual bool Hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const override;
	virtual bool BoundingBox(double time0, double time1, AABB& outputBox) const override;
	
};

inline bool BoxCompare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b, int axis)
{
	AABB boxA;
	AABB boxB;

	if (!a->BoundingBox(0, 0, boxA) || !b->BoundingBox(0, 0, boxB))
		std::cerr << "Constructor Error: BVH.\n";

	return boxA.GetMin().e[axis] < boxB.GetMin().e[axis];
}

inline bool BoxCompareX(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b)
{
	return BoxCompare(a, b, 0);
}

inline bool BoxCompareY(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b)
{
	return BoxCompare(a, b, 1);
}

inline bool BoxCompareZ(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b)
{
	return BoxCompare(a, b, 2);
}

bool BVHnode::BoundingBox(double time0, double time1, AABB& outputBox) const
{
	outputBox = box;
	return true;
}

bool BVHnode::Hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const
{
	if (!box.Hit(r,tMin,tMax))
		return false;

	bool hitLeft = left->Hit(r, tMin, tMax, rec);
	bool hitRight = right->Hit(r, tMin, hitLeft ? rec.t : tMax, rec);

	return hitLeft || hitRight;
}

 BVHnode::BVHnode(const std::vector<std::shared_ptr<Hittable>>& objects,
	size_t start, size_t end, double time0, double time1)
{
	 auto srcObjects = objects;

	 int axis = RandomInt(0, 2);

	 auto comparator = (axis == 0) ? BoxCompareX
					 : (axis == 1) ? BoxCompareY
					 : BoxCompareZ;

	 size_t objectSpan = end - start;

	 if (objectSpan == 1)
	 {
		 left = right = srcObjects[start];
	 }
	 else if (objectSpan == 2)
	 {
		 if (comparator(srcObjects[start], srcObjects[start + 1]))
		 {
			 left = objects[start];
			 right = objects[start + 1];
		 }
		 else
		 {
			 left = objects[start + 1];
			 right = objects[start];
		 }
	 }
	 else
	 {
		 std::sort(srcObjects.begin() + start, srcObjects.begin() + end, comparator);

		 auto middle = start + objectSpan / 2;
		 left = std::make_shared<BVHnode>(objects, start, middle, time0, time1);
		 right = std::make_shared<BVHnode>(objects, middle, end, time0, time1);
	 }

	 AABB leftBox;
	 AABB rightBox;

	 if (!left->BoundingBox(time0, time1, leftBox) || !right->BoundingBox(time0, time1, rightBox))
	 {
		 std::cerr << "No bounding box in constructor!. \n";
	 }

	 box = SurroundingBox(leftBox, rightBox);

}