#pragma once
#include "SpriteTypes.h"

namespace Angazi::Graphics
{
	class TextureManager
	{
	public:
		static void StaticInitialize(const std::filesystem::path& root);
		static void StaticTerminate();
		static TextureManager* Get();
	public:
		TextureManager() = default;
		~TextureManager();

		TextureManager(const TextureManager&) = delete;
		TextureManager& operator=(const TextureManager) = delete;

		void SetRootPath(const std::filesystem::path& path);

		TextureId Load(const char* fileName);
		void Clear();

		void BindVS(TextureId id, uint32_t slot = 0);
		void BindPS(TextureId id, uint32_t slot = 0);

		Texture* GetTexture(TextureId id);

	private:
		std::string mRootPath;
		std::unordered_map <TextureId, std::unique_ptr<Texture>> mInventory;
	};
}