#pragma once

#include "Essentials.h"
#include "Vector3.h"
#include "Material.h"
#include "Texture.h"

class ConstantMedium : public Hittable
{
public:
	std::shared_ptr<Hittable> boundary;
	std::shared_ptr<Material> phaseFunction;
	double neg_inv_density;

	ConstantMedium(std::shared_ptr<Hittable> _boundary,double density, std::shared_ptr<Texture> texture)
		: boundary(_boundary), neg_inv_density(-1/density), phaseFunction(std::make_shared<Isotropic>(texture)) {}

	ConstantMedium(std::shared_ptr<Hittable> _boundary, double density, Color color)
		: boundary(_boundary), neg_inv_density(-1 / density), phaseFunction(std::make_shared<Isotropic>(color)) {}

	virtual bool BoundingBox(double time0, double time1, AABB& outputBox) const override
	{
		return boundary->BoundingBox(time0, time1, outputBox);
	}

	virtual bool Hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const override
	{
		HitRecord record1, record2;

		if (!boundary->Hit(r, -infinity, infinity, record1))
			return false;

		if (!boundary->Hit(r, record1.t + 0.0001, infinity, record2))
			return false;

		if (record1.t < tMin)
			record1.t = tMin;

		if (record2.t > tMax)
			record2.t = tMax;

		if (record1.t > record2.t)
			return false;

		if (record1.t < 0) record1.t = 0;

		const auto rayLength = r.GetDirection().GetLength();
		const auto distanceInsideBoundary = (record2.t - record1.t) * rayLength;
		const auto hitDistance = neg_inv_density * log(RandomDouble());

		if (hitDistance > distanceInsideBoundary)
			return false;

		rec.t = record1.t + hitDistance / rayLength;
		rec.point = r.At(rec.t);

		rec.normal = Vector3(1, 0, 0);
		rec.isFrontFace = true;
		rec.materialPtr = phaseFunction;

		return true;

	}

};