#pragma once

#include "Colors.h"

namespace Angazi::GraphicsGL
{
	struct Material
	{
		Color ambient;
		Color diffuse;
		Color specular;
		float power;
		float padding[3];
	};
}