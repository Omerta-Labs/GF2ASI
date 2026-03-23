#pragma once

#include "SDK/EARS_Framework/Toolkits/StateMachine/AnimateStateMachine.h"

// Addons
#include "Addons/Hook.h"

namespace EARS::Modules
{
	class OnGraphPathingSM : public EARS::Framework::AnimateStateMachine
	{
	public:

		OnGraphPathingSM() = delete;
		OnGraphPathingSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~OnGraphPathingSM();

		//~ Begin AnimateStateMachine Interface
		virtual uint32_t GetStateMachineID() const override { return 0x6B4FE568; }
		virtual EARS::StateMachineSys::StateMachine* Update(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t& TerminateLevel, uint32_t EvalLevel) override;
		virtual int PlayAnim(const uint32_t AnimID, const bool bBlend, const bool bForceAnim, const bool bIgnoreGameMovementBlend, const float FrameRateScale, const bool bGameMovementTranslationScale) override;
		//~ End AnimateStateMachine Interface

		static EARS::StateMachineSys::StateMachine* S_OnGraphPathingSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);


	protected:

		//~ Begin AnimateStateMachine Interface
		virtual void SpawnChild(uint32_t SimTime, float FrameTime, uint32_t TableID) override;
		virtual EARS::StateMachineSys::StateMachineSnapshot* WriteInitDataToSnapShot(EARS::StateMachineSys::StateMachineSnapshot* pSnap) override;
		//~ End AnimateStateMachine Interface

	private:
		char m_Padding[0x9C];
	};
	static_assert(sizeof(OnGraphPathingSM) == 0xEC);
}
