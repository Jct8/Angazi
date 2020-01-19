#pragma once

#include "Colors.h"

namespace Angazi::Graphics
{
	// This needs to be 16 byte aligned to be HLSL compliant
	struct DirectionalLight
	{
		// <-------16--------->
		// [direction][padding]
		// [    ambient       ]
		// [    diffuse       ]
		// [   specular       ]
		Math::Vector3 direction;
		float padding;
		Color ambient;
		Color diffuse;
		Color specular;
	};
}