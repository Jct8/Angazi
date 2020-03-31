#include "TileMap.h"
#include <ImGui/Inc/imgui.h>

using namespace Angazi;
using namespace Angazi::Graphics;

namespace
{
	std::unique_ptr<TileMap> sInstance;
}

void TileMap::StaticInitialize()
{
	ASSERT(sInstance == nullptr, "TileMap already initialized");
	sInstance = std::make_unique<TileMap>();
}

void TileMap::StaticTerminate()
{
	sInstance->Unload();
	sInstance.reset();
}

TileMap& TileMap::Get()
{
	ASSERT(sInstance != nullptr, "No TileMap created!");
	return *sInstance;
}

void TileMap::LoadMap(std::filesystem::path fileName, std::filesystem::path graphFileName)
{
	FILE* file = nullptr;
	fopen_s(&file, fileName.u8string().c_str(), "r");

	fscanf_s(file, "Columns: %d\n", &mColumns);
	fscanf_s(file, "Rows: %d\n", &mRows);

	mMap = std::make_unique<int[]>(mColumns*mRows);

	for (int y = 0; y < mRows; ++y)
	{
		for (int x = 0; x < mColumns; ++x)
		{
			int i = GetIndex(x, y);
			mMap[i] = fgetc(file) - '0';
		}
		fgetc(file);
	}
	fscanf_s(file, "\n");
	mMapSpawn = std::make_unique<char[]>(mColumns*mRows);

	for (int y = 0; y < mRows; ++y)
	{
		for (int x = 0; x < mColumns; ++x)
		{
			int i = GetIndex(x, y);
			mMapSpawn[i] = fgetc(file);
		}
		fgetc(file);
	}
	fclose(file);

	//Graph nodes
	fopen_s(&file, graphFileName.u8string().c_str(), "r");
	fscanf_s(file, "Columns: %d\n", &mColumns);
	fscanf_s(file, "Rows: %d\n", &mRows);
	mMapNodes = std::make_unique<char[]>(mColumns*mRows);
	for (int y = 0; y < mRows; ++y)
	{
		for (int x = 0; x < mColumns; ++x)
		{
			int i = GetIndex(x, y);
			mMapNodes[i] = fgetc(file);
		}
		fgetc(file);
	}
	fclose(file);

	mGraph.Resize(mColumns, mRows);

}

void TileMap::LoadTexture(std::filesystem::path fileName)
{
	FILE* file = nullptr;
	fopen_s(&file, fileName.u8string().c_str(), "r");

	int count = 0;
	fscanf_s(file, "Count: %d\n", &count);

	mTiles.resize(count);
	for (int i = 0; i < count; i++)
	{
		char buffer[256];
		fscanf_s(file, "%s\n", buffer, static_cast<int>(std::size(buffer)));
		mTiles[i].Initialize("../../Assets/Images/Rougelike/" + std::string(buffer));
	}

	fclose(file);

}

void TileMap::NewMap()
{
	mColumns = 10;
	mRows = 10;
	mMap = std::make_unique<int[]>(mColumns*mRows);

	for (int y = 0; y < mRows; ++y)
	{
		for (int x = 0; x < mColumns; ++x)
		{
			int i = GetIndex(x, y);
			mMap[i] = 0;
		}
	}

}

void TileMap::Unload()
{
	for (int i = 0; i < mTiles.size(); i++)
	{
		mTiles[i].Terminate();
	}
	mTiles.clear();

	mColumns = 0;
	mRows = 0;

	mMap.reset();
}

void TileMap::ShowEditor()
{
	if (!mShowEditor)
	{
		return;
	}

	ImGui::Begin("Settings Editor", nullptr);

	ImGui::BeginGroup();
	if (ImGui::CollapsingHeader("Map:"))
	{

		if (ImGui::Button("New"))
		{
			NewMap();
		}
		ImGui::SameLine();
		if (ImGui::Button("Open"))
		{
			char filePath[MAX_PATH]{};
			const char* title = "Open Map";
			const char* filter = "Text Files (*.txt)\0*.txt";
			if (MainApp().OpenFileDialog(filePath, title, filter))
			{
				LoadMap(filePath, "path.txt");
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Save"))
		{
			char filePath[MAX_PATH]{};
			const char* title = "Save Map";
			const char* filter = "Text Files (*.txt)\0*.txt";
			if (MainApp().SaveFileDialog(filePath, title, filter))
			{
				std::filesystem::path savePath = filePath;
				if (savePath.extension().empty())
					savePath += ".txt";
				SaveMap(savePath);
			}
		}
		ImGui::Separator();

		if (ImGui::Checkbox("Allow Editing ", &mEditTile))
		{

		}

		columnsTemp = mColumns;
		rowsTemp = mRows;

		ImGui::PushID("Different");
		ImGui::Text("Columns %d ", mColumns);
		ImGui::SameLine();
		if (ImGui::Button("-"))
		{
			columnsTemp--;
			MapResize();
		}
		ImGui::SameLine();
		if (ImGui::Button("+"))
		{
			columnsTemp++;
			MapResize();

		}
		ImGui::PopID();

		ImGui::Text("Rows %d ", mRows);
		ImGui::SameLine();
		if (ImGui::Button("-"))
		{
			rowsTemp--;
			MapResize();
		}
		ImGui::SameLine();
		if (ImGui::Button("+"))
		{
			rowsTemp++;
			MapResize();
		}


		if (ImGui::SliderInt("Columns", &columnsTemp, 1, 100))
		{
			MapResize();
		}
		if (ImGui::SliderInt("Rows", &rowsTemp, 1, 100))
		{
			MapResize();
		}

		ImGui::Separator();

		for (int i = 0; i < (int)mTiles.size(); ++i)
		{
			if (ImGui::ImageButton(mTiles[i].GetShaderResourceView(), { 32.0f,32.0f }))
			{
				mCurrentTile = i;
			}
			if (i % 6 != 2)
				ImGui::SameLine();
		}

	}
	ImGui::EndGroup();
	ImGui::End();

}

void TileMap::MapResize()
{
	if (columnsTemp < 0)
		columnsTemp = 0;
	if (rowsTemp < 0)
		rowsTemp = 0;

	int newColumnCount;
	columnsTemp > mColumns ? newColumnCount = mColumns : newColumnCount = columnsTemp;

	int newRowCount;
	rowsTemp > mRows ? newRowCount = mRows : newRowCount = rowsTemp;

	std::unique_ptr<int[]> tempMap = std::make_unique<int[]>(columnsTemp*rowsTemp);
	for (int y = 0; y < newRowCount; ++y)
	{
		for (int x = 0; x < newColumnCount; ++x)
		{
			int i = x + (y*mColumns);
			int j = x + (y*columnsTemp);
			tempMap[j] = mMap[i];
		}
	}

	mMap = std::move(tempMap);
	mColumns = columnsTemp;
	mRows = rowsTemp;
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
			fprintf_s(file, "%c", mMap[i] + '0');
		}
		fprintf_s(file, "\n");
	}

	fclose(file);
}

void TileMap::Update(float deltaTime)
{
	auto input = Input::InputSystem::Get();
	if (input->IsMouseDown(Input::MouseButton::LBUTTON) && mEditTile)
	{
		Math::Vector2 mousePos = { static_cast<float> (input->GetMouseScreenX()), static_cast<float>(input->GetMouseScreenY()) };


		Math::Vector2 worldPos = Camera2D::Get().ConvertToWorldPosition(mousePos);
		int x = static_cast<int>(worldPos.x / 32.0f);
		int y = static_cast<int>(worldPos.y / 32.0f);

		if (x >= 0 && x < mColumns && y >= 0 && y < mRows)
		{
			int i = GetIndex(x, y);
			mMap[i] = mCurrentTile;
		}
	}
}

AI::Path TileMap::GetPath(Math::Vector2 start, Math::Vector2 end)
{
	auto isBlocked = [this](AI::Coord coord) {return mMapNodes[GetIndex(coord.x, coord.y)] != '?'; };
	auto getCost = [this](AI::Coord to, AI::Coord from)
	{
		float cost = 1.0f;
		if (to.y > from.y)
		{
			cost = 1.2f;
		}
		return cost;
	};
	auto getHeuristicEuclid = [this](AI::Coord to, AI::Coord from)
	{
		return Math::Distance({ static_cast<float>(to.x),static_cast<float>(to.y) }
		, { static_cast<float>(from.x),static_cast<float>(from.y) });
	};

	auto getHeuristicDiagonal = [this](AI::Coord to, AI::Coord from)
	{
		return abs(static_cast<float>(from.x) - static_cast<float>(to.x))
			+ abs(static_cast<float>(from.y) - static_cast<float>(to.y));
	};

	auto getHeuristicManhattan = [this](AI::Coord to, AI::Coord from)
	{
		return Math::Max(
			abs(static_cast<float>(from.x) - static_cast<float>(to.x))
			, abs(static_cast<float>(from.y) - static_cast<float>(to.y)));
	};

	std::function<float(AI::Coord, AI::Coord)> getHeuristic = getHeuristicEuclid;

	AI::Coord startPoint = { static_cast<int>(start.x), static_cast<int>(start.y) };
	AI::Coord endPoint = { static_cast<int>(end.x), static_cast<int>(end.y) };
	path = mAStar.Search(mGraph, startPoint, endPoint, isBlocked, getCost, getHeuristic);

	return path;

}

void TileMap::Render()
{
	float offset = mTileSize * 0.5f;
	float circleRadius = mTileSize * 0.2f;

	auto graphics = GraphicsSystem::Get();
	for (int y = 0; y < mRows; ++y)
	{
		for (int x = 0; x < mColumns; ++x)
		{
			int i = GetIndex(x, y);
			Math::Vector2 worldPosOfTile = { x *32.0f , y * 32.0f };
			Math::Vector2 screenPos = Camera2D::Get().ConvertToScreenPosition(worldPosOfTile);
			if (screenPos.x >= -32.0f && screenPos.x < graphics->GetBackBufferWidth() && screenPos.y >= -32.0f && screenPos.y < graphics->GetBackBufferHeight())
			{
				SpriteRenderer::Get()->Draw(mTiles[mMap[i]], screenPos, 0.0f , Pivot::TopLeft);
			}
		}
	}

	int mouseX = Input::InputSystem::Get()->GetMouseScreenX();
	int mouseY = Input::InputSystem::Get()->GetMouseScreenY();


	if (mEditTile)
		SpriteRenderer::Get()->Draw(mTiles[mCurrentTile], Math::Vector2{ static_cast<float>(mouseX) , static_cast<float>(mouseY) }, 0.0f, Pivot::TopLeft);

}

std::vector<Math::Vector2> TileMap::GetSpawnLocation(char c)
{
	std::vector<Math::Vector2> locations;
	for (int y = 0; y < mRows; ++y)
	{
		for (int x = 0; x < mColumns; ++x)
		{
			int i = GetIndex(x, y);
			int index = GetIndex(x, y, mColumns);
			if (mMapSpawn[index] == c)
				locations.push_back({ static_cast<float>(x),static_cast<float>(y) });
		}
	}
	return locations;
}

Math::Rect TileMap::GetBound() const
{
	return {
	0.0f,
	0.0f,
	mColumns *32.0f,
	mRows*32.0f,
	};
}

bool TileMap::CheckCollision(const Math::LineSegment & lineSegment) const
{
	//int startX = static_cast<int>(floor(lineSegment.from.x / 32.0f));
	int startX = static_cast<int>(lineSegment.from.x / 32.0f);
	int startY = static_cast<int>(lineSegment.from.y / 32.0f);
	int endX = static_cast<int>(lineSegment.to.x / 32.0f);
	int endY = static_cast<int>(lineSegment.to.y / 32.0f);

	if (startX < 0 || startX >= mColumns ||
		startY < 0 || startY >= mRows ||
		endX < 0 || endX >= mColumns ||
		endY < 0 || endY >= mRows
		)
	{
		return true;
	}

	for (int y = startY; y <= endY; y++)
	{
		for (int x = startX; x <= endX; ++x)
		{
			int index = GetIndex(x, y, mColumns);
			if (mMap[index] > 2 && mMap[index] != 15)
				return true;
		}
	}
	return false;
}

int TileMap::GetIndex(int col, int row) const
{
	return col + (row*mColumns);
}

int TileMap::GetIndex(int col, int row, int maxColumns) const
{
	return col + (row*maxColumns);
}