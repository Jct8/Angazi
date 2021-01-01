#include "Precompiled.h"
#include "TerrainService.h"

#include "CameraService.h"
#include "LightService.h"
#include "GameWorld.h"
#include "ImGui/Inc/imgui.h"

using namespace Angazi;

META_DERIVED_BEGIN(TerrainService, Service)
	META_FIELD_BEGIN
		//META_FIELD(mRows, "Rows")
		//META_FIELD(mColumns, "Columns")
		META_FIELD(mCellSize, "CellSize")
	META_FIELD_END
	META_NO_METHOD
META_CLASS_END;

void TerrainService::Initialize()
{
	mCameraService = GetWorld().GetService<CameraService>();
	mLightService = GetWorld().GetService<LightService>();

	mTerrain.Initialize(mRows,mColumns, mCellSize);
	mTerrain.SetHeightScale(mHeightScale);
	mTerrain.LoadHeightmap(mHeighMapPath);
}

void TerrainService::Terminate()
{
	mTerrain.Terminate();
}

void TerrainService::Render()
{
	mTerrain.SetDirectionalLight(mLightService->GetActiveLight());
	mTerrain.Render(mCameraService->GetActiveCamera());
}

void TerrainService::ShowInspectorProperties()
{
	
}
