#pragma once

namespace Angazi::Core::Meta
{
	class MetaType;

	class MetaFunction
	{
	public:
		MetaFunction(const char* name, const MetaType* type, std::vector<const MetaType*> paramTypes);

		const char* GetName() const { return mName.c_str(); }
		const MetaType* GetReturnType() const { return mReturnType; }

		const MetaType* GetParameterType(size_t index) const { return mParameterTypes[index]; }
		size_t GetParameterCount() const { return mParameterTypes.size(); }

		template<class... Params>
		constexpr auto Invoke(void* instance,Params... params)
		{
			return std::invoke(mfunction, instance, params);
		}

	private:

		const std::string mName;
		const MetaType* mReturnType;
		const std::vector<const MetaType*> mParameterTypes;

		std::function<MetaType()> mfunction;
	};
}