#include "Input.h"

#include "Addons/Hook.h"

namespace EARS
{
	namespace Framework
	{
		bool InputDeviceManager::CheckButtonsANY(uint8_t JoypadIdx, uint16_t ButtonMask, uint8_t Status) const
		{
			return MemUtils::CallClassMethod<bool, const InputDeviceManager*, uint8_t, uint16_t, uint8_t>(0x410280, this, JoypadIdx, ButtonMask, Status);
		}

		const Controller_Info* InputDeviceManager::GetControllerInfo(uint8_t JoypadIdx) const
		{
			if (JoypadIdx != 18 && JoypadIdx < 16)
			{
				const Controller_Info& SelectedJoypad = m_Controllers[JoypadIdx];
				if (SelectedJoypad.m_ControllerEnabled)
				{
					return &SelectedJoypad;
				}
			}

			return nullptr;
		}

		InputDeviceManager* InputDeviceManager::GetInstance()
		{
			return *(InputDeviceManager**)0x12233B4;
		}
	} // Framework
} // EARS
