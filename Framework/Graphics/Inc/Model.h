#pragma once

#include "Mesh.h"
#include "MeshBuffer.h"
#include "Material.h"
#include "Skeleton.h"
#include "AnimationSet.h"

namespace Angazi::Graphics
{
	class Model;
	class Texture;
	class Effect;

	class ModelLoader
	{
	public:
		static void LoadModel(std::filesystem::path fileName, Model & model);
		static void LoadSkeleton(std::filesystem::path fileName, Skeleton & skeleton);
		static void LoadAnimationSet(std::filesystem::path fileName, AnimationSet & animationSet);
	};

	class Model
	{
	public:
		void Initialize(const std::filesystem::path & fileName);
		void Terminate();

		void Draw(Effect* effect ) const;
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
			std::string normalMapName;
			std::unique_ptr<Texture> diffuseMap;
			std::unique_ptr<Texture> normalMap;
		};

		std::vector<MeshData> meshData;
		std::vector<MaterialData> materialData;
		Skeleton skeleton;
		AnimationSet animationSet;
	};
}