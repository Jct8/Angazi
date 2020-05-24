#include "Precompiled.h"
#include "PhysicsWorld.h"

using namespace Angazi;
using namespace Angazi::Physics;

void PhysicsWorld::Initialize(const Settings & settings)
{
	mSettings = settings;
}

void PhysicsWorld::Update(float deltaTime)
{
	mTimer += deltaTime;
	while (mTimer >= mSettings.timeStep)
	{
		mTimer -= mSettings.timeStep;

		AccumulateForces();
		Integrate();
		SatisfyConstraints();
	}
}

void PhysicsWorld::DebugDraw() const
{
	for (auto p : mParticles)
		Graphics::SimpleDraw::AddSphere(p->position, p->radius, Graphics::Colors::AliceBlue, 2, 4);
}

void PhysicsWorld::AddParticles(Particle * p)
{
	mParticles.push_back(p);
}

void PhysicsWorld::Clear()
{
	for (auto particle : mParticles)
		delete particle;
	mParticles.clear();
}

void PhysicsWorld::AccumulateForces()
{
	for (auto p : mParticles)
		p->acceleration = mSettings.gravity;
}

void PhysicsWorld::Integrate()
{
	const float timeStepSqr = Math::Sqr(mSettings.timeStep);
	for (auto p : mParticles)
	{
		Math::Vector3 displacement = (p->position - p->lastPosition) + (p->acceleration * timeStepSqr);
		p->lastPosition = p->position;
		p->position = p->position + displacement;
	}
}

void PhysicsWorld::SatisfyConstraints()
{
	// TODO
}
