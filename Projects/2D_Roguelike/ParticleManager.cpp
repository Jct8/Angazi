#include "ParticleManager.h"

using namespace Angazi;

namespace
{
	std::unique_ptr<ParticleManager> sInstance = nullptr;
	const int defaultSize = 50;
}

void ParticleManager::StaticInitialize()
{
	ASSERT(sInstance == nullptr, "ParticleManager already initialized");
	sInstance = std::make_unique<ParticleManager>();
}

void ParticleManager::StaticTerminate()
{
	sInstance->Unload();
	sInstance.reset();
}

ParticleManager & ParticleManager::Get()
{
	ASSERT(sInstance != nullptr, "No ParticleManager created!");
	return *sInstance;
}

void ParticleManager::Load()
{
	for (int i = 0; i < defaultSize; i++)
	{
		mParticles["BloodMelee"].emplace_back();
		mParticles["BloodMelee"][i].Load("../../Assets/Images/Rougelike/bloodSplat.png", 3, 3);
	}
	for (int i = 0; i < defaultSize; i++)
	{
		mParticles["LightningSheet"].emplace_back();
		mParticles["LightningSheet"][i].Load("../../Assets/Images/Rougelike/LightningSheet.png", 1, 8);
	}
	for (int i = 0; i < defaultSize; i++)
	{
		mParticles["particleSheet"].emplace_back();
		mParticles["particleSheet"][i].Load("../../Assets/Images/Rougelike/particleSheet.png", 8, 5);
	}

}

void ParticleManager::Unload()
{
	for (auto& particleType : mParticles)
		for (auto& particle : particleType.second)
			particle.Unload();
	mParticles.clear();
}

void ParticleManager::Update(float deltaTime)
{
	std::map<std::string, std::deque<Particle>>::iterator iter = mParticles.begin();
	for (; iter != mParticles.end(); iter++)
	{
		for (size_t i = 0; i < iter->second.size(); i++)
		{
			if (iter->second[i].IsActive())
			{
				iter->second[i].Update(deltaTime);
			}
		}
	}

}

void ParticleManager::Render()
{
	std::map<std::string, std::deque<Particle>>::iterator iter = mParticles.begin();
	for (; iter != mParticles.end(); iter++)
	{
		for (size_t i = 0; i < iter->second.size(); i++)
		{
			if (iter->second[i].IsActive())
			{
				iter->second[i].Render();
			}
		}
	}
}

void ParticleManager::StartParticle(Math::Vector2 pos, std::string key, Graphics::Pivot pivot)
{
	for (size_t i = 0; i < mParticles[key].size(); i++)
	{
		if (!mParticles[key][i].IsActive())
		{
			mParticles[key][i].Start(pos);
			mParticles[key][i].SetPivot(pivot);
			break;
		}
	}
}
