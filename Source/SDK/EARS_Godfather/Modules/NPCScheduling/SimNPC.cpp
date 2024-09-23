#include "SimNPC.h"

// Hook
#include "Addons/Hook.h"

String* EARS::Modules::SimNPC::GetName()
{
	return MemUtils::CallClassMethod<String*, EARS::Modules::SimNPC*>(0x0790610, this);
}

void EARS::Modules::SimNPC::SetIsCrewMember(bool bIsCrewMember)
{
	MemUtils::CallClassMethod<void, EARS::Modules::SimNPC*, bool>(0x0791E00, this, bIsCrewMember);
}

bool EARS::Modules::SimNPC::GetIsCrewMember() const
{
	return m_Flags.Test(0x1000000);
}
