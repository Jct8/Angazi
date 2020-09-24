#pragma once

#include "Service.h"

namespace Angazi
{
	class LightService : public Service
	{
	public:
		META_CLASS_DECLARE;


		void Initialize() override;
		void ShowInspectorProperties() override;

		Graphics::DirectionalLight* AddLight(const char* name);
		Graphics::DirectionalLight* FindLight(const char* name);

		void SetActiveLight(const char* name);

		Graphics::DirectionalLight& GetActiveLight();
		const Graphics::DirectionalLight& GetActiveLight() const;

	private:
		using LightMap = std::map<std::string, Graphics::DirectionalLight>;
		LightMap mLightMap;
		Graphics::DirectionalLight* mActiveLight = nullptr;
	};
}