#pragma once

#include <Angazi/Inc/Angazi.h>

class PipeManager;


class Bird
{
public:
	void Load();

	void Update(float deltaTime, const PipeManager& pm);
	void Render();

	void Flap();

	void Spawn(const Angazi::Math::Vector2& pos);
	void Kill();

	Angazi::Math::Circle GetBound() const { return { mPosition,mRadius }; }
	bool IsAlive() const { return mAlive; };

	std::unique_ptr<Angazi::AI::NEAT::NeuralNet> brain;
	float fitness = 0.0f;

private:
	std::array<Angazi::Graphics::TextureId, 5> mTextureIds;
	Angazi::Math::Vector2 mPosition;
	Angazi::Math::Vector2 mVelocity;
	float mRadius = 30.0f;
	float mAnimationFrame = 0.0f;
	bool mAlive = false;
};