#pragma once
#include <Angazi/Inc/Angazi.h>

class Weapon
{
public:
	virtual ~Weapon() = default;

	virtual void Load() = 0;
	virtual void Unload() = 0;

	virtual void Render() = 0;

	virtual void Fire(const Angazi::Math::Vector2 &position, const Angazi::Math::Vector2 &direction) = 0;
	virtual bool CanFire() = 0;
	virtual void Reload() = 0;

	Angazi::Graphics::Texture* GetSurvivorTextureId() { return &mSurvivorTextureId; }
	Angazi::Graphics::Texture* GetCrosshairTextureId() { return &mCrosshairTextureId; }

	float GetSurvivorWitdh() const { return mSurvivorTextureId.GetWidth() * 0.5f; };

protected:
	Angazi::Graphics::Texture mSurvivorTextureId;
	Angazi::Graphics::Texture mCrosshairTextureId;
	Angazi::Graphics::Texture mTextureId;
	int mAmmoCount = 0;
	int mDamage = 0;
	float mFireDelay = 0;
	float mReloadDelay = 0;
	bool isReloading = false;
};
