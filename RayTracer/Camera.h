#pragma once

#include "Vector3.h"
#include "Ray.h"
#include "Essentials.h"

class Camera
{
private:
	Point3 origin;
	Point3 lowerLeftCorner;
	Vector3 horizontal;
	Vector3 vertical;
	Vector3 u, v, w;
	double lensRadius;
	double time0, time1;
public:
	Camera(Point3 lookfrom, Point3 lookAt, Vector3 upVector, double verticalFOV, double aspectRatio,
		   double aperture, double focusDist, double _time0 = 0, double _time1 = 0)
	{
		auto theta = DegreeToRadian(verticalFOV);
		auto h = tan(theta / 2);
		auto viewportHeight = 2.0 * h;
		auto viewportWidth = aspectRatio * viewportHeight;
		auto focalLength = 1.0;

		w = GetUnitVector(lookfrom - lookAt);
		u = GetUnitVector(Cross(upVector, w));
		v = Cross(w, u);

		origin = lookfrom;
		horizontal = u * viewportWidth * focusDist;
		vertical = v * viewportHeight * focusDist;
		lowerLeftCorner = origin - horizontal / 2 - vertical / 2 - w * focusDist;
	
		lensRadius = aperture / 2;

		time0 = _time0;
		time1 = _time1;
	}

	Ray GetRay(double s, double t) const
	{
		Vector3 rd = RandominUnitDisk() * lensRadius;
		Vector3 offset = u * rd.GetX() + v * rd.GetY();

		return Ray(origin + offset, lowerLeftCorner + horizontal * s + vertical * t - origin - offset,RandomDouble(time0,time1));
	}
};