#pragma once

#include "Vector3.h"
#include <iostream>
#include "Essentials.h"

void WriteToColor(std::ostream& out, Color pixelColor, int sampleCount)
{
	auto r = pixelColor.GetX();
	auto g = pixelColor.GetY();
	auto b = pixelColor.GetZ();

	auto scale = 1.0 / sampleCount;
	r = sqrt(scale * r);
	g = sqrt(scale * g);
	b = sqrt(scale * b);

	out << static_cast<int>(256 * Clamp(r, 0.0, 0.999)) << ' '
		<< static_cast<int>(256 * Clamp(g, 0.0, 0.999)) << ' '
		<< static_cast<int>(256 * Clamp(b, 0.0, 0.999)) << '\n';
}