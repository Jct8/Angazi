#include "Precompiled.h"
#include "MeshManager.h"

#include "ObjLoader.h"

using namespace Angazi::Graphics;

namespace
{
	std::unique_ptr<MeshManager> sMeshManager;
}

void MeshManager::StaticInitialize(const std::filesystem::path& root)
{
	ASSERT(sMeshManager == nullptr, "MeshManager -- ModelManager already initialized");
	sMeshManager = std::make_unique<MeshManager>();
	sMeshManager->mRootPath = root;
}

void MeshManager::StaticTerminate()
{
	if (sMeshManager != nullptr)
	{
		sMeshManager->Clear();
		sMeshManager.reset();
	}
}

MeshManager* MeshManager::Get()
{
	ASSERT(sMeshManager != nullptr, "MeshManager -- No instance registered.");
	return sMeshManager.get();
}

MeshManager::~MeshManager()
{
	ASSERT(mInventory.empty(), "MeshManager -- Clear() must be called to clean up.");
}

void MeshManager::SetRootPath(const std::filesystem::path& path)
{
	mRootPath = path;
}

MeshId MeshManager::LoadMesh(const std::filesystem::path& filePath, bool useRootPath)
{
	std::filesystem::path modelPath = mRootPath;
	if (useRootPath)
		modelPath.append(filePath.c_str());
	else
		modelPath = filePath;

	std::hash<std::string> hasher;
	TextureId hash = hasher(modelPath.string());

	auto [iter, successful] = mInventory.insert({ hash , nullptr });
	if (successful)
	{
		std::unique_ptr<MeshBuffer> meshBuffer = std::make_unique<MeshBuffer>();
		Mesh mesh;
		ObjLoader::Load(modelPath,1.0f,mesh);
		meshBuffer->Initialize(mesh);
		iter->second = std::move(meshBuffer);
	}
	return hash;
}

MeshBuffer* MeshManager::GetModel(MeshId id)
{
	auto iter = mInventory.find(id);
	return iter != mInventory.end() ? iter->second.get() : nullptr;
}

void MeshManager::Clear()
{
	for (auto& [id, model] : mInventory)
	{
		if (model)
		{
			model->Terminate();
			model.reset();
		}
	}
	mInventory.clear();
}
