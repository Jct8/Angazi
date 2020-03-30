#include "Precompiled.h"
#include "PerceptionModule.h"

using namespace Angazi::AI;

PerceptionModule::PerceptionModule(Agent & agent, ImportanceCalculator importanceCalculator)
	:mAgent(agent),mImportanceCalculator(std::move(importanceCalculator))
{

}

void PerceptionModule::Update(float deltaTime)
{
	// Update sensors and refresh/add memory records
	for (auto&[name, sensor] : mSensors)
		sensor->Update(mAgent, mMemory, deltaTime);

	// Remove any records older than memory span
	for (auto iter = mMemory.begin(); iter != mMemory.end(); )
	{
		if (iter->lastRecordTime + mMemorySpan < Core::Timer::Get()->GetTotalTime())
			iter = mMemory.erase(iter);
		else
			++iter;
	}

	// Calculate importance on new records
	for (auto& m : mMemory)
		mImportanceCalculator(mAgent, m);

	// Sort records by imporance
	mMemory.sort([](const MemoryRecord& a, const MemoryRecord& b)
	{
		return a.importance > b.importance;
	});

}