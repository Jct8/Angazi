#pragma once

namespace Angazi::Graphics
{
	using MeshId = size_t;

	class MeshManager
	{
	public:
		static void StaticInitialize(const std::filesystem::path& root);
		static void StaticTerminate();
		static MeshManager* Get();
	public:
		MeshManager() = default;
		~MeshManager();

		MeshManager(const MeshManager&) = delete;
		MeshManager& operator=(const MeshManager) = delete;

		void SetRootPath(const std::filesystem::path& path);

		MeshId LoadMesh(const std::filesystem::path& filePath, bool useRootPath = true);
		MeshBuffer* GetModel(MeshId id);

		void Clear();

	private:
		void LoadPrimitiveMesh(const std::string& type);

		std::filesystem::path mRootPath;
		std::unordered_map <MeshId, std::unique_ptr<MeshBuffer>> mInventory;
		std::unordered_map<std::string, MeshId> mPrimitiveMeshes;
	};
}