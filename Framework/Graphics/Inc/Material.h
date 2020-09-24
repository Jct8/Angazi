#pragma once

#include "Colors.h"

namespace Angazi::Graphics
{
	struct Material
	{
		Color ambient;
		Color diffuse;
		Color specular;
		float power = 0.0f;
		float padding[2]{};
		META_CLASS_DECLARE
	};
}
