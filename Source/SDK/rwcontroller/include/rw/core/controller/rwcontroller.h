#pragma once

// C++
#include <stdint.h>

/**
 * RenderWare core controller layer (the low-level input backend the PC build runs on).
 *
 * On PC this sits on DirectInput8 for keyboards/mice/joysticks and XInput for pads;
 * rw::core::controller::Manager owns one DeviceInfo + one DeviceState per attached
 * device, and EARS::Framework::InputDeviceManager polls those every frame and folds
 * them down into its own Controller_Info array.
 *
 * Reading a DeviceState directly is the only way to see input the EARS layer discards -
 * most importantly arbitrary keyboard keys, since InputDeviceManager only forwards the
 * handful of keys mapped onto the 16 virtual pad buttons.
 */
namespace rw
{
	namespace core
	{
		namespace controller
		{
			/** Static description of an attached device. Does not change frame to frame. */
			class DeviceInfo
			{
			public:

				enum Type : int32_t
				{
					PAD = 0x0,
					WHEEL = 0x1,
					MOUSE = 0x2,
					KEYBOARD = 0x3,
					UNKNOWN = 0x4,
				};

				enum ButtonCapability : int32_t
				{
					ANALOG = 0x1,
				};

				uint32_t GetDeviceID() const { return mDeviceID; }
				Type GetDeviceType() const { return mDeviceType; }
				uint32_t GetNumberButtons() const { return mNumberButtons; }
				uint32_t GetNumberAxis() const { return mNumberAxis; }

			private:

				uint32_t mDeviceID = 0;					// 0x00
				Type mDeviceType = UNKNOWN;				// 0x04
				uint32_t mNumberButtons = 0;			// 0x08
				uint32_t mNumberAxis = 0;				// 0x0C
				uint32_t* mButtonCapabilities = nullptr;// 0x10 - one ButtonCapability mask per button
			};

			static_assert(sizeof(rw::core::controller::DeviceInfo) == 0x14, "rw::core::controller::DeviceInfo must equal 0x14");

			/**
			 * Per-frame state of one device.
			 *
			 * Button indices are laid out as one flat range: [0, mNumDigitalButtons) are
			 * digital and live as single bits in mDigitalButtonValues, and
			 * [mNumDigitalButtons, mNumDigitalButtons + mNumAnalogButtons) are analog and
			 * live as whole bytes in mButtonValues. The accessors below take an index in
			 * that combined space, exactly as the game's own code does.
			 *
			 * For a DirectInput keyboard the digital indices are DIK_* scancodes - the
			 * engine itself probes 15/56/184 (DIK_TAB/DIK_LMENU/DIK_RMENU) to detect
			 * alt-tab. So DIK_F2 etc. can be read straight out of this.
			 */
			class DeviceState
			{
			public:

				enum ConnectedState : int32_t
				{
					DISCONNECTED = 0x0,
					CONNECTED = 0x1,
				};

				enum NetworkState : int32_t
				{
					LOCAL = 0x0,
					NETWORKED = 0x1,
				};

				enum KeyboardState : int32_t
				{
					KS_SHIFT = 0x1,
					KS_CTRL = 0x2,
					KS_ALT = 0x4,
				};

				enum ValueRanges : int32_t
				{
					MIN_BUTTON_VALUE = 0,
					MAX_BUTTON_VALUE = 255,
					MIN_AXIS_VALUE = -1000,
					MAX_AXIS_VALUE = 1000,
				};

				uint32_t GetDeviceID() const { return mDeviceID; }
				bool IsConnected() const { return mIsConnected == CONNECTED; }
				uint32_t GetTimestamp() const { return mTimestamp; }

				uint32_t GetNumDigitalButtons() const { return mNumDigitalButtons; }
				uint32_t GetNumAnalogButtons() const { return mNumAnalogButtons; }
				uint32_t GetNumberAxis() const { return mNumberAxis; }

				/* Is this button down? Digital buttons read their bit, analog buttons test
				 * for a non-zero value. Out-of-range indices return false. (game: 0x00B930B0) */
				bool GetButtonPressed(uint32_t Index) const;

				/* Pressure for this button, 0..MAX_BUTTON_VALUE. A digital button that is
				 * down reads back as MAX_BUTTON_VALUE. (game: 0x00B93170) */
				uint8_t GetButtonValue(uint32_t Index) const;

				/* Axis position in MIN_AXIS_VALUE..MAX_AXIS_VALUE. NOT bounds checked by
				 * the game - callers must keep Index < GetNumberAxis(). (game: 0x00B93230) */
				int32_t GetAxisValue(uint32_t Index) const;

			private:

				uint32_t mDeviceID = 0;						// 0x00
				ConnectedState mIsConnected = DISCONNECTED;	// 0x04
				NetworkState mIsNetworked = LOCAL;			// 0x08
				uint32_t mTimestamp = 0;					// 0x0C
				uint32_t mNumDigitalButtons = 0;			// 0x10
				uint32_t mNumAnalogButtons = 0;				// 0x14
				uint32_t mNumberAxis = 0;					// 0x18
				uint8_t* mDigitalButtonValues = nullptr;	// 0x1C - bitfield, 1 bit per digital button
				uint8_t* mButtonValues = nullptr;			// 0x20 - one byte per analog button
				int32_t* mAxisValues = nullptr;				// 0x24
			};

			static_assert(sizeof(rw::core::controller::DeviceState) == 0x28, "rw::core::controller::DeviceState must equal 0x28");

			/**
			 * Owner of every attached device. Singleton, created during startup.
			 *
			 * The device tables hang off an internal LLManager (Manager+0x00), so the
			 * accessors here double-dereference. Device indices are dense and stable for
			 * as long as the device stays attached; a slot whose DeviceInfo type is
			 * UNKNOWN is an empty slot and GetDeviceInfo returns null for it.
			 */
			class Manager
			{
			public:

				/** rw::core::controller::Manager::sInstance (0x0113D294) */
				static Manager* GetInstance();

				/* Number of device slots the manager is tracking. (game: 0x00B92E60) */
				uint32_t GetNumberDevices() const;

				/* Null when the slot is empty (device type UNKNOWN). (game: 0x00B94A20) */
				const DeviceInfo* GetDeviceInfo(uint32_t DeviceIndex) const;

				/* Live state for a slot. (game: 0x00B94A40) */
				const DeviceState* GetDeviceState(uint32_t DeviceIndex) const;

				/* Convenience: first connected device of the given type, or null.
				 * Use DeviceInfo::KEYBOARD to reach arbitrary key state. */
				const DeviceState* FindFirstDeviceStateOfType(DeviceInfo::Type InType) const;

			private:

				void* m_pLLManager = nullptr;	// 0x00 - device tables live at +0x40/+0x44/+0xC4
			};
		} // controller
	} // core
} // rw
