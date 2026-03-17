#include "UIFrontend.h"

#include "Addons/Hook.h"

// SDK
#include "SDK/EARS_Common/Bitflags.h"

namespace EARS::Apt
{
	static UIFrontend::StateData s_StateData[37];

	class PortLockManager
	{
	public:

		void SetIsDialogAllowed(const bool bDialogAllowed)
		{
			MemUtils::CallClassMethod<void, PortLockManager*, bool>(0x068C590, this, bDialogAllowed);
		}

		static PortLockManager* GetInstance()
		{
			return *(PortLockManager**)0x1129814;
		}
	};

	void UIFrontend::StartNewGame()
	{
		MemUtils::CallClassMethod<void, UIFrontend*>(0x9315A0, this);
	}

	void UIFrontend::ExecuteNextState()
	{
		hook::Type<FrontendState> CurrentStateVal = hook::Type<FrontendState>(0x1130548);
		hook::Type<FrontendState> NextStateVal = hook::Type<FrontendState>(0x0E5672C);
		hook::Type<Flags32> FrontendFlagsVal = hook::Type<Flags32>(0x01130590);

		const Flags32 FrontendFlags = FrontendFlagsVal.get();
		//if (FrontendFlags.Test((int)UIFrontendFlags::FE_FLAG_ALLOW_STATE_CHANGES))
		if ((FrontendFlags.GetAllFlags() & 0x10) == 0)
		{
			const FrontendState NextState = NextStateVal.get();
			UIFrontend::StateData& State = s_StateData[(int)NextState - 1];

			const FrontendState PreviousState = CurrentStateVal.get();
			CurrentStateVal.set(PreviousState);

			PortLockManager* PortLockMgr = PortLockManager::GetInstance();
			PortLockMgr->SetIsDialogAllowed(State.m_Flags != 1);

			State.m_Func(this, PreviousState);
		}
	}

	void UIFrontend::AddNextStateSequence(int DelayTimeMS)
	{
		MemUtils::CallClassMethod<void, UIFrontend*>(0x931F80, this);
	}

	void UIFrontend::EnterState_StartNewGame(int ePreviousState)
	{
	}

	UIFrontend* UIFrontend::GetInstance()
	{
		return *(UIFrontend**)0x1129928;
	}

	void UIFrontend::PatchState(int State, StateFunc Function)
	{
		UIFrontend::StateData& State2 = s_StateData[State - 1];
		State2.m_Func = Function;
	}

	void UIFrontend::CopyStates()
	{
		hook::Type<UIFrontend::StateData[37]> States = hook::Type<UIFrontend::StateData[37]>(0xD89450);
		for (int32_t idx = 0; idx < 37; idx++)
		{
			s_StateData[idx].m_Flags = States[idx].m_Flags;
			s_StateData[idx].m_State = States[idx].m_State;
			s_StateData[idx].m_Func = States[idx].m_Func;
		}
	}
}
