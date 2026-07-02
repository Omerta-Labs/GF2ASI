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
