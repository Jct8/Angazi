#pragma once
#include <Angazi/Inc/Angazi.h>

class Mineral : public Angazi::AI::Entity
{
public:
	Mineral(Angazi::AI::AIWorld& world);

	void Initialize();

	void Render();

	void Unseen() { seen = false; }
	void Seen() { seen = true; }

	void Collect() { collected = true; }
	bool IsCollected() const { return collected; }

private:
	Angazi::Graphics::TextureId mTextureId;
	Angazi::Graphics::TextureId mEyeTextureId;
	bool seen = false;
	bool collected = false;
};

