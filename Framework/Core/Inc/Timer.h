#pragma once

namespace Angazi::Core
{
	class Timer
	{
	public:
		Timer();

		void Initialize();
		void Update();

		float GetElapsedTime() const;
		float GetTotalTime() const;
		float GetFramesPerSecond() const;

	private:
		LARGE_INTEGER mTicksPerSecond;
		LARGE_INTEGER mLastTick;
		LARGE_INTEGER mCurrentTick;

		float mElapsedTime;
		float mTotalTime;

		float mLastUpdateTime;
		float mFrameSinceLastSecond;
		float mFramesPerSecond;
	};
}