#pragma once

#include "Essentials.h"
#include "Ray.h"
#include "Vector3.h"
#include "Hittable.h"
#include "Texture.h"
class Material
{
public:
	virtual Color Emitted(double u, double v, const Point3& p) const 
	{
		return Color(0, 0, 0);
	}
	virtual bool Scatter(const Ray& ray, const HitRecord& record, Color& attenuation, Ray& scattered) const = 0;
};

class Lambertian : public Material
{
public:
	std::shared_ptr<Texture> albedo;

public:
	Lambertian(const Color& _albedo) : albedo(std::make_shared<SolidColor>(_albedo)) {}
	Lambertian(std::shared_ptr<Texture> _albedo) : albedo(_albedo) {}

	virtual bool Scatter(const Ray& ray, const HitRecord& record, Color& attenuation, Ray& scattered) const override
	{
		auto scatterDirection = record.normal + RandomUnitVector();
		scattered = Ray(record.point, scatterDirection,ray.GetTime());

		if (scatterDirection.isNearZero())
			scatterDirection = record.normal;
		attenuation = albedo->Value(record.u,record.v,record.point); // albedo / probability
		return true;
	}

};

class Metal : public Material
{
public:
	Color albedo;
	double fuzzyDegree;
public:
	Metal(const Color& _albedo, double _fuzzyDegree) : albedo(_albedo), fuzzyDegree(_fuzzyDegree < 1 ? _fuzzyDegree : 1) {}

	virtual bool Scatter(const Ray& ray, const HitRecord& record, Color& attenuation, Ray& scattered) const override
	{
		Vector3 reflected = Reflect(GetUnitVector(ray.GetDirection()), record.normal);
		scattered = Ray(record.point, reflected + RandominUnitSphere() * fuzzyDegree, ray.GetTime());
		attenuation = albedo;
		return ( Dot(scattered.GetDirection(), record.normal) > 0 );
	}
};

class Dielectric : public Material
{
public:
	double iOR;

public:
	Dielectric(double _iOR) : iOR(_iOR) {}

	virtual bool Scatter(const Ray& ray, const HitRecord& record, Color& attenuation, Ray& scattered) const override
	{
		attenuation = Color(1.0, 1.0, 1.0);
		double refractRatio = record.isFrontFace ? (1.0 / iOR) : iOR;

		Vector3 unitDirection = GetUnitVector(ray.GetDirection());		
		double cosTheta = fmin(Dot(-unitDirection, record.normal), 1.0);
		double sinTheta = sqrt(1.0 - cosTheta * cosTheta);

		bool canNotRefract = refractRatio * sinTheta > 1.0;
		Vector3 refracted;

		if (canNotRefract || Reflectance(cosTheta,refractRatio) > RandomDouble())
			refracted = Reflect(unitDirection, record.normal);
		else
			refracted = Refract(unitDirection, record.normal, refractRatio);

		scattered = Ray(record.point, refracted, ray.GetTime());

		return true;
	}

private:
	static double Reflectance(double cosine, double refRatio)
	{
		//Schlick's Approximation
		auto r0 = (1 - refRatio) / (1 + refRatio);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cosine), 5);
	}
};

class DiffuseLight : public Material
{
public:
	std::shared_ptr<Texture> emitTexture;

public:
	DiffuseLight(std::shared_ptr<Texture> _emitTexture) : emitTexture(_emitTexture) {}

	DiffuseLight(Color color) : emitTexture(std::make_shared<SolidColor>(color)) {}

	virtual bool Scatter(const Ray& ray, const HitRecord& record, Color& attenuation, Ray& scattered) const override
	{
		return false;
	}

	virtual Color Emitted(double u, double v, const Point3& p) const override
	{
		return emitTexture->Value(u, v, p);
	}

};

class Isotropic : public Material
{
public:
	std::shared_ptr<Texture> albedo;

public:
	Isotropic(Color color) : albedo(std::make_shared<SolidColor>(color)) {}
	Isotropic(std::shared_ptr<Texture> _albedo) : albedo(_albedo) {}

	virtual bool Scatter(const Ray& ray, const HitRecord& record, Color& attenuation, Ray& scattered) const override
	{
		scattered = Ray(record.point, RandominUnitSphere(), ray.time);
		attenuation = albedo->Value(record.u, record.v, record.point);
		return true;
	}
};