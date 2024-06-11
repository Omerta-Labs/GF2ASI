#include "UIHud.h"

// Addons
#include "Addons/Hook.h"

void EARS::Apt::UIHUD::Suppress()
{
	MemUtils::CallClassMethod<void, EARS::Apt::UIHUD*>(0x093C210, this);
}

void EARS::Apt::UIHUD::Unsuppress()
{
	MemUtils::CallClassMethod<void, EARS::Apt::UIHUD*>(0x093A2F0, this);
}

EARS::Apt::UIHUD* EARS::Apt::UIHUD::GetInstance()
{
	return *(EARS::Apt::UIHUD**)0x1129C4C;
}
