#pragma once
#include <Angazi/Inc/Angazi.h>

class TileMap
{
public:
	void Load();
	void Unload();

	void Update(float deltaTime);
	void Render() const;

private:

	int GetIndex(int x, int y) const;

	std::array<Angazi::TextureId, 6> mTextureIds;
	std::vector<int> mTiles;
	int mColumns = 0;
	int mRows = 0;
	int mTileSize = 32;

	int mCurrentIndex = 0;
};