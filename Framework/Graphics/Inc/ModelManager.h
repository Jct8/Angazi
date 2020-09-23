#pragma once

#include "Model.h"

namespace Angazi::Graphics
{
	using ModelId = size_t;

	class ModelManager
	{
	public:
		static void StaticInitialize(const std::filesystem::path& root);
		static void StaticTerminate();
		static ModelManager* Get();
	public:
		ModelManager() = default;
		~ModelManager();

		ModelManager(const ModelManager&) = delete;
		ModelManager& operator=(const ModelManager) = delete;

		void SetRootPath(const std::filesystem::path& path);

		ModelId LoadModel(const std::filesystem::path& filePath, bool useRootPath = true);
		Model* GetModel(ModelId id);

		void Clear();

	private:
		std::filesystem::path mRootPath;
		std::unordered_map <ModelId, std::unique_ptr<Model>> mInventory;
	};
}