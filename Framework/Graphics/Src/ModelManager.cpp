#include "Precompiled.h"
#include "ModelManager.h"

using namespace Angazi::Graphics;

namespace
{
	std::unique_ptr<ModelManager> sModelManager;
}

void ModelManager::StaticInitialize(const std::filesystem::path& root)
{
	ASSERT(sModelManager == nullptr, "ModelManager -- ModelManager already initialized");
	sModelManager = std::make_unique<ModelManager>();
	sModelManager->mRootPath = root;
}

void ModelManager::StaticTerminate()
{
	if (sModelManager != nullptr)
	{
		sModelManager->Clear();
		sModelManager.reset();
	}
}

ModelManager* ModelManager::Get()
{
	ASSERT(sModelManager != nullptr, "ModelManager -- No instance registered.");
	return sModelManager.get();
}

ModelManager::~ModelManager()
{
	ASSERT(mInventory.empty(), "ModelManager -- Clear() must be called to clean up.");
}

void ModelManager::SetRootPath(const std::filesystem::path& path)
{
	mRootPath = path;
}

ModelId ModelManager::LoadModel(const std::filesystem::path& filePath, bool useRootPath)
{
	std::filesystem::path modelPath = mRootPath;
	if (useRootPath)
		modelPath.append(filePath.c_str());
	else
		modelPath = filePath;

	std::hash<std::string> hasher;
	TextureId hash = hasher(modelPath.string());

	auto [iter,successful] = mInventory.insert({ hash , nullptr });
	if (successful)
	{
		std::unique_ptr<Model> model = std::make_unique<Model>();
		model->Initialize(modelPath);
		iter->second = std::move(model);
	}
	return hash;
}

Model* ModelManager::GetModel(ModelId id)
{
	auto iter = mInventory.find(id);
	return iter != mInventory.end() ? iter->second.get() : nullptr;
}

void ModelManager::Clear()
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
