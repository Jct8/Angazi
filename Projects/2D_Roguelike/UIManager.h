#pragma once
#include <Angazi/Inc/Angazi.h>

class UIManager
{
public:
	static void StaticInitialize();
	static void StaticTerminate();
	static UIManager& Get();

	void Load();
	void Unload();

	void Update(float deltaTime);
	void Render();
private:
	Angazi::Graphics::TextureId barEmpty;
	Angazi::Graphics::TextureId barRed;

	float playerHealthPercent;
};

