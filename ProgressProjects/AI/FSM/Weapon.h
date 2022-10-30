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

	Angazi::Graphics::TextureId GetSurvivorTextureId() { return mSurvivorTextureId; }
	Angazi::Graphics::TextureId GetCrosshairTextureId() { return mCrosshairTextureId; }

	float GetSurvivorWitdh() const { return Angazi::Graphics::TextureManager::Get()->GetTexture(mSurvivorTextureId)->GetWidth() * 0.5f; };

protected:
	Angazi::Graphics::TextureId mSurvivorTextureId;
	Angazi::Graphics::TextureId mCrosshairTextureId;
	Angazi::Graphics::TextureId mTextureId;
	int mAmmoCount = 0;
	int mDamage = 0;
	float mFireDelay = 0;
	float mReloadDelay = 0;
	bool isReloading = false;
};
