#pragma once
#include <Angazi/Inc/Angazi.h>

class TileMap
{
public:
	void Load();
	void Unload();

	void Update(float deltaTime);
	void Render();

	void DebugUI();
	void SaveMap(std::filesystem::path fileName);
	void LoadMap(std::filesystem::path fileName);

private:
	int GetIndex(int x, int y) const;

	Angazi::AI::Graph mGraph; 
	Angazi::AI::BFS mBFS;
	Angazi::AI::DFS mDFS;
	Angazi::AI::Dijkstras mDijkstras;
	Angazi::AI::AStar mAStar;
	Angazi::AI::Path path;

	std::list<Angazi::AI::Coord> closedList;
	std::vector< Angazi::AI::Coord> parentList;
	std::array<Angazi::Graphics::TextureId, 6> mTextureIds;
	std::vector<int> mTiles;

	Angazi::AI::Coord mStartPosition{ 2, 2 };
	Angazi::AI::Coord mEndPosition{ 11, 15 };

	int mColumns = 0;
	int mRows = 0;
	int mTileSize = 32;
	int mCurrentTile = 0;

	int mSearchType = 0;
	int mHeuristicType = 0;
	bool mChooseStart = false;
	bool mChooseEnd = false;
	bool mUpdate = true;
	bool mShowGraph = true;
	bool check = false;
};