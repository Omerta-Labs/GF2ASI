#pragma once

#include "SDK/EARS_Common/SafePtr.h"
#include "SDK/EARS_Framework/Core/Base/Base.h"

// TODO: Move this elsewhere eventually
namespace RWS
{
	class CSystemCommands
	{
	public:

		virtual ~CSystemCommands() {}

	private:
	};

	static_assert(sizeof(CSystemCommands) == 0x4, "RWS::CSystemCommands must equal 0x4");
}

namespace EARS::Framework
{
	struct iAcceptStateChanges
	{
	public:

		virtual void ClearState(const uint32_t* State) = 0;
		virtual void SetState(const uint32_t* State) = 0;
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

	class Camera;

	class CameraInfo : public EARS::Framework::Base, public RWS::CSystemCommands
	{
	public:

		CameraInfo();
		virtual ~CameraInfo();

		//~ Begin RWS::CAttributeHandler / EARS::Framework::Base overrides
		virtual void HandleAttributes(const RWS::CAttributePacket& InPacket) override;
		virtual bool QueryInterface(const uint32_t ClassID, void** OutObjectPtr) const override;
		//~ End overrides

		virtual bool IsCameraModifier() const;
		virtual EARS::Framework::Camera* Create();

	protected:

		// Only used by marketing camera, we can leave as protected for now
		void SetIsNonInterruptable(bool bIsNonInterruptable) { m_bIsNonInterruptable = bIsNonInterruptable; }

	private:

		bool m_bFreeOnPop = false;
		bool m_bIsNonInterruptable = false;
	};

	static_assert(sizeof(CameraInfo) == 0x58, "EARS::Framework::CameraInfo must equal 0x58");

	class Camera : public SafeObj, public iAcceptStateChanges
	{
	public:

		Camera(const EARS::Framework::CameraInfo& InCameraInfo);

		//~ Begin iAcceptStateChanges Interface
		virtual void ClearState(const uint32_t* State) override { /* no default implementation */ }
		virtual void SetState(const uint32_t* State) override { /* no default implementation */ }
		//~ End iAcceptStateChanges Interface

		virtual void Init(uint32_t PlayerID, bool bResetCamera) { /* nothing by default */ }

		virtual void Update(float DeltaTime) { /* nothing by default */ }

		virtual void HandleCollision(EARS::Framework::CameraData* Data, EARS::Framework::CameraInterpType InterpType, float Weight) { /* nothing by default */ }

		virtual uint32_t GetCameraID() const { return 0x514EB665; }

		// operator overloads
		void* operator new(size_t size);
		void operator delete(void* pointer, size_t size);

	protected:

		SafePtr<EARS::Framework::CameraInfo> m_CameraInfo;
		__declspec(align(16)) EARS::Framework::CameraData m_CameraData; // TODO: Validate

	};
}
