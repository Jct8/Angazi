#pragma once
#include <Angazi/Inc/Angazi.h>

class Weapon
{
public:
	virtual ~Weapon() = default;

	virtual void Load(std::filesystem::path fileName) = 0;
	virtual void Unload() = 0;
	virtual void Render(int mFrame, Angazi::Math::Vector2 screenPos, bool isFacingLeft) = 0;
	virtual void Attack(int mFrame, Angazi::Math::Vector2 position, bool isFacingLeft, bool isPlayer) = 0;
	virtual int GetFrameCount() { return static_cast<int>(mAnimations.size()); }

	void SetDealtDamage(bool value) { dealtDamage = value; }
	bool GetDealthDamage() { return dealtDamage; }
	Angazi::Graphics::TextureId GetSprite() { return mSprite; }

	virtual bool IsMelee() const = 0;

protected:
	int mDamage = 0;
	float mSpeed = 0;
	bool dealtDamage = false;

	std::deque<Angazi::Graphics::TextureId> mAnimations;
	std::vector<Angazi::Math::Vector2> mAnimationDamage;
	Angazi::Graphics::TextureId mSprite;
};
