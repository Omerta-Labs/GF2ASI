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

			/* Mouse movement accumulated over the last frame, in pixels, already scaled by the
			 * user's Mouse.Sensitivity option. This is a per-frame displacement, not a rate:
			 * do NOT multiply it by delta time. (PC only - the mouse device also feeds this
			 * into the merged virtual controller's stick, where it is NOT time-normalised.) */
			void GetMouseDelta(float& OutX, float& OutY) const
			{
				OutX = m_MouseDeltaX;
				OutY = m_MouseDeltaY;
			}

			/* Mouse wheel movement over the last frame (PC only) */
			float GetMouseWheelDelta() const { return m_MouseWheelDelta; }

			/* Device slot indices (PC only). INVALID_DEVICE_SLOT when not present. */
			uint8_t GetMouseDeviceSlot() const { return m_MouseDeviceSlot; }
			uint8_t GetKeyboardDeviceSlot() const { return m_KeyboardDeviceSlot; }
			uint8_t GetActiveDeviceSlot() const { return m_ActiveDeviceSlot; }

			/* Sentinel stored in the device slot bytes when no device is assigned */
			static constexpr uint8_t INVALID_DEVICE_SLOT = 0x12;

			/** Fetch the singleton of the InputDeviceManager */
			static InputDeviceManager* GetInstance();

		private:

			char m_Padding_InputDevice[0x1C];
			Controller_Info m_Controllers[16];		// 0x2C
			char m_Padding_InputDevice1[0x80];		// 0x46C
			void* m_InputDevices[16];				// 0x4EC - per-slot device objects (XInput/keyboard/mouse)
			bool m_bMergeDevices;					// 0x52C - when set, source slots are merged into the target slot each frame
			bool m_MergeSourceSlots[16];			// 0x52D
			uint8_t m_MergeTargetSlot;				// 0x53D
			char m_Padding_InputDevice2[0x6];		// 0x53E
			float m_MouseDeltaX;					// 0x544 - per-frame pixel delta * Mouse.Sensitivity, +X = right
			float m_MouseDeltaY;					// 0x548 - per-frame pixel delta * Mouse.Sensitivity, +Y = down
			float m_MouseWheelDelta;				// 0x54C
			uint32_t m_MouseButtons;				// 0x550
			char m_Padding_InputDevice3[0x10];		// 0x554
			uint8_t m_MouseDeviceSlot;				// 0x564
			uint8_t m_KeyboardDeviceSlot;			// 0x565
			uint8_t m_LastLeftStickDeviceSlot;		// 0x566
			uint8_t m_LastRightStickDeviceSlot;		// 0x567
			uint8_t m_ActiveDeviceSlot;				// 0x568 - most recently used device slot
			char m_Padding_InputDevice4[0x3];		// 0x569
			uint32_t m_ActiveDeviceTime;			// 0x56C
			char m_Padding_InputDevice5[0x1C];		// 0x570
		};

		static_assert(sizeof(EARS::Framework::InputDeviceManager) == 0x58C, "EARS::Framework::InputDeviceManager must equal 0x58C");
	} // Framework
} // EARS
