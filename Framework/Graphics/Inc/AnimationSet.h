#pragma once

#include "AnimationClip.h"

namespace Angazi::Graphics
{
	struct AnimationSet
	{
		std::vector<std::unique_ptr<AnimationClip>> clips;
	};
}