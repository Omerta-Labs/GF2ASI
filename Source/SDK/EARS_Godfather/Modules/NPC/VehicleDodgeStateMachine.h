#pragma once

#include "SDK/EARS_Framework/Toolkits/StateMachine/AnimateStateMachine.h"

// Addons
#include "Addons/Hook.h"

namespace EARS::Modules
{
	class VehicleDodgeStateMachine : public EARS::Framework::AnimateStateMachine
	{
	public:

		VehicleDodgeStateMachine() = delete;
		VehicleDodgeStateMachine(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~VehicleDodgeStateMachine();

		//~ Begin AnimateStateMachine Interface
		virtual uint32_t GetStateMachineID() const override { return 0x0BD0065C8; }
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
		virtual int PlayAnim(const uint32_t AnimID, const bool bBlend, const bool bForceAnim, const bool bIgnoreGameMovementBlend, const float FrameRateScale, const bool bGameMovementTranslationScale) override;
		//~ End AnimateStateMachine Interface

		static EARS::StateMachineSys::StateMachine* S_VehicleDodgeStateMachine_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

	private:
		char m_Padding[0x20];
	};
	static_assert(sizeof(VehicleDodgeStateMachine) == 0x70);
}
