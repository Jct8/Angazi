#include "Precompiled.h"
#include "TextureManager.h"

using namespace Angazi::Graphics;

namespace
{
	std::unique_ptr<TextureManager> sTextureManager;
}

void TextureManager::StaticInitialize(const std::filesystem::path& root)
{
	ASSERT(sTextureManager == nullptr, "TextureManager -- TextureManager already initialized!");
	sTextureManager = std::make_unique<TextureManager>();
	sTextureManager->SetRootPath(root);
	sTextureManager->defaultTexture = sTextureManager->Load("Empty.png", false, true);
}

void TextureManager::StaticTerminate()
{
	if (sTextureManager != nullptr)
	{
		sTextureManager->Clear();
		sTextureManager.reset();
	}
}

TextureManager * TextureManager::Get()
{
	ASSERT(sTextureManager != nullptr, "TextureManager -- No instance registered.");
	return sTextureManager.get();
}

TextureManager::~TextureManager()
{
	ASSERT(mInventory.empty(), "TextureManager -- Clear() must be called to clean up.");
}

void TextureManager::SetRootPath(const std::filesystem::path& path)
{
	mRootPath = path;
}

TextureId TextureManager::Load(const std::filesystem::path & filePath,bool useGammaCorrection, bool useRootPath)
{
	std::filesystem::path texturePath = mRootPath;
	if (useRootPath)
		texturePath.append(filePath.c_str());
	else
		texturePath = filePath;

	if (!std::filesystem::exists(texturePath))
		return 0;

	std::hash<std::string> hasher;
	TextureId hash = hasher(texturePath.string());

	auto [iter, successful] = mInventory.insert({ hash , nullptr });
	if (successful)
	{
		std::unique_ptr<Texture> texture = std::make_unique<Texture>();
		texture->Initialize(texturePath);
		iter->second = std::move(texture);
	}
	return hash;
}

void TextureManager::Clear()
{
	for (auto& [id,model] : mInventory)
	{
		if (model)
		{
			model->Terminate();
			model.reset();
		}
	}
	mInventory.clear();
}

void TextureManager::BindVS(TextureId id, uint32_t slot)
{
	auto iter = mInventory.find(id);
	if (iter != mInventory.end())
		iter->second->BindVS(slot);
}

void TextureManager::BindPS(TextureId id, uint32_t slot)
{
	auto iter = mInventory.find(id);
	if (iter != mInventory.end())
		iter->second->BindPS(slot);
}

Texture * TextureManager::GetTexture(TextureId id)
{
	auto iter = mInventory.find(id);
	if (iter != mInventory.end())
		return iter->second.get();
	auto defaultIter = mInventory.find(defaultTexture);
	return defaultIter != mInventory.end() ? defaultIter->second.get() : nullptr;
}