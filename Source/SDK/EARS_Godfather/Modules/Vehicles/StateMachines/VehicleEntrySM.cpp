#include "VehicleEntrySM.h"

#include "SDK/EARS_Godfather/Modules/Sentient/Sentient.h"
#include "SDK/EARS_Godfather/Modules/Vehicles/Behaviours/WhiteboxCar/WhiteboxCar.h"

#include "addons/Hook.h"

namespace EARS::Modules
{
	VehicleEntrySM::VehicleEntrySM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
		: AnimateStateMachine(TableID, SMParams)
		, m_SeatPreference(VehicleSeatPreference::SEAT_CLOSEST)
		, m_DoorLocatorHeadingAxis(HeadingAlignment::HEADING_ALIGNMENT_AT)
		, m_VehicleEntryFlags(0)
		, m_EnterSpeedMult(1.0f)
	{
		m_DoorLocator.m_LocatorID = 0;
		// TODO: RwMatrix_Identify(&this->m_mBoneOffset);
	}

	VehicleEntrySM::~VehicleEntrySM()
	{
	}

	bool VehicleEntrySM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
	{
		return MemUtils::CallClassMethod<bool, VehicleEntrySM*, uint32_t, float, uint32_t, uint32_t, EARS::StateMachineSys::State::StateMessageData*>(
				0x00884500, this, SimTime, FrameTime, CurFlags, MessageID, MsgData);
	}

	// -------------------------------------------------------------------------
	// VehicleEntrySM::CheckTransition — PC address: sub_883A50
	//
	// Switch on TransID with 5 custom cases (10–14), default falls through to
	// AnimateStateMachine::CheckTransition (sub_4AC640).
	//
	// PC ASM evidence:
	//   lea eax, [ecx-0Ah]       ; base = 10
	//   cmp eax, 4               ; 5 cases (10,11,12,13,14)
	//   ja  def_883A63            ; default → AnimateStateMachine::CheckTransition
	//   jmp ds:jpt_883A63[eax*4] ; jump table
	// -------------------------------------------------------------------------
	bool VehicleEntrySM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData)
	{
		switch (TransID)
		{
		case TRANSID_SENTIENT_ISINCAR:
		{
			// PC ASM (case 10, loc_883A88):
			//   mov eax, [esi+58h]       ; m_Sentient
			//   mov eax, [eax+1B94h]     ; Sentient+0x1B94 = m_SentientStateFlags
			//   shr eax, 13h             ; >> 19
			//   and al, 1               ; bit 19 = IsInVehicle
			return m_Sentient->IsInVehicle();
		}

		case TRANSID_SENTIENT_ACTION:
		{
			// PC ASM (case 11, loc_883A9A):
			//   mov ecx, [esi+58h]       ; ecx = m_Sentient
			//   push 46F5C7B4h           ; action hash constant
			//   call sub_45EEC0          ; Animated::IsPlayingAction(hash)
			return m_Sentient->IsPlayingAction(0x46F5C7B4);
		}

		case TRANSID_DOOR_NOT_REACHABLE:
		{
			// PC ASM (case 12, loc_883AAB):
			//   resolve m_Vehicle SafePtr → ecx (unused by sub_875B40)
			//   lea edx, [esi+5Ch]       ; &m_DoorLocator
			//   push edx
			//   call sub_875B40          ; DoorLocatorToIndex(&m_DoorLocator) → eax = door index
			//   reload m_Vehicle SafePtr → ecx
			//   push eax                 ; door index
			//   call sub_45EF10          ; Animated::IsPlayingSecondaryAnim(doorIndex)
			//   test al, al
			//   setz al                  ; return !result
			const int doorIndex = DoorLocatorToIndex(&m_DoorLocator);
			EARS::Vehicles::WhiteboxCar* vehicle = m_Vehicle.GetPtr();
			if (vehicle)
			{
				return (vehicle->IsPlayingSecondaryAnim(doorIndex) == false);
			}
			// Null vehicle path: sub_45EF10 with ecx=0 → [ecx+218h] will fail,
			// but the original ASM still calls through with ecx=0.
			// In practice, the AnimViewInfo null check inside IsPlayingSecondaryAnim returns false,
			// so !false = true.
			return true;
		}

		case TRANSID_SCALED_TIMEOUT:
		{
			return GetTimeInCurState() >= (TRACK_TO_DOOR_TIME / m_EnterSpeedMult);
		}
		case TRANSID_VEHICLE_GONE:
		{
			return (m_Vehicle.GetPtr() == nullptr);
		}
		default:
		{
			return AnimateStateMachine::CheckTransition(SimTime, FrameTime, TransID, TransData);
		}
		}
	}

	/* static */
	int VehicleEntrySM::DoorLocatorToIndex(const EARS::Vehicles::BaseVehicle::VehicleLocator* Locator)
	{
		// Pure switch converting VehicleLocator::m_LocatorID to a 0-based door index.
		switch (Locator->m_LocatorID)
		{
		case 10: return 0;
		case 11: return 1;
		case 12: return 2;
		case 13: return 3;
		default: return -1;
		}
	}

	void VehicleEntrySM::SetEntryParameters(EARS::Vehicles::WhiteboxCar* Vehicle, EARS::Modules::Sentient* Sentient, uint32_t flags)
	{
		MemUtils::CallClassMethod<void, VehicleEntrySM*, EARS::Vehicles::WhiteboxCar*, EARS::Modules::Sentient*, uint32_t>(
				0x0883620, this, Vehicle, Sentient, flags);
	}

	/* static */
	VehicleEntrySM* VehicleEntrySM::S_VehicleEntrySM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams)
	{
		return new VehicleEntrySM(InID, InSMParams);
	}
}
