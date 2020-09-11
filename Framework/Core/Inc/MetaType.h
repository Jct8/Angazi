#pragma once

namespace Angazi::Core::Meta
{
	class MetaArray;
	class MetaClass;
	class MetaPointer;

	class MetaType
	{
	public:
		using DeserializeFunc = std::function<void(void* instance, const rapidjson::Value& jsonValue)>;

		enum class Category
		{
			Primitive,
			Class,
			Array,
			Pointer
		};

		MetaType(Category category, const char* name, size_t size, DeserializeFunc deserialize = nullptr);

		const MetaArray* AsMetaArray() const;
		const MetaClass* AsMetaClass() const;
		const MetaPointer* AsMetaPointer() const;

		Category GetCategory() const { return mCategory; }
		const char* GetName() const { return mName.c_str(); }
		size_t GetSize() const { return mSize; }

		virtual void Deserialize(void* instance, const rapidjson::Value& jsonValue) const;

	private:
		const Category mCategory;
		const std::string mName;
		const size_t mSize;
		const DeserializeFunc mDeserialize;
	};
}