#pragma once

// SDK
#include "SDK/EARS_Common/Singleton.h"
#include "SDK/EARS_Framework/Core/EventHandler/CEventHandler.h"

namespace EARS
{
	namespace Framework
	{
		enum ButtonMask : int32_t
		{
			CTRL_BUTTON_NONE = 0x0,
			CTRL_BUTTON_LEFT_DPAD_UP = 0x1,
			CTRL_BUTTON_LEFT_DPAD_DOWN = 0x2,
			CTRL_BUTTON_LEFT_DPAD_LEFT = 0x4,
			CTRL_BUTTON_LEFT_DPAD_RIGHT = 0x8,
			CTRL_BUTTON_RIGHT_DPAD_UP = 0x10,
			CTRL_BUTTON_RIGHT_DPAD_DOWN = 0x20,
			CTRL_BUTTON_RIGHT_DPAD_LEFT = 0x40,
			CTRL_BUTTON_RIGHT_DPAD_RIGHT = 0x80,
			CTRL_BUTTON_LEFT_BOTTOM = 0x100,
			CTRL_BUTTON_LEFT_TOP = 0x200,
			CTRL_BUTTON_RIGHT_BOTTOM = 0x400,
			CTRL_BUTTON_RIGHT_TOP = 0x800,
			CTRL_BUTTON_START = 0x1000,
			CTRL_BUTTON_SELECT = 0x2000,
			CTRL_BUTTON_LEFT_STICK = 0x4000,
			CTRL_BUTTON_RIGHT_STICK = 0x8000,
			CTRL_BUTTON_ALL = 0xFFFF,
		};

		enum ButtonStatus : int32_t
		{
			BUTTON_STATUS_UP = 0x0,
			BUTTON_STATUS_DOWN = 0x1,
			BUTTON_STATUS_PRESSED = 0x2,
			BUTTON_STATUS_RELEASED = 0x3,
			BUTTON_STATUS_MAX = 0x4,
		};

		enum ControllerID : int32_t
		{
			CTRL_ID_0 = 0x0,
			CTRL_ID_1 = 0x1,
			CTRL_ID_2 = 0x2,
			CTRL_ID_3 = 0x3,
			CTRL_ID_MAX = 0x4,
			CTRL_ID_ALL = 0x5,
			CTRL_ID_NONE = 0x6,
			CTRL_ID_ANY = 0x7,
			CTRL_ID_ANYTWO = 0x8,
		};

		struct Controller_PositionInfo
		{
			float m_X = 0.0f;
			float m_Y = 0.0f;
		};

		struct Controller_Info
		{
			EARS::Framework::Controller_PositionInfo m_LeftStick;
			EARS::Framework::Controller_PositionInfo m_RightStick;
			uint16_t m_DigitalButtons = 0;
			uint16_t m_PreviousDigitalButtons = 0;
			uint16_t m_MaskedDigitalButtons = 0;
			uint8_t m_PressureButtons[16];
			uint8_t m_PressureFloor[16];
			uint16_t m_Accelerometers[4];
			bool m_ControllerEnabled = false;
			uint8_t m_ActuatorValues[2];
		};

		/**
		 * Core Manager for dealing with the Input Device states
		 */
		class InputDeviceManager : RWS::CEventHandler, Singleton<EARS::Framework::InputDeviceManager>
		{
		public:

			/* check button state on specific joypad */
			bool CheckButtonsANY(uint8_t JoypadIdx, uint16_t ButtonMask, uint8_t Status) const;

			/* Get the state of a current controller */
			const Controller_Info* GetControllerInfo(uint8_t JoypadIdx) const;

			/** Fetch the singleton of the InputDeviceManager */
			static InputDeviceManager* GetInstance();

		private:

			char m_Padding_InputDevice[0x1C];
			Controller_Info m_Controllers[16];		// 0x2C
			char m_Padding_InputDevice1[0x120];
		};

		static_assert(sizeof(EARS::Framework::InputDeviceManager) == 0x58C, "EARS::Framework::InputDeviceManager must equal 0x58C");
	} // Framework
} // EARS
