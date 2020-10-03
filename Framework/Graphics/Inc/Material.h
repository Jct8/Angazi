#pragma once

#include "Colors.h"

namespace Angazi::Graphics
{
	struct Material
	{
		META_CLASS_DECLARE;
		float padding[1]{};
		float power = 0.0f;
		Color ambient;
		Color diffuse;
		Color specular;
	};
}
