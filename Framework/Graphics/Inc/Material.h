#pragma once

#include "Colors.h"

namespace Angazi::Graphics
{
	struct Material
	{
		META_CLASS_DECLARE;
		float padding[1]{};
		float power = 100.0f;
		Color ambient{ 1.0f };
		Color diffuse{ 1.0f };
		Color specular{ 0.75f,0.75f,0.75f,1.0f };
	};
}
