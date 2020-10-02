#pragma once

#include "Terrain.h"
#include "Service.h"

namespace Angazi
{
	class CameraService;
	class LightService;

	class TerrainService : public Service
	{
	public:
		META_CLASS_DECLARE;

		void Initialize() override;
		void Terminate() override;

		void Render() override;
		void ShowInspectorProperties() override;

	private:
		const CameraService* mCameraService = nullptr;
		const LightService* mLightService = nullptr;

		Terrain mTerrain;
		uint32_t mRows = 200;
		uint32_t mColumns = 200;
		std::filesystem::path mHeighMapPath = "../../Assets/Heightmaps/heightmap_200x200.raw";

		float mHeightScale = 30.0f;
		float mCellSize = 1.0f;

	};
}