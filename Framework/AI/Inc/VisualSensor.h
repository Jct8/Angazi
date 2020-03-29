#pragma once
#include "Sensor.h"

namespace Angazi::AI
{
	class VisualSensor : public Sensor
	{
	public:
		float viewRange = 100.0f;
		float viewAngle = 90.0f;
		float neighborhoodRadius = 100.0f;

		void Update(Agent& agent, MemoryRecords& memory, float deltaTime) override;
	};
}