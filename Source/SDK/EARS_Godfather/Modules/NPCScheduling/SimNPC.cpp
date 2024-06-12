#include "SimNPC.h"

// Hook
#include "Addons/Hook.h"

String* EARS::Modules::SimNPC::GetName()
{
	return MemUtils::CallClassMethod<String*, EARS::Modules::SimNPC*>(0x0790610, this);
}
