#include "Precompiled.h"
#include "TimeUtil.h"
#include "DebugUtil.h"

using namespace Angazi::Core::TimeUtil;

namespace
{
	float mLastUpdateTime;
	float mFrameSinceLastSecond;
	float mFramesPerSecond;
}

void Angazi::Core::TimeUtil::Update()
{
	//Calculate the FPS
	mFrameSinceLastSecond += 1.0f;
	float totalTime = GetTime();
	if (totalTime >= mLastUpdateTime + 1.0f)
	{
		mFramesPerSecond = mFrameSinceLastSecond / (totalTime - mLastUpdateTime);
		mFrameSinceLastSecond = 0.0f;
		mLastUpdateTime = totalTime;
	}
}

float Angazi::Core::TimeUtil::GetTime()
{
	static const auto startTime = std::chrono::high_resolution_clock::now();
	auto currentTime = std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count() / 1000.0f;
}

float Angazi::Core::TimeUtil::GetDeltaTime()
{
	static auto lastTime = std::chrono::high_resolution_clock::now();
	auto currentTime = std::chrono::high_resolution_clock::now();
	auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count() / 1000.0f;
	lastTime = currentTime;
	return deltaTime;
}

float Angazi::Core::TimeUtil::GetFramesPerSecond()
{
	return mFramesPerSecond;
}
