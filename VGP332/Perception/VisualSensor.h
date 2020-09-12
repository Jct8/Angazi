#pragma once

#include <Angazi/Inc/Angazi.h>

class VisualSensor : public Angazi::AI::Sensor
{
public:
	void Update(Angazi::AI::Agent& agent, Angazi::AI::MemoryRecords& memory, float deltaTime) override;

	float viewRange = 0.0f;
	float viewAngle = 0.0f;
};