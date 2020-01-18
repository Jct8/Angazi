#pragma once
#include <Angazi/Inc/Angazi.h>

class TileMap
{
public:
	void Load();
	void Unload();

	void Update(float deltaTime);
	void Render();

private:
	int GetIndex(int x, int y) const;

	AI::Graph mGraph; // Homework: draw circle and line to neighbors
	std::array<X::TextureId, 6> mTextureIds;
	std::vector<int> mTiles;
	int mColumns = 0;
	int mRows = 0;
	int mTileSize = 32;

	int mCurrentIndex = 0;
};