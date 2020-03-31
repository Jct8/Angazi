#pragma once
#include <Angazi/Inc/Angazi.h>

class SightSensor : public Angazi::AI::Sensor
{
public:
	float viewRange = 100.0f;
	float neighborhoodRadius = 100.0f;
	uint32_t enemyID = 1;

	void Update(Angazi::AI::Agent& agent, Angazi::AI::MemoryRecords& memory, float deltaTime) override;
};
