#pragma once
#include <Angazi/Inc/Angazi.h>

class RedState : public Angazi::AppState
{
public:
	void Initialize() override;
	void Update(float deltatTime) override;

};

class BlueState : public Angazi::AppState
{
public:
	void Initialize() override;
	void Update(float deltatTime) override;

};