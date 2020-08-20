#pragma once

#include <random>

inline auto& RandomEngine()
{
	static std::random_device myRandomDevice{};
	static std::mt19937 myRandomEngine{ myRandomDevice() };
	return myRandomEngine;
}

inline int RandomInt(int min, int max)
{
	return std::uniform_int_distribution<>{ min, max }(RandomEngine());
}

inline double RandomReal()
{
	return std::uniform_real_distribution<>{ 0, 1 }(RandomEngine());
}
