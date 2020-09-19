#pragma once

#include <Angazi/Inc/Angazi.h>

class Asteroid
{
public:
	enum Type
	{
		Small,
		Medium,
		Large,
		End
	};

	void Load();
	void Update(float deltaTime);
	void Render();

	void Spawn(Angazi::Math::Vector2 position, Type type);

	Angazi::Math::Circle GetBoundingCircle() const { return { mPosition,mRadius }; }

	void SetActive(bool active) { isActive = active; }
	bool IsActive() const { return isActive; }

	Angazi::Math::Vector2 GetPosition() const { return mPosition; }
	float GetRadius() const { return mRadius; }
	Type GetType() {return mType; }

private:
	Angazi::Math::Vector2 mPosition;
	Angazi::Math::Vector2 mVelocity;

	Type mType;
	float mRadius;
	bool isActive = false;
};