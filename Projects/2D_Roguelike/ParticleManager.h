#pragma once
#include "Particle.h"
#include <Angazi/Inc/Angazi.h>

class ParticleManager
{
public:
	static void StaticInitialize();
	static void StaticTerminate();
	static ParticleManager& Get();

	void Load();
	void Unload();
	void Update(float deltaTime);
	void Render();
	int GetParticleWidth(std::string name) { return mParticles[name][0].GetTextureWidth(); }

	void StartParticle(Angazi::Math::Vector2 pos, std::string key, Angazi::Graphics::Pivot pivot);

private:
	std::map<std::string,std::vector<Particle>> mParticles;
};

