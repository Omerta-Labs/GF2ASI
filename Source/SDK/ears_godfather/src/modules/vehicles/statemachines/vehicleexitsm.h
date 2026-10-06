#pragma once

#include "framework/toolkits/statemachine/animatesm.h"

// Addons
#include "Platform/MemUtils.h"

namespace EARS::Modules
{
	class VehicleExitSM : public EARS::Framework::AnimateStateMachine
	{
	public:

		VehicleExitSM() = delete;
		VehicleExitSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~VehicleExitSM();

		//~ Begin AnimateStateMachine Interface
		virtual uint32_t GetStateMachineID() const override { return 0x0FF45BBCA; }
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
		virtual int PlayAnim(const uint32_t AnimID, const bool bBlend, const bool bForceAnim, const bool bIgnoreGameMovementBlend, const float FrameRateScale, const bool bGameMovementTranslationScale) override;
		//~ End AnimateStateMachine Interface

		static EARS::StateMachineSys::StateMachine* S_VehicleExitSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

		// State indices for this SM's state table, as passed to SMBuilder::AddState.
		// UNVERIFIED: the enum tag is ours; the enumerator names and values are the original's.
		enum VehicleExitSMStateID : uint32_t
		{
			STATE_INIT             = 0,
			STATE_WAIT_FOR_CLEAR   = 1,
			STATE_CHANGE_SEAT      = 2,
			STATE_EXITVEHICLE      = 3,
			STATE_SPAWN_ON_VEHICLE = 4,
			STATE_DESPAWN          = 5,
			STATE_TERMINATE        = 6,
		};

	private:
		char m_Padding[0xB0];
	};
	static_assert(sizeof(VehicleExitSM) == 0x100);
}
