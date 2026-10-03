#include "SDK/rwcontroller/include/rw/core/controller/rwcontroller.h"

namespace rw
{
	namespace core
	{
		namespace controller
		{
			namespace
			{
				// LLManager field offsets, reached through Manager::m_pLLManager.
				constexpr uint32_t kLLNumberDevices = 0x40;
				constexpr uint32_t kLLDeviceInfoTable = 0x44;
				constexpr uint32_t kLLDeviceStateTable = 0xC4;

				template <typename T>
				T ReadAt(const void* pBase, uint32_t Offset)
				{
					return *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(pBase) + Offset);
				}
			}

			bool DeviceState::GetButtonPressed(uint32_t Index) const
			{
				if (Index < mNumDigitalButtons)
				{
					const uint8_t Bit = static_cast<uint8_t>(1u << (Index & 7));
					return (mDigitalButtonValues[Index >> 3] & Bit) == Bit;
				}

				if (Index < mNumDigitalButtons + mNumAnalogButtons)
				{
					return mButtonValues[Index - mNumDigitalButtons] != 0;
				}

				return false;
			}

			uint8_t DeviceState::GetButtonValue(uint32_t Index) const
			{
				if (Index < mNumDigitalButtons)
				{
					const uint8_t Bit = static_cast<uint8_t>(1u << (Index & 7));
					return ((mDigitalButtonValues[Index >> 3] & Bit) == Bit) ? MAX_BUTTON_VALUE : 0;
				}

				if (Index < mNumDigitalButtons + mNumAnalogButtons)
				{
					return mButtonValues[Index - mNumDigitalButtons];
				}

				return 0;
			}

			int32_t DeviceState::GetAxisValue(uint32_t Index) const
			{
				return mAxisValues[Index];
			}

			Manager* Manager::GetInstance()
			{
				return *reinterpret_cast<Manager**>(0x0113D294);
			}

			uint32_t Manager::GetNumberDevices() const
			{
				return ReadAt<uint32_t>(m_pLLManager, kLLNumberDevices);
			}

			const DeviceInfo* Manager::GetDeviceInfo(uint32_t DeviceIndex) const
			{
				const DeviceInfo* pInfo = ReadAt<const DeviceInfo*>(m_pLLManager, kLLDeviceInfoTable + (DeviceIndex * 4));

				// An UNKNOWN type marks an empty slot, and the game treats it as absent.
				if (pInfo != nullptr && pInfo->GetDeviceType() == DeviceInfo::UNKNOWN)
				{
					return nullptr;
				}

				return pInfo;
			}

			const DeviceState* Manager::GetDeviceState(uint32_t DeviceIndex) const
			{
				return ReadAt<const DeviceState*>(m_pLLManager, kLLDeviceStateTable + (DeviceIndex * 4));
			}

			const DeviceState* Manager::FindFirstDeviceStateOfType(DeviceInfo::Type InType) const
			{
				const uint32_t NumDevices = GetNumberDevices();

				for (uint32_t Index = 0; Index < NumDevices; ++Index)
				{
					const DeviceInfo* pInfo = GetDeviceInfo(Index);
					if (pInfo == nullptr || pInfo->GetDeviceType() != InType)
					{
						continue;
					}

					const DeviceState* pState = GetDeviceState(Index);
					if (pState != nullptr && pState->IsConnected())
					{
						return pState;
					}
				}

				return nullptr;
			}
		} // controller
	} // core
} // rw
