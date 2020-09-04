#pragma once

namespace Angazi::Core::Meta
{
	class MetaArray;
	class MetaClass;
	class MetaPointer;

	class MetaType
	{
	public:
		enum class Category
		{
			Primitive,
			Class,
			Array,
			Pointer
		};

		MetaType(Category category, const char* name, size_t size);

		const MetaArray* AsMetaArray() const;
		const MetaClass* AsMetaClass() const;
		const MetaPointer* AsMetaPointer() const;

		Category GetCategory() const { return mCategory; }
		const char* GetName() const { return mName.c_str(); }
		size_t GetSize() const { return mSize; }

	private:
		const Category mCategory;
		const std::string mName;
		const size_t mSize;
	};
}