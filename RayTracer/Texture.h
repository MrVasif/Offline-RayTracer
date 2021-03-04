#pragma once

#include "Essentials.h"
#include "Vector3.h"
#include "perlin.h"
#include "stb_image_importer.h"

class Texture
{
public:
	virtual Color Value(double u, double v, const Point3& p) const = 0;
};

class SolidColor : public Texture
{
private:
	Color colorValue;

public:
	SolidColor() {}

	SolidColor(Color _colorValue) : colorValue(_colorValue) {}

	SolidColor(double r,double g,double b) : SolidColor(Color(r,g,b)) {}

	virtual Color Value(double u, double v, const Point3& p) const override
	{
		return colorValue;
	}
};

class CheckerTexture : public Texture
{
public:
	std::shared_ptr<Texture> odd;
	std::shared_ptr<Texture> even;

public:
	CheckerTexture() {}

	CheckerTexture(std::shared_ptr<Texture> _even, std::shared_ptr<Texture> _odd)
		: even(_even), odd(_odd) {}

	CheckerTexture(Color c1, Color c2)
		: even(std::make_shared<SolidColor>(c1)), odd(std::make_shared<SolidColor>(c2)) {}

	virtual Color Value(double u, double v, const Point3& p) const override
	{
		auto sines = sin(10 * p.GetX()) * sin(10 * p.GetY()) * sin(10 * p.GetZ());

		if (sines < 0)
			return odd->Value(u, v, p);
		else
			return even->Value(u, v, p);
	}

};

class NoiseTexture : public Texture
{
public:
	Perlin noise;
	double scale;
public:

	NoiseTexture(double _scale) : scale(_scale) {}

	virtual Color Value(double u, double v, const Point3& p) const override
	{
		//return Color(1, 1, 1) * 0.5 * (1.0 + noise.Noise(p * scale));
	
		return Color(1, 1, 1) * 0.5 * (1 + sin(scale * p.GetZ() + 10 * noise.Turbulence(p * scale)));
	}
};

class ImageTexture : public Texture
{
private:
	unsigned char* data;
	int width;
	int height;
	int bytesPerScanline;

public:
	const static int bytesPerPixel = 3;

	ImageTexture() : data(nullptr), width(0), height(0), bytesPerScanline(0) {}

	ImageTexture(const char* fileName)
	{
		auto componentsPerPixel = bytesPerPixel;

		data = stbi_load(fileName, &width, &height, &componentsPerPixel, componentsPerPixel);

		if (!data)
		{
			std::cerr << "ERROR: IMAGE COULDN'T LOAD!" << "'.\n";
			width = height = 0;
		}

		bytesPerScanline = bytesPerPixel * width;
	}

	virtual Color Value(double u, double v, const Point3& p) const override
	{
		if (data == nullptr)
			return Color(0, 0, 0);

		u = Clamp(u, 0.0, 1.0);
		v = 1.0 - Clamp(v, 0.0, 1.0);

		auto i = static_cast<int>(u * width);
		auto j = static_cast<int>(v * height);

		if (i >= width)
			i = width - 1;
		if (j >= height)
			j = height - 1;

		const auto colorScale = 1.0 / 255.0;
		auto pixel = data + j * bytesPerScanline + i * bytesPerPixel;

		return Color(colorScale * pixel[0], colorScale * pixel[1], colorScale * pixel[2]);
	}
};