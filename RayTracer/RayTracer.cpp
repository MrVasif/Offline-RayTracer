#include <iostream>
#include "Color.h"
#include "Ray.h"
#include "Essentials.h"
#include "HittableList.h"
#include "Sphere.h"
#include "Camera.h"
#include "Material.h"
#include "rectangle.h"
#include "bvh.h"
#include "Box.h"
#include "medium.h"
double RaySphereHitTest(const Point3& center, double radius, const Ray& r)
{ 
	// Replace b = 2h.
	Vector3 dist = r.GetOrigin() - center;
	auto a = r.GetDirection().GetLengthSquared();
	auto halfb = Dot(dist, r.GetDirection());
	auto c = dist.GetLengthSquared() - radius * radius;
	auto discriminant = halfb * halfb - a * c;
	if (discriminant < 0)
		return -1.0;
	else
		return (-halfb - std::sqrt(discriminant)) / (2.0 * a);
}

Color RayColor(const Ray& r, const Hittable& world, int depth, const Color& backgroundColor)
{
	HitRecord record;
	
	if (depth <= 0)
		return Color(0, 0, 0);

	if (!world.Hit(r, 0.001, infinity, record))
	{
		return backgroundColor;
	}

	Ray scattered;
	Color attenuation;
	Color emitted = record.materialPtr->Emitted(record.u, record.v, record.point);

	if (!record.materialPtr->Scatter(r, record, attenuation, scattered))
		return emitted;

	return emitted + attenuation * RayColor(scattered, world, depth - 1,backgroundColor);
	/*
	Vector3 unitDirection = GetUnitVector(r.GetDirection());
	auto t = 0.5 * (unitDirection.GetY() + 1.0);
	return  Color(1.0, 1.0, 1.0) * (1.0 - t) + Color(0.5, 0.7, 1.0) * t;
	*/
}

int main()
{
	const auto aspectRatio = 1;
	const int image_width = 800;
	const int image_height = static_cast<int>(image_width / aspectRatio);
	const int sampleCountPerPixel = 5000;
	const int maxDepth = 50;
	HittableList world;

	auto greenTone = std::make_shared<Lambertian>(Color(0.48, 0.83, 0.53));

	HittableList boxes;
	

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			double w = 100;
			//world.AddObject(std::make_shared<Box>(Point3(0, -100, 0), Point3(100 + w * i, 100, 100 + w * j), std::make_shared<Lambertian>(Color(1, 0, 0))));
		}
	}
	//world.AddObject(std::make_shared<BVHnode>(boxes, 0, 1));
	//world.AddObject(std::make_shared<Box>(Point3(0, -100, 0), Point3(165, 130, 165), std::make_shared<Lambertian>(Color(1,0,0))));
	//world.AddObject(std::make_shared<Sphere>(Point3(360, 150, 145), 70, std::make_shared<Dielectric>(1.5)));
	
	//auto light = std::make_shared<DiffuseLight>(Color(7, 7, 7));
	//world.AddObject(std::make_shared<RectangleXZ>(123, 423, 147, 412, 554, light));

	
	auto red = std::make_shared<Lambertian>(Color(0.65, 0.05, 0.05));
	auto white = std::make_shared<Lambertian>(Color(0.73, 0.73, 0.73));
	auto green = std::make_shared<Lambertian>(Color(0.12, 0.45, 0.15));
	auto light = std::make_shared<DiffuseLight>(Color(15, 15, 15));

	world.AddObject(std::make_shared<RectangleYZ>(0, 555, 0, 555, 555, green));
	world.AddObject(std::make_shared<RectangleYZ>(0, 555, 0, 555, 0, red));
	world.AddObject(std::make_shared<RectangleXZ>(213, 343, 227, 332, 554, light));
	world.AddObject(std::make_shared<RectangleXZ>(0, 555, 0, 555, 0, white));
	world.AddObject(std::make_shared<RectangleXZ>(0, 555, 0, 555, 555, white));
	world.AddObject(std::make_shared<RectangleXY>(0, 555, 0, 555, 555, white));

	std::shared_ptr<Hittable> box1 = std::make_shared<Box>(Point3(0, 0, 0), Point3(165, 330, 165), white);
	box1 = std::make_shared<YRotated>(box1, 15);
	box1 = std::make_shared<Translated>(box1, Vector3(265, 0, 295));
	world.AddObject(box1);
	

	//-----------


	//std::shared_ptr<Hittable> box2 = std::make_shared<Box>(Point3(0, 0, 0), Point3(165, 165, 165), white);
	//box2 = std::make_shared<YRotated>(box2, -18);
	//box2 = std::make_shared<Translated>(box1, Vector3(130, 0, 65));
	//world.AddObject(box2);
	/*
	for (int x = -15; x < 15; x++)
	{
		for (int z = -15; z < 15; z++)
		{
			auto randomMaterial = RandomDouble();

			Point3 center(x + 0.9 * RandomDouble(), 0.2, z + 0.9 * RandomDouble());

			if ((center - Point3(4, 0.2, 0)).GetLength() > 0.9)
			{
				std::shared_ptr<Material> sphereMaterial;
				
				if (randomMaterial < 0.8)
				{
					auto albedo = Color::Random() * Color::Random();
					sphereMaterial = std::make_shared<Lambertian>(albedo);
					auto center2 = center + Vector3(0, RandomDouble(0, 0.5), 0);
					world.AddObject(std::make_shared<MovingSphere>(center, center2, 0.0, 1.0, 0.2, sphereMaterial));
				}
				else if (randomMaterial < 0.95)
				{
					auto albedo = Color::Random(0.5, 1);
					auto fuzz = RandomDouble(0, 0.5);
					sphereMaterial = std::make_shared<Metal>(albedo, fuzz);
					world.AddObject(std::make_shared<Sphere>(center, 0.2, sphereMaterial));
				}
				else
				{
					sphereMaterial = std::make_shared<Dielectric>(1.5);
					world.AddObject(std::make_shared<Sphere>(center, 0.2, sphereMaterial));
				}
				
			}
		}
	}
	*/


	Point3 lookFrom(478, 178, -1400);
	Point3 lookAt(278, 278, 0);
	Vector3 upVector(0, 1, 0);
	auto distanceToFocus = 10.0;
	auto aperture = 0.0;
	Camera cam(lookFrom,lookAt,upVector,40,aspectRatio,aperture,distanceToFocus,0.0,1.0);
	const Color backgroundColor = Color(0.0, 0.0, 0.0);
	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for (int j = image_height - 1; j >= 0; --j)
	{
		for (int i = 0; i < image_width; ++i)
		{
			Color pixelColor(0, 0, 0);
			for (int s = 0; s < sampleCountPerPixel; ++s)
			{
				auto u = double(i + RandomDouble()) / (image_width - 1);
				auto v = double(j + RandomDouble()) / (image_height - 1);
				Ray r = cam.GetRay(u, v);
				pixelColor += RayColor(r, world,maxDepth,backgroundColor);
			}
			WriteToColor(std::cout, pixelColor,sampleCountPerPixel);
		}
	}

	std::cerr << "\nDone!\n";
}

