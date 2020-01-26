#include "TileMap.h"
#include "ImGui/Inc/imgui.h"

using namespace Angazi;
using namespace Angazi::Input;
using namespace Angazi::Graphics;

void TileMap::Load()
{
	//mTextureIds[0] = Angazi::LoadTexture("grass.png");
	//mTextureIds[1] = Angazi::LoadTexture("flower.png");
	//mTextureIds[2] = Angazi::LoadTexture("tree0.png");
	//mTextureIds[3] = Angazi::LoadTexture("tree1.png");
	//mTextureIds[4] = Angazi::LoadTexture("tree2.png");
	//mTextureIds[5] = Angazi::LoadTexture("tree3.png");

	mColumns = 10;
	mRows = 10;
	mTiles.resize(mColumns*mRows, 0);
	
	mGraph.Resize(mColumns, mRows);
}

void TileMap::Unload()
{
}

void TileMap::Update(float deltaTime)
{
	auto inputSystem = InputSystem::Get();

	int column = inputSystem->GetMouseScreenX() / mTileSize;
	int row = inputSystem->GetMouseScreenY() / mTileSize;

	//Check bound and make sure we are within the map
	//Check if mouse is clicked
	//Index into mTiles and change value
	if (inputSystem->IsMousePressed(Input::MouseButton::LBUTTON))
	{
		if (mChooseStart)
		{
			mChooseStart = false;
			mStartPosition = { column,row };
			mUpdate = true;
		}
		else if (mChooseEnd)
		{
			mChooseEnd = false;
			mEndPosition = { column , row };
			mUpdate = true;
		}
		else if (row < mRows && column < mColumns)
		{
			const int index = GetIndex(column, row);
			//int tileIndex = (mTiles[index] + 1) % (mTextureIds.size());
			mTiles[index] = mCurrentTile;
		}
	}

	if (mUpdate)
	{
		switch (mSearchType)
		{
		case 0:
			path = mDFS.Search(mGraph, mStartPosition, mEndPosition, [this](AI::Coord coord) {return mTiles[mGraph.GetIndex(coord)] > 1; });
			closedList = mDFS.GetClosedList();
			parentList = mDFS.GetParents();
			break;
		case 1:
			path = mBFS.Search(mGraph, mStartPosition, mEndPosition, [this](AI::Coord coord) {return mTiles[mGraph.GetIndex(coord)] != 0; });
			closedList = mBFS.GetClosedList();
			parentList = mBFS.GetParents();
			break;
		default:
			break;
		}
		mUpdate = false;
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
			//SimpleDraw::AddScreenCircle({ pos.x + offset , pos.y + offset }, circleRadius, Colors::AliceBlue);
			std::vector<AI::Coord> coords = mGraph.GetNode({ x,y })->neighbors;
			for (int i = 0; i < coords.size(); i++)
			{
				//SimpleDraw::AddScreenLine(pos.x + offset, pos.y + offset, coords[i].x*mTileSize + offset, coords[i].y*mTileSize + offset, Colors::AliceBlue);
			}
		}
	}

	SimpleDraw::AddScreenCircle({ mStartPosition.x*mTileSize + offset , mStartPosition.y*mTileSize + offset }, circleRadius, Colors::LightGray);
	SimpleDraw::AddScreenCircle({ mEndPosition.x*mTileSize + offset , mEndPosition.y*mTileSize + offset }, circleRadius, Colors::Red);

	if (!closedList.empty())
	{
		for (auto coord : closedList)
		{
			int parentIndex = mGraph.GetIndex(coord);
			if (parentList[parentIndex].IsValid())
			{
				SimpleDraw::AddScreenLine(coord.x*mTileSize + offset, coord.y*mTileSize + offset
					, parentList[parentIndex].x*mTileSize + offset, parentList[parentIndex].y*mTileSize + offset, Colors::AliceBlue);
			}
		}
	}

	if (!path.empty())
	{
		for (int i = 0; i < path.size() - 1; i++)
		{
			SimpleDraw::AddScreenLine(path[i].x*mTileSize + offset, path[i].y*mTileSize + offset
				, path[i + 1].x*mTileSize + offset, path[i + 1].y*mTileSize + offset, Colors::DarkRed);
		}
	}
}

void TileMap::DebugUI()
{
	ImGui::SetNextWindowSize({ 300,400 });
	ImGui::Begin("Options", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::BeginGroup();
	if (ImGui::CollapsingHeader("Map:", ImGuiTreeNodeFlags_DefaultOpen))
	{
		/*for (int i = 0; i < (int)mTextureIds.size(); ++i)
		{
			if (ImGui::ImageButton(X::GetSprite(mTextureIds[i]), { 32.0f,32.0f }))
			{
				mCurrentTile = i;
			}
			if (i % 6 != 2)
				ImGui::SameLine();
		}*/
		ImGui::SameLine();
		/*if (ImGui::Button("Load"))
		{
			char filePath[MAX_PATH]{};
			const char* title = "Open Map";
			const char* filter = "Text Files (*.txt)\0*.txt";
			if (X::OpenFileDialog(filePath, title, filter))
			{
				LoadMap(filePath);
			}
		}*/
		ImGui::SameLine();
		/*if (ImGui::Button("Save"))
		{
			char filePath[MAX_PATH]{};
			const char* title = "Save Map";
			const char* filter = "Text Files (*.txt)\0*.txt";
			if (X::SaveFileDialog(filePath, title, filter))
			{
				std::filesystem::path savePath = filePath;
				if (savePath.extension().empty())
					savePath += ".txt";
				SaveMap(savePath);
			}
		}*/
	}
	ImGui::EndGroup();
	ImGui::BeginGroup();
	if (ImGui::CollapsingHeader("Search Type:", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::RadioButton("DFS:", &mSearchType, 0))
		{
			mSearchType = 0;
			mUpdate = true;
		}
		if (ImGui::RadioButton("BFS:", &mSearchType, 1))
		{
			mSearchType = 1;
			mUpdate = true;
		}
	}
	ImGui::EndGroup();

	ImGui::BeginGroup();
	if (ImGui::CollapsingHeader("Move Position:", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::Button("Place Start Position"))
		{
			mChooseStart = true;
		}
		if (ImGui::Button("Place End Position"))
		{
			mChooseEnd = true;
		}
		if (ImGui::Button("Update"))
		{
			mUpdate = true;
		}
	}
	ImGui::EndGroup();

	ImGui::End();
}

int TileMap::GetIndex(int x, int y) const
{
	return x + (y*mColumns);
}

void TileMap::LoadMap(std::filesystem::path fileName)
{
	FILE* file = nullptr;
	fopen_s(&file, fileName.u8string().c_str(), "r");

	fscanf_s(file, "Columns: %d\n", &mColumns);
	fscanf_s(file, "Rows: %d\n", &mRows);

	for (int y = 0; y < mRows; ++y)
	{
		for (int x = 0; x < mColumns; ++x)
		{
			int i = GetIndex(x, y);
			mTiles[i] = fgetc(file) - '0';
		}
		fgetc(file);
	}

	fclose(file);
}

void TileMap::SaveMap(std::filesystem::path fileName)
{
	FILE* file = nullptr;
	fopen_s(&file, fileName.u8string().c_str(), "w");

	fprintf_s(file, "Columns: %d\n", mColumns);
	fprintf_s(file, "Rows: %d\n", mRows);

	for (int y = 0; y < mRows; ++y)
	{
		for (int x = 0; x < mColumns; ++x)
		{
			int i = GetIndex(x, y);
			fprintf_s(file, "%c", mTiles[i] + '0');
		}
		fprintf_s(file, "\n");
	}

	fclose(file);
}