#pragma once

#include "Essentials.h"
#include "Vector3.h"
class Perlin
{
private:
	static const int pointCount = 256;
	Vector3* randomVectors;
	int* permX;
	int* permY;
	int* permZ;

	static void Permute(int* p, int n)
	{
		for (int i = n - 1; i > 0; i--)
		{
			int target = RandomInt(0, i);
			int tmpVar = p[i];
			p[i] = p[target];
			p[target] = tmpVar;
		}
	}
	static int* perlinGeneratePerm()
	{
		auto p = new int[pointCount];

		for (int i = 0; i < Perlin::pointCount; i++)
		{
			p[i] = i;
		}

		Permute(p, pointCount);

		return p;
	}

	static double PerlinInterpolation(Vector3 c[2][2][2], double u, double v, double w)
	{
		auto uu = u * u * (3 - 2 * u);
		auto vv = v * v * (3 - 2 * v);
		auto ww = w * w * (3 - 2 * w);

		auto accum = 0.0;

		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				for (int k = 0; k < 2; k++)
				{
					Vector3 weightVector(u - i, v - j, w - k);
					accum += (i * uu + (1 - i) * (1 - uu)) *
						(j * vv + (1 - j) * (1 - vv)) *
						(k * ww + (1 - k) * (1 - ww)) *
						Dot(c[i][j][k], weightVector);
				}
			}
		}

		return accum;
	}

public:
	Perlin()
	{
		randomVectors = new Vector3[pointCount];
		for (int i = 0; i < pointCount; i++)
		{
			randomVectors[i] = GetUnitVector(Vector3::Random(-1, 1));
		}

		permX = perlinGeneratePerm();
		permY = perlinGeneratePerm();
		permZ = perlinGeneratePerm();
	}

	double Noise(const Point3& p) const
	{
		/*
		auto i = static_cast<int>(4 * p.GetX()) & 255;
		auto j = static_cast<int>(4 * p.GetY()) & 255;
		auto k = static_cast<int>(4 * p.GetZ()) & 255;

		return randomFloat[permX[i] ^ permY[j] ^ permZ[k]];
		*/

		auto u = p.GetX() - floor(p.GetX());
		auto v = p.GetY() - floor(p.GetY());
		auto w = p.GetZ() - floor(p.GetZ());

		auto i = static_cast<int>(floor(p.GetX()));
		auto j = static_cast<int>(floor(p.GetY()));
		auto k = static_cast<int>(floor(p.GetZ()));

		Vector3 c[2][2][2];

		for (int di = 0; di < 2; di++)
		{
			for (int dj = 0; dj < 2; dj++)
			{
				for (int dk = 0; dk < 2; dk++)
				{
					c[di][dj][dk] = randomVectors[
						permX[(i + di) & 255] ^
						permY[(j + dj) & 255] ^
					    permZ[(k + dk) & 255]
					];
				}
			}
		}

		return PerlinInterpolation(c, u, v, w);
	}

	double Turbulence(const Point3& p, int depth = 7) const
	{
		auto accum = 0.0;
		auto tempP = p;
		auto weight = 1.0;

		for (int i = 0; i < depth; i++)
		{
			accum += weight * Noise(tempP);
			weight *= 0.5;
			tempP *= 2;
		}

		return fabs(accum);
	}

	~Perlin()
	{
		delete[] randomVectors;
		delete[] permX;
		delete[] permY;
		delete[] permZ;
	}
};