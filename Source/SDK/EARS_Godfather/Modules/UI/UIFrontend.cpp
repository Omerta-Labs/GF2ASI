#include "UIFrontend.h"

#include "Addons/Hook.h"

namespace EARS::Apt
{
	void UIFrontend::StartNewGame()
	{
		MemUtils::CallClassMethod<void, UIFrontend*>(0x9315A0, this);
	}

	void UIFrontend::EnterState_StartNewGame(int ePreviousState)
	{
	}

	UIFrontend* UIFrontend::GetInstance()
	{
		return *(UIFrontend**)0x1129928;
	}
}
