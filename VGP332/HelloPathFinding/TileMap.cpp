#include "TileMap.h"

using namespace Angazi;
using namespace Angazi::Input;
using namespace Angazi::Graphics;

void TileMap::Load()
{
	mTextureIds[0] = Angazi::LoadTexture("grass.png");
	mTextureIds[1] = Angazi::LoadTexture("flower.png");
	mTextureIds[2] = Angazi::LoadTexture("tree0.png");
	mTextureIds[3] = Angazi::LoadTexture("tree1.png");
	mTextureIds[4] = Angazi::LoadTexture("tree2.png");
	mTextureIds[5] = Angazi::LoadTexture("tree3.png");

	mColumns = 10;
	mRows = 10;
	mTiles.resize(mColumns*mRows, 0);
}

void TileMap::Unload()
{
}

void TileMap::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();

	int column = inputSystem->GetMouseMoveX() / mTileSize;
	int row = inputSystem->GetMouseMoveY() / mTileSize;

	//Check bound and make sure we are within the map
	//Check if mouse is clicked
	//Index into mTiles and change value
	if (inputSystem->IsMousePressed(Input::MouseButton::LBUTTON))
	{
		if (row < mRows && column < mColumns)
		{
			const int index = GetIndex(column, row);
			int tileIndex = (mTiles[index] + 1) % (mTextureIds.size());
			mTiles[index] = tileIndex;
		}
	}
}

void TileMap::Render() 
{
	float offset = mTileSize * 0.5f;
	float circleRadius = mTileSize * 0.2f;
	for (int y = 0; y < mRows; y++)
	{
		for (int x = 0; x < mColumns; x++)
		{
			const int index = GetIndex(x, y);
			Angazi::Math::Vector2 pos
			{
				static_cast<float>(x)*32.0f,
				static_cast<float>(y)*32.0f
			};
			//X::DrawSprite(mTextureIds[mTiles[index]], { pos.x , pos.y }, X::Pivot::TopLeft);
			//X::DrawScreenCircle({ pos.x + offset , pos.y + offset }, circleRadius, X::Colors::AliceBlue);
			std::vector<AI::Coord> coords = mGraph.GetNode({ x,y })->neighbors;
			for (int i = 0; i < coords.size(); i++)
			{
				SimpleDraw::AddScreenLine(pos.x + offset, pos.y + offset, coords[i].x*mTileSize + offset, coords[i].y*mTileSize + offset, Colors::AliceBlue);
			}
		}
	}
}

int TileMap::GetIndex(int x, int y) const
{
	return x + (y*mColumns);
}
