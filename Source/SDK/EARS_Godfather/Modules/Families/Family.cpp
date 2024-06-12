#include "Family.h"

// Addons
#include "Addons/Hook.h"

void EARS::Modules::Family::AddMedic(EARS::Modules::Sentient* NewMedic)
{
	MemUtils::CallClassMethod<void, EARS::Modules::Family*, EARS::Modules::Sentient*>(0x08C3C80, this, NewMedic);
}
