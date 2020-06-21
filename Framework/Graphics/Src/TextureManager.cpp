#include "Precompiled.h"
#include "TextureManager.h"

using namespace Angazi::Graphics;

namespace
{
	std::unique_ptr<TextureManager> sTextureManager;
}

void TextureManager::StaticInitialize(const std::filesystem::path& root)
{
	ASSERT(sTextureManager == nullptr, "[TextureManager] TextureManager already initialized!");
	sTextureManager = std::make_unique<TextureManager>();
	sTextureManager->SetRootPath(root);
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
	ASSERT(sTextureManager != nullptr, "[TextureManager] No instance registered.");
	return sTextureManager.get();
}

TextureManager::~TextureManager()
{
	ASSERT(mInventory.empty(), "[TextureManager] Clear() must be called to clean up.");
}

void TextureManager::SetRootPath(const std::filesystem::path& path)
{
	mRootPath = path.string();
}

TextureId TextureManager::Load(const char * fileName)
{
	std::string texturePath = mRootPath + "/" + fileName;

	std::hash<std::string> hasher;
	TextureId hash = hasher(texturePath);

	auto result = mInventory.insert({ hash , nullptr });
	if (result.second)
	{
		std::unique_ptr<Texture> texture = std::make_unique<Texture>();
		texture->Initialize(texturePath);
		result.first->second = std::move(texture);
	}

	return hash;
}

void TextureManager::Clear()
{
	for (auto& item : mInventory)
	{
		if (item.second)
		{
			item.second->Terminate();
			item.second.reset();
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
	return iter != mInventory.end() ? iter->second.get() : nullptr;
}