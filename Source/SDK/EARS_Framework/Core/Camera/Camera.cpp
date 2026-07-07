#include "Camera.h"

#include "Addons/Hook.h"

// SDK
#include "SDK/EARS_Godfather/System/Memory/GlobalHeapAllocator.h"

namespace EARS::Framework
{
	CameraInfo::CameraInfo()
		: Base()
		, m_bFreeOnPop(false)
		, m_bIsNonInterruptable(false)
	{
	}

	CameraInfo::~CameraInfo()
	{
		// default
	}

	void CameraInfo::HandleAttributes(const RWS::CAttributePacket& InPacket)
	{
		// EARS::Framework::CameraInfo::HandleAttributes
		MemUtils::CallClassMethod<void, CameraInfo*, const RWS::CAttributePacket&>(0x004738B0, this, InPacket);
	}

	bool CameraInfo::QueryInterface(const uint32_t ClassID, void** OutObjectPtr) const
	{
		if (ClassID != 0x66F692B3)
		{
			return EARS::Framework::Base::QueryInterface(ClassID, OutObjectPtr);
		}

		*OutObjectPtr = (void*)this;
		return true;
	}

	bool CameraInfo::IsCameraModifier() const
	{
		return false;
	}

	Camera::Camera(const EARS::Framework::CameraInfo& InCameraInfo)
	{
		
	}

	EARS::Framework::Camera* CameraInfo::Create()
	{
		assert(false);
		return nullptr;
	}

	void Camera::GetPosition(RwV3d& OutPosition) const
	{
		OutPosition.m_X = m_CameraData.m_Position[0];
		OutPosition.m_Y = m_CameraData.m_Position[1];
		OutPosition.m_Z = m_CameraData.m_Position[2];
	}

	void Camera::SetPosition(const RwV3d& InPosition)
	{
		m_CameraData.m_Position[0] = InPosition.m_X;
		m_CameraData.m_Position[1] = InPosition.m_Y;
		m_CameraData.m_Position[2] = InPosition.m_Z;
	}

	void Camera::GetRotation(RwV3d& OutRotation) const
	{
		OutRotation.m_X = m_CameraData.m_Rotation[0];
		OutRotation.m_Y = m_CameraData.m_Rotation[1];
		OutRotation.m_Z = m_CameraData.m_Rotation[2];
	}

	void Camera::SetRotation(const RwV3d& InRotation)
	{
		m_CameraData.m_Rotation[0] = InRotation.m_X;
		m_CameraData.m_Rotation[1] = InRotation.m_Y;
		m_CameraData.m_Rotation[2] = InRotation.m_Z;
	}

	void* Camera::operator new(size_t size)
	{
		EA::Allocator::IAllocator* Allocator = get_thread_new_allocator();
		EA::TagValuePair Pair(2, 16);
		return Allocator->Alloc(size, Pair);
	}

	void Camera::operator delete(void* pointer, size_t size)
	{
		EA::Allocator::IAllocator* Allocator = get_thread_new_allocator();
		Allocator->Free(pointer, size);
	}
}
