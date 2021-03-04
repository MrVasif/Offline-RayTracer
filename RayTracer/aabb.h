#pragma once

#include "Essentials.h"
#include "Vector3.h"
#include "Ray.h"
class AABB
{
private:
	Point3 minimum;
	Point3 maximum;

public:
	AABB() {};
	AABB(const Point3& _minimum,const Point3& _maximum) : minimum(_minimum), maximum(_maximum) {}

	Point3 GetMin() const { return minimum; }
	Point3 GetMax() const { return maximum; }

	bool Hit(const Ray& ray, double& tMin, double& tMax) const
	{
		for (int i = 0; i < 3; i++)
		{
			auto t0 = fmin((minimum[i] - ray.GetOrigin()[i]) / ray.GetDirection()[i],
			(maximum[i] - ray.GetOrigin()[i]) / ray.GetDirection()[i]);
			auto t1 = fmax((minimum[i] - ray.GetOrigin()[i]) / ray.GetDirection()[i],
				(maximum[i] - ray.GetOrigin()[i]) / ray.GetDirection()[i]);
		
			tMin = fmax(t0, tMin);
			tMax = fmin(t1, tMax);

			if (tMin >= tMax)
				return false;
		}

		return true;
	}
};

AABB SurroundingBox(AABB box0, AABB box1)
{
	Point3 small(fmin(box0.GetMin().GetX(), box1.GetMin().GetX()),
				 fmin(box0.GetMin().GetY(), box1.GetMin().GetY()),
				 fmin(box0.GetMin().GetZ(), box1.GetMin().GetZ()));

	Point3 big(fmin(box0.GetMax().GetX(), box1.GetMax().GetX()),
				 fmin(box0.GetMax().GetY(), box1.GetMax().GetY()),
		         fmin(box0.GetMax().GetZ(), box1.GetMax().GetZ()));

	return AABB(small, big);
}

