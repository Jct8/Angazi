#pragma once
#include <AI.h>

class SightSensor : public AI::Sensor
{
public:
	float viewRange = 100.0f;
	float neighborhoodRadius = 100.0f;
	uint32_t enemyID = 1;

	void Update(AI::Agent& agent, AI::MemoryRecords& memory, float deltaTime) override;
};
