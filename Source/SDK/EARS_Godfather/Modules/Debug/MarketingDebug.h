#pragma once

// SDK
#include "SDK/EARS_Framework/Core/Camera/Camera.h"

namespace EARS::Modules
{
	class MarketingCamera : public EARS::Framework::Camera
	{
	public:

		//~ Begin EARS::Framework::CameraInfo Interface
		virtual void Init(uint32_t PlayerID, bool bResetCamera) override;
		virtual void Update(float DeltaTime) override;
		//~ End EARS::Framework::CameraInfo Interface

		MarketingCamera(const EARS::Framework::CameraInfo& InCameraInfo);
	};

	class MarketingCameraInfo : public EARS::Framework::CameraInfo
	{
	public:

		MarketingCameraInfo();

		//~ Begin EARS::Framework::CameraInfo Interface
		virtual EARS::Framework::Camera* Create() override;
		//~ End EARS::Framework::CameraInfo Interface

	};
}