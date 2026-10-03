#pragma once

// SDK
#include "ears_common/singleton.h"
#include "framework/core/eventhandler/ceventhandler.h"
#include "framework/modules/controllermanager/eventbitmask.h"
#include "framework/modules/controllermanager/controllereventconfig.h"

// C++
#include <stdint.h>

namespace EARS
{
	namespace Common
	{
		struct guid128_t;
	}

	namespace Modules
	{
		/**
		 * The named-event layer on top of EARS::Framework::InputDeviceManager.
		 *
		 * Game code never asks "is the A button down"; it asks whether a named event is
		 * happening, e.g. IsEventHappening(pad, HashString_SDBM("Fire")). What that event
		 * means is data - a ControllerEventConfig ("CEC") pushed onto a per-controller
		 * stack. Push a different config and the same code responds to different input.
		 *
		 * That stack is why a photo mode does not need to patch anything: push a custom
		 * CEC on entry and pop it on exit, and the gameplay bindings underneath come back
		 * exactly as they were.
		 *
		 * UpdateAll runs off both iMsgRunningTick and iMsgPausedTick, so events keep being
		 * evaluated while the game is paused.
		 *
		 * NB: the layout here is the PC build. The X360 build has 4 controllers and puts
		 * m_CMI at +0x18, because its RWS::CEventHandler base is larger.
		 */
		class ControllerManager : RWS::CEventHandler, public Singleton<ControllerManager>
		{
		public:

			static constexpr uint8_t MAX_CONTROLLERS = 16;			// PC; the X360 build has 4
			static constexpr int MAX_RECORDS = 16;					// ControllerManagerInfo::cMaxRecords
			static constexpr uint32_t MAX_CONFIG_RES = 128;			// g_ConfigMap capacity
			static constexpr uint32_t MAX_CONFIG_STACK = 8;			// depth of the per-controller config stack
			static constexpr uint32_t MAX_EVENTS = 128;				// maxEvents the manager is constructed with

			enum eStickIndex : int32_t
			{
				kCtrlLeftStickIndex = 0x0,
				kCtrlRightStickIndex = 0x1,
				kCtrlNumberSticks = 0x2,
			};

			enum eButtonType : int32_t
			{
				BUTTON_START = 0x0,
				BUTTON_SELECT = 0x1,
				BUTTON_LEFT_SHOULDER_TOP = 0x2,
				BUTTON_LEFT_SHOULDER_BOTTOM = 0x3,
				BUTTON_RIGHT_SHOULDER_TOP = 0x4,
				BUTTON_RIGHT_SHOULDER_BOTTOM = 0x5,
				BUTTON_LEFT_DPAD_LEFT = 0x6,
				BUTTON_LEFT_DPAD_RIGHT = 0x7,
				BUTTON_LEFT_DPAD_UP = 0x8,
				BUTTON_LEFT_DPAD_DOWN = 0x9,
				BUTTON_RIGHT_DPAD_LEFT = 0xA,
				BUTTON_RIGHT_DPAD_RIGHT = 0xB,
				BUTTON_RIGHT_DPAD_UP = 0xC,
				BUTTON_RIGHT_DPAD_DOWN = 0xD,
				BUTTON_LEFT_STICK = 0xE,
				BUTTON_RIGHT_STICK = 0xF,
			};

			enum eEventType : int32_t
			{
				kEventType_Undef = -1,
				kEventType_IsHappening = 0x0,
				kEventType_IsJustTriggered = 0x1,
				kEventType_Max = 0x2,
			};

			/** One sampled frame of pad state, packed so it can be compared as a single 64-bit value. */
			union PadInput
			{
				struct
				{
					int8_t stickDirMin[2];	// 0x00
					int8_t stickDirMax[2];	// 0x02
					uint16_t buttonMask;	// 0x04
					uint16_t pad;			// 0x06
				};
				uint64_t completeRecord;	// 0x00
			};

			static_assert(sizeof(PadInput) == 0x8, "ControllerManager::PadInput must equal 0x8");

			/** A PadInput plus how long it has been held - the history a sequence is matched against. */
			struct ControllerManagerRecord
			{
				PadInput padInput = {};	// 0x00
				uint32_t numberMs = 0;	// 0x08
			};

			static_assert(sizeof(ControllerManagerRecord) == 0x10, "ControllerManager::ControllerManagerRecord must equal 0x10");

			struct ControllerManagerStickData
			{
				float x = 0.0f;						// 0x00
				float y = 0.0f;						// 0x04
				float angle = 0.0f;					// 0x08
				float length = 0.0f;				// 0x0C
				uint8_t dirMin = 0;					// 0x10
				uint8_t dirMax = 0;					// 0x11
				uint8_t lastFrameDir = 0;			// 0x12
				bool ignoreMinorVariants = false;	// 0x13
				int8_t validDirTicks = 0;			// 0x14
			};

			static_assert(sizeof(ControllerManagerStickData) == 0x18, "ControllerManager::ControllerManagerStickData must equal 0x18");

			struct ControllerTuneVars
			{
				float iPadDeadZoneRange = 0.0f;					// 0x00
				uint32_t iAnalogStickSpeedSensitivityLeft = 0;	// 0x04
				uint32_t iAnalogStickSpeedSensitivityRight = 0;	// 0x08
				float fMinLengthPrimeDirection = 0.0f;			// 0x0C
				int32_t iMinTicksAcceptDirCharged = 0;			// 0x10
				int32_t iMinTicksAcceptDirQuick = 0;			// 0x14
				float fPadMajorAxes = 0.0f;						// 0x18
				float fCtrlRemapSlope = 0.0f;					// 0x1C
				float fCtrlRemapExp = 0.0f;						// 0x20
			};

			static_assert(sizeof(ControllerTuneVars) == 0x24, "ControllerManager::ControllerTuneVars must equal 0x24");

			/** Per-controller state: the config stack, the input history, and the event bitmasks. */
			struct ControllerManagerInfo
			{
				/** Stack of active configs. The top one is what events resolve against. */
				struct CtrlEventConfigStack
				{
					bool empty() const { return m_top == 0; }
					uint32_t size() const { return m_top; }
					ControllerEventConfig* top() const { return (m_top != 0) ? m_arr[m_top - 1] : nullptr; }

					uint32_t m_top = 0;										// 0x00 - entry count, not an index
					ControllerEventConfig* m_arr[MAX_CONFIG_STACK] = {};		// 0x04
				};

				static_assert(sizeof(CtrlEventConfigStack) == 0x24, "ControllerManagerInfo::CtrlEventConfigStack must equal 0x24");

				/** Parallel stack of last-frame masks, used to derive IsJustTriggered. */
				struct LastFrameEventMaskStack
				{
					uint32_t size() const { return m_top; }
					Bitmask* top() const { return (m_top != 0) ? m_arr[m_top - 1] : nullptr; }

					uint32_t m_top = 0;								// 0x00
					uint32_t m_persistFlag = 0;						// 0x04 - Flags32, one bit per stack level
					Bitmask* m_arr[MAX_CONFIG_STACK] = {};			// 0x08
				};

				static_assert(sizeof(LastFrameEventMaskStack) == 0x28, "ControllerManagerInfo::LastFrameEventMaskStack must equal 0x28");

				bool Active() const { return !ctrlEventConfigStack.empty(); }

				CtrlEventConfigStack ctrlEventConfigStack;					// 0x000
				ControllerManagerStickData sd[kCtrlNumberSticks];			// 0x024
				ControllerManagerRecord arrayOfRecords[MAX_RECORDS];		// 0x058 - 8-aligned, hence 4 bytes of tail padding after sd
				int32_t lastRecordToUse = 0;								// 0x158
				Bitmask* eventMask = nullptr;								// 0x15C - which events fired this frame
				LastFrameEventMaskStack* lastFrameEventMaskStack = nullptr;	// 0x160
				bool controllerEnabled = false;								// 0x164
			};

			static_assert(sizeof(ControllerManagerInfo) == 0x168, "ControllerManager::ControllerManagerInfo must equal 0x168");

			// Config registration

			/* Register a config under its name hash so PushConfiguration can find it.
			 * Fails if a config with the same hash is already registered. Does not take a
			 * reference - the caller keeps ownership of the memory. */
			bool AddConfigResource(ControllerEventConfig* pConfig);

			/* Unregister. Fails while the config still has references, i.e. while pushed. */
			bool RemoveConfigResource(ControllerEventConfig* pConfig);

			/* Push a registered config by name onto this controller's stack and clear the
			 * live event mask. bKeepLastFrameBitmask carries the current "already held"
			 * state across the switch, which stops a button that was down at the moment of
			 * the push from reading as a fresh press under the new config.
			 * (game: 0x00464960) */
			bool PushConfiguration(uint8_t ControllerID, const char* ConfigName, bool bKeepLastFrameBitmask);

			/* Pop back to the previous config. (game: 0x00464A10) */
			bool PopConfiguration(uint8_t ControllerID);

			/* Name of the config currently on top, or null when the stack is empty. */
			const char* GetTopConfigName(uint8_t ControllerID) const;

			/* Resolve the config name an RWS attribute GUID refers to. (game: 0x00464930) */
			const char* GetConfigNameFromRWSGUID(const EARS::Common::guid128_t* pGUID) const;

			// Event queries

			/* Is this event active right now? EventHash is HashString_SDBM(eventName).
			 * ControllerID may be one of the EARS::Framework::ControllerID aggregates
			 * (CTRL_ID_ALL / CTRL_ID_ANY / ...), in which case every pad is tested.
			 * (game: 0x004657D0) */
			bool IsEventHappening(uint8_t ControllerID, uint32_t EventHash);

			/* Did this event become active this frame? (game: 0x004657F0) */
			bool IsEventJustTriggered(uint8_t ControllerID, uint32_t EventHash);

			/* Collect the descriptor indices in the top config whose eventHashID matches.
			 * More than one is normal - that is how alternative bindings are expressed.
			 * Returns how many indices were written.
			 *
			 * Reimplemented rather than bound: the retail build compiled the original with
			 * a non-standard calling convention that cannot be expressed in C++. */
			int GetEventList(uint8_t ControllerID, uint32_t EventHash, int* pOutList, int MaxListSize) const;

			// Stick / button queries

			float GetStickMag(uint8_t ControllerID, eStickIndex StickIndex, bool bRemapped) const;
			float GetStickHeading(uint8_t ControllerID, eStickIndex StickIndex, bool bRemapped) const;
			bool IsButtonHeld(uint8_t ControllerID, eButtonType ButtonType) const;
			bool CheckButtonActivity(uint8_t ControllerID, eButtonType ButtonType, uint8_t Status) const;

			// Frame update

			/* Sample every controller and rebuild the event masks. Driven by
			 * iMsgRunningTick and iMsgPausedTick. (game: 0x00464B80) */
			void UpdateAll();

			ControllerManagerInfo& GetControllerManagerInfo(uint8_t ControllerID) { return m_CMI[ControllerID]; }
			const ControllerManagerInfo& GetControllerManagerInfo(uint8_t ControllerID) const { return m_CMI[ControllerID]; }

			const ControllerTuneVars& GetTuneVars() const { return m_EventControllerTuneVars; }

			/** Singleton<EARS::Modules::ControllerManager>::s_pSingleton (0x01223464) */
			static ControllerManager* GetInstance();

		private:

			ControllerManagerInfo m_CMI[MAX_CONTROLLERS];						// 0x0010
			void* m_pAllocator = nullptr;										// 0x1690 - EA::Allocator::IAllocator*
			ControllerTuneVars m_EventControllerTuneVars;						// 0x1694
			float m_StickMag[MAX_CONTROLLERS * kCtrlNumberSticks];				// 0x16B8
			float m_StickHeading[MAX_CONTROLLERS * kCtrlNumberSticks];			// 0x1738
			float m_RemappedStickMag[MAX_CONTROLLERS * kCtrlNumberSticks];		// 0x17B8
			float m_RemappedStickHeading[MAX_CONTROLLERS * kCtrlNumberSticks];	// 0x1838
			bool m_bLostControllerReported = false;								// 0x18B8
			char m_Padding_ControllerManager[0x13];								// 0x18B9 - PC-only tail, zeroed by the ctor; absent from the X360 build
		};

		static_assert(sizeof(EARS::Modules::ControllerManager) == 0x18D0, "EARS::Modules::ControllerManager must equal 0x18D0");
	} // Modules
} // EARS
