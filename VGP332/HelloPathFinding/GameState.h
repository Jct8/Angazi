#pragma once
#include <Angazi/Inc/Angazi.h>
#include "TileMap.h"

class GameState : public Angazi::AppState
{
public:
	void Initialize() override;
	void Terminate() override;

	void Update(float deltaTime) override;
	void Render() override;
	
	void DebugUI() override;

private:
	Angazi::Graphics::Camera mCamera;
	Angazi::AI::Graph mGraph;

	TileMap tileMap;
};
