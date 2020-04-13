#pragma once

namespace Angazi::Core::TimeUtil
{
	void Initialize();
	void Update();

	float GetTime();
	float GetDeltaTime();
	float GetFramesPerSecond();
}