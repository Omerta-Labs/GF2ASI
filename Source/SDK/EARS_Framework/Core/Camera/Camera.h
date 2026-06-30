#pragma once

#include "SDK/EARS_Common/SafePtr.h"
#include "SDK/EARS_Framework/Core/Base/Base.h"

// TODO: Move this elsewhere eventually
namespace RWS
{
	class CSystemCommands
	{
	public:

		virtual ~CSystemCommands() = 0;

	private:

		// TODO: Unknown member. Present in the engine layout (CSystemCommands is vtable + 4 bytes = 0x8),
		// which pushes CameraInfo's derived members (e.g. CustomCameraInfo::m_Pos) to their correct offsets.
		uint32_t m_Unknown_CSystemCommands = 0;
	};

	static_assert(sizeof(CSystemCommands) == 0x8, "RWS::CSystemCommands must equal 0x8");
}

namespace EARS::Framework
{
	struct iAcceptStateChanges
	{
	public:

		virtual ~iAcceptStateChanges() = 0;
	};

	enum CameraAuthoredAspectRatio : uint32_t
	{
		CAMERA_AUTHORED_16x9 = 0x1,
		CAMERA_AUTHORED_4x3 = 0x2,
	};

	enum CameraAspectConversionType : uint32_t
	{
		CAMERA_DISPLAY_CONST_X = 0x1,
		CAMERA_DISPLAY_CONST_Y = 0x2,
	};

	enum CameraInterpType : uint32_t
	{
		CAMERA_INTERP_NONE = 0x0,
		CAMERA_INTERP_LINEAR = 0x1,
		CAMERA_INTERP_COS = 0x2,
		CAMERA_INTERP_RAMP = 0x3,
		CAMERA_INTERP_RADIAL = 0x4,
		CAMERA_INTERP_TAPERED_RADIAL = 0x5,
		CAMERA_INTERP_COS_RADIAL = 0x6,
		CAMERA_INTERP_COS_RADIAL_WITH_UPDATE = 0x7,
		CAMERA_INTERP_RAMP_WITHOUT_UPDATE = 0x8,
	};

	struct __declspec(align(8)) CameraData
	{
	public:

		float m_Position[4];
		float m_Anchor[4];
		float m_Rotation[4];
		float m_FieldOfView = 0.0f;
		EARS::Framework::CameraAuthoredAspectRatio m_AuthoredAspectRatio;
		EARS::Framework::CameraAspectConversionType m_AspectConversionType;
	};

	class CameraInfo : public EARS::Framework::Base, public RWS::CSystemCommands
	{

	};

	static_assert(sizeof(CameraInfo) == 0x58, "EARS::Framework::CameraInfo must equal 0x58");

	class Camera : public SafeObj, public iAcceptStateChanges
	{
	public:

		SafePtr<EARS::Framework::CameraInfo> m_CameraInfo;
		__declspec(align(16)) EARS::Framework::CameraData m_CameraData; // TODO: Validate

	};
}
