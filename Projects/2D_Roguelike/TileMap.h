#pragma once
#include "Camera2D.h"
#include <Angazi/Inc/Angazi.h>

class TileMap
{
public:

	static void StaticInitialize();
	static void StaticTerminate();
	static TileMap& Get();
public:
	void LoadMap(std::filesystem::path fileName, std::filesystem::path graphFileName);
	void LoadTexture(std::filesystem::path fileName);
	void NewMap();
	void Unload();
	void ShowEditor();
	void MapResize();

	void SaveMap(std::filesystem::path fileName);
	void SetShowEditor(bool show) { mShowEditor = show; }

	void Update(float deltaTime);
	void Render();

	std::vector<Angazi::Math::Vector2> GetSpawnLocation(char c);
	float GetTileSize() { return mTileSize; };

	Angazi::Math::Rect GetBound() const;
	bool CheckCollision(const Angazi::Math::LineSegment & lineSegment) const;

	Angazi::AI::Path GetPath(Angazi::Math::Vector2 start, Angazi::Math::Vector2 end);
	float GetWorldWidth() { return mColumns * mTileSize; };
	float GetWorldHeight() { return mRows * mTileSize; };

private:
	int GetIndex(int col, int row) const;
	int GetIndex(int col, int row, int maxColumns) const;

	std::unique_ptr<int[]> mMap;
	std::unique_ptr<char[]> mMapSpawn;
	std::unique_ptr<char[]> mMapNodes;

	Angazi::AI::AStar mAStar;
	Angazi::AI::Path path;
	Angazi::AI::Graph mGraph;

	int mColumns = 0;
	int mRows = 0;
	int rowsTemp;
	int columnsTemp;
	int mCurrentTile = 1;
	float mTileSize = 32.0f;
	
	bool mShowEditor = false;
	bool mEditTile = false;
	
	std::deque<Angazi::Graphics::Texture> mTiles;
};