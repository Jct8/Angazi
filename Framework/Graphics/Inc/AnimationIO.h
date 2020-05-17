#pragma once

#include "AnimationClip.h"

namespace Angazi::Graphics
{
	class AnimationIO
	{
	public:
		static void Write(FILE *file, const Animation& animation);
		static void Read(FILE *file, const Animation& animation);

		static void Write(FILE *file, const AnimationClip& animationClip);
		static void Read(FILE *file, const AnimationClip& animationClip);
	};
}