#pragma once

#include "Service.h"

namespace Angazi
{
	class CameraService : public Service
	{
	public:
		META_CLASS_DECLARE;

		void Initialize() override;
		void ShowInspectorProperties() override;

		Graphics::Camera* AddCamera(const char* name);
		Graphics::Camera* FindCamera(const char* name);

		void SetActiveCamera(const char* name);

		Graphics::Camera& GetActiveCamera();
		const Graphics::Camera& GetActiveCamera() const;

	private:
		using CameraMap = std::map<std::string, Graphics::Camera>;
		CameraMap mCameraMap;
		Graphics::Camera* mActiveCamera = nullptr;
	};
}