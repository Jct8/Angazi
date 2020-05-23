#pragma once
#include "Particle.h"

namespace Angazi::Physics
{
	class PhysicsWorld
	{
	public :
		struct Settings
		{
			Math::Vector3 gravity{ 0.0f,-9.8f,0.0f };
			float timeStep = 1.0f / 60.0f;
			float drag = 0.0f;
			int iterations = 1;
		};

		void Initialize(const Settings& settings);

		void Update(float deltaTime);
		void DebugDraw() const;

		void AddParticles(Particle* p);
		void Clear();

	private:
		void AccumulateForces();
		void Integrate();
		void SatisfyConstraints();

		std::vector<Particle*> mParticles;

		Settings mSettings;
		float mTimer = 0.0f;
	};
}