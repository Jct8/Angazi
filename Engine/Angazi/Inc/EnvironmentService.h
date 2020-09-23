#pragma once

#include "Service.h"

namespace Angazi
{
	class EnvironmentService : public Service
	{
	public:
		META_CLASS_DECLARE;

		void Terminate() override;
		
		void Render() override;
		void ShowInspectorProperties() override;

		Graphics::Skybox* AddEnvironment(const char* name);
		Graphics::Skybox* FindEnvironment(const char* name);

		void SetActiveEnvironment(const char* name);

		Graphics::Skybox& GetActiveEnvironment();
		const Graphics::Skybox& GetActiveEnvironment() const;

	private:
		using EnvironmentMap = std::map<std::string, Graphics::Skybox>;
		EnvironmentMap mEnvironmentMap;
		Graphics::Skybox* mActiveSkybox = nullptr;
	};
}