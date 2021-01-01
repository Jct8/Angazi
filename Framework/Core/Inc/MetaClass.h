#pragma once

#include "MetaType.h"

namespace Angazi::Core::Meta
{
	class MetaField;
	class MetaFunction;

	class MetaClass :public MetaType
	{
	public:
		using CreateFunc = std::function<void*()>;

		MetaClass(const char* name, size_t size, const MetaClass* parent, std::vector<MetaField> fields, CreateFunc create, std::vector<MetaFunction> methods);

		const MetaClass* GetParent() const;
		const MetaField* FindField(const char* name) const;
		const MetaField* GetField(size_t index) const;
		size_t GetFieldCount() const;

		void* Create() const;
		void Deserialize(void* classInstance, const rapidjson::Value& jsonValue) const;
		void Serialize(const void* classInstance, rapidjson::Value& jsonValue, rapidjson::Document& document) const;

		const MetaFunction* FindMethod(const char* name) const;
		const MetaFunction* GetMethod(size_t index) const;
		size_t GetMethodCount() const;

	private:
		size_t GetParentFieldCount() const;
		size_t GetParentMethodCount() const;

		const MetaClass* mParent;
		const std::vector<MetaField> mFields;
		const std::vector<MetaFunction> mMethods;

		const CreateFunc mCreate;
	};
}