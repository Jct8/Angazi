#pragma once

#include "Mesh.h"
#include "MeshBuffer.h"
#include "Skeleton.h"

namespace Angazi::Graphics
{
	struct Material;
	class Model;
	class Texture;

	class ModelLoader
	{
	public:
		static void LoadModel(std::filesystem::path fileName, Model & model);
		static void LoadSkeleton(std::filesystem::path fileName, Skeleton & model);
	};

	class Model
	{
	public:
		void Initialize(const std::filesystem::path & fileName);
		void Terminate();

		void Draw() const;
	public:
		struct MeshData
		{
			SkinnedMesh mesh;
			uint32_t materialIndex = 0;
			MeshBuffer meshBuffer;
		};

		struct MaterialData
		{
			Material material;
			std::string diffuseMapName;
			std::unique_ptr<Texture> diffuseMap;
		};

		std::vector<MeshData> meshData;
		std::vector<MaterialData> materialData;
		Skeleton skeleton;
	};
}