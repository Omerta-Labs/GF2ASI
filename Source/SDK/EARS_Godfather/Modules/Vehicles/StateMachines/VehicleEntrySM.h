#pragma once

// SDK
#include "SDK/EARS_Common/BitFlags.h"
#include "SDK/EARS_Common/SafePtr.h"
#include "SDK/EARS_Framework/Toolkits/StateMachine/AnimateStateMachine.h"
#include "SDK/EARS_Physics/Behaviours/Vehicles/BaseVehicle.h"

namespace EARS::Modules
{
	// forward declares
	class Sentient;

	enum class VehicleSeatPreference : int32_t
	{
		VehicleSeatPreference_REF = -1,
		SEAT_DRIVER = 0x0,
		SEAT_PASSENGER = 0x1,
		SEAT_REAR_LEFT = 0x2,
		SEAT_REAR_RIGHT = 0x3,
		SEAT_CLOSEST_FRONT = 0x4,
		SEAT_CLOSEST_REAR = 0x5,
		SEAT_CLOSEST = 0x6,
		VehicleSeatPreference_MAX_VALUE = 0x7,
	};

	struct QuatTransS
	{
		float q[4];
		float t[4];
	};


	class VehicleEntrySM : public EARS::Framework::AnimateStateMachine
	{
	public:

		VehicleEntrySM() = delete;
		VehicleEntrySM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~VehicleEntrySM();

		//~ Begin VehicleEntrySM Interface
		virtual uint32_t GetStateMachineID() const override { return 0xFA2C42B8; }
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
		//~ End VehicleEntrySM Interface

		virtual void SetEntryParameters(EARS::Vehicles::WhiteboxCar* Vehicle, EARS::Modules::Sentient* Sentient, unsigned int Flags);

		static VehicleEntrySM* S_VehicleEntrySM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

	private:

		// TransIDs extending AnimateSMTransID (base TRANSID_LAST = 10)
		// PC ASM: sub_883A50, switch on [ecx-0Ah] with 5 cases
		enum VehicleEntrySMTransID : uint32_t
		{
			TRANSID_SENTIENT_ISINCAR      = 10,  // bit 19 of Sentient+0x1B94 — is the sentient in a vehicle?
			TRANSID_SENTIENT_ACTION       = 11,  // Sentient->IsPlayingAction(0x46F5C7B4)
			TRANSID_DOOR_NOT_REACHABLE    = 12,  // DoorLocator→index, !Vehicle->IsPlayingSecondaryAnim(index)
			TRANSID_SCALED_TIMEOUT        = 13,  // TimeInCurrentState >= TRACK_TO_DOOR_TIME / m_EnterSpeedMult
			TRANSID_VEHICLE_GONE          = 14,  // Vehicle SafePtr resolves to null
			TRANSID_LAST                  = 15
		};

		/** Convert a VehicleLocator's m_LocatorID to a door index (0–3).
		 *  PC ASM: sub_875B40 — pure switch: LocatorID 10→0, 11→1, 12→2, 13→3, default→-1.
		 *  This is a standalone function (does not use 'this'). */
		static int DoorLocatorToIndex(const EARS::Vehicles::BaseVehicle::VehicleLocator* Locator);

		/** Time threshold for TRANSID_SCALED_TIMEOUT.
		 *  360 name: TRACK_TO_DOOR_TIME, PC constant: dword_D76F64 = 0x3F28F5C3. */
		static constexpr float TRACK_TO_DOOR_TIME = 0.66f;

		enum class HeadingAlignment : int32_t
		{
			HEADING_ALIGNMENT_AT = 0x0,
			HEADING_ALIGNMENT_LEFT = 0x1,
			HEADING_ALIGNMENT_RIGHT = 0x2,
			HEADING_ALIGNMENT_COUNT = 0x3,
		};

		SafePtr<EARS::Vehicles::WhiteboxCar> m_Vehicle;
		EARS::Modules::Sentient* m_Sentient = nullptr;
		EARS::Vehicles::BaseVehicle::VehicleLocator m_DoorLocator;
		EARS::Vehicles::BaseVehicle::VehicleLocator m_SeatLocator;
		EARS::Modules::VehicleSeatPreference m_SeatPreference;
		EARS::Modules::VehicleEntrySM::HeadingAlignment m_DoorLocatorHeadingAxis;
		Flags32 m_VehicleEntryFlags;
		float m_EnterSpeedMult = 0.0f;
		__declspec(align(8)) RwMatrixTag m_BoneOffset;
		QuatTransS m_Start;
		QuatTransS m_Door;
		QuatTransS m_Seat;
	};

	static_assert(sizeof(VehicleEntrySM) == 0x120);
}
