#pragma once

#include "Vector3.h"
#include "Hittable.h"
#include "aabb.h"

class Sphere : public Hittable
{
public:
	Point3 center;
	double radius;
	std::shared_ptr<Material> material;
public:
	Sphere() {}
	Sphere(Point3 _center,double _radius, std::shared_ptr<Material> _material) 
		: center(_center), radius(_radius), material(_material) {}

	virtual bool Hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const override;
	virtual bool BoundingBox(double time0, double time1, AABB& outputBox) const override;

private:
	static void GetSphereUV(const Point3& p, double& u, double& v)
	{
		auto theta = acos(-p.GetY());
		auto phi = atan2(-p.GetZ(), p.GetX()) + PI;

		u = phi / (2 * PI);
		v = theta / PI;
	}
};

bool Sphere::Hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const
{
	// Replaced b = 2h.
	Vector3 dist = r.GetOrigin() - center;
	auto a = r.GetDirection().GetLengthSquared();
	auto halfb = Dot(dist, r.GetDirection());
	auto c = dist.GetLengthSquared() - radius * radius;
	auto discriminant = halfb * halfb - a * c;
	if (discriminant < 0)
		return false;
	auto discriminantSqrt = sqrt(discriminant);

	auto root = (-halfb - discriminantSqrt) / a;
	if (root < tMin || tMax < root)
	{
		root = (-halfb + discriminantSqrt) / a;
		if (root < tMin || tMax < root)
			return false;
	}

	rec.t = root;
	rec.point = r.At(rec.t);
	Vector3 outwardNormal = (rec.point - center) / radius;
	rec.SetFaceNormal(r, outwardNormal);
	GetSphereUV(outwardNormal, rec.u, rec.v);
	rec.materialPtr = material;

	return true;
}

bool Sphere::BoundingBox(double time0, double time1, AABB& outputBox) const
{
	outputBox = AABB(center - Vector3(radius, radius, radius),
					 center + Vector3(radius, radius, radius));
	return true;
}


class MovingSphere : public Hittable
{
public:
	Point3 center0, center1;
	double time0, time1;
	double radius;
	std::shared_ptr<Material> material;

public:
	MovingSphere() {};
	MovingSphere(Point3 _center0, Point3 _center1, double _time0, double _time1, double _radius, std::shared_ptr<Material> _material)
		: center0(_center0), center1(_center1), time0(_time0), time1(_time1), radius(_radius), material(_material)
	{};

	virtual bool Hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const override;
	virtual bool BoundingBox(double time0, double time1, AABB& outputBox) const override;
	Point3 GetCenter(double time) const
	{
		return center0 + (center1 - center0) *((time - time0) / (time1 - time0));
	}
};

bool MovingSphere::Hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const
{
	Vector3 dist = r.GetOrigin() - GetCenter(r.GetTime());
	auto a = r.GetDirection().GetLengthSquared();
	auto halfb = Dot(dist, r.GetDirection());
	auto c = dist.GetLengthSquared() - radius * radius;
	auto discriminant = halfb * halfb - a * c;
	if (discriminant < 0)
		return false;
	auto discriminantSqrt = sqrt(discriminant);

	auto root = (-halfb - discriminantSqrt) / a;
	if (root < tMin || tMax < root)
	{
		root = (-halfb + discriminantSqrt) / a;
		if (root < tMin || tMax < root)
			return false;
	}

	rec.t = root;
	rec.point = r.At(rec.t);
	Vector3 outwardNormal = (rec.point - GetCenter(r.GetTime())) / radius;
	rec.SetFaceNormal(r, outwardNormal);
	rec.materialPtr = material;

	return true;
}

bool MovingSphere::BoundingBox(double time0, double time1, AABB& outputBox) const
{
	AABB box0(GetCenter(time0) - Vector3(radius, radius, radius),
		GetCenter(time0) + Vector3(radius, radius, radius));

	AABB box1(GetCenter(time1) - Vector3(radius, radius, radius),
		GetCenter(time1) + Vector3(radius, radius, radius));

	outputBox = SurroundingBox(box0, box1);

	return true;
}