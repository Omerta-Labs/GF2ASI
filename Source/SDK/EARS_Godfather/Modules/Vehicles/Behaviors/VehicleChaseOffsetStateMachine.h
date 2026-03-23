#pragma once

#include "SDK/EARS_Framework/Toolkits/StateMachine/AnimateStateMachine.h"

// Addons
#include "Addons/Hook.h"

namespace EARS::Modules
{
	class VehicleChaseOffsetStateMachine : public EARS::Framework::AnimateStateMachine
	{
	public:

		VehicleChaseOffsetStateMachine() = delete;
		VehicleChaseOffsetStateMachine(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~VehicleChaseOffsetStateMachine();

		//~ Begin AnimateStateMachine Interface
		virtual uint32_t GetStateMachineID() const override { return 0x0DFD3C4CB; }
		virtual EARS::StateMachineSys::StateMachine* Update(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t& TerminateLevel, uint32_t EvalLevel) override;
		virtual void AddChild(uint32_t SimTime, float FrameTime, EARS::StateMachineSys::StateMachine* Child) override;
		virtual void RemoveChild(EARS::StateMachineSys::StateMachine* ChildMachine) override;
		//~ End AnimateStateMachine Interface

		static EARS::StateMachineSys::StateMachine* S_VehicleChaseOffsetStateMachine_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);


	protected:

		//~ Begin AnimateStateMachine Interface
		virtual void SpawnChild(uint32_t SimTime, float FrameTime, uint32_t TableID) override;
		virtual const EARS::StateMachineSys::StateMachineSnapshot* ReadInitDataFromSnapShot(const EARS::StateMachineSys::StateMachineSnapshot* pSnap) override;
		//~ End AnimateStateMachine Interface

	private:
		char m_Padding[0x6C];
	};
	static_assert(sizeof(VehicleChaseOffsetStateMachine) == 0xBC);
}
