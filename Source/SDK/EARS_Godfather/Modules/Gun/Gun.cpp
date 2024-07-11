#include "Gun.h"

// Addons
#include "Addons/Hook.h"

void EARS::Modules::Gun::SetInfiniteAmmo(const bool bInfinite)
{
	if (bInfinite)
	{
		m_GunFlags.Set(GunFlags::FLAG_INFINITE_AMMO);
	}
	else
	{
		m_GunFlags.Clear(GunFlags::FLAG_INFINITE_AMMO);
	}
}

void EARS::Modules::Gun::AddAmmo(const uint32_t Ammo)
{
	MemUtils::CallClassMethod<void, EARS::Modules::Gun*, uint32_t>(0x0701FF0, this, Ammo);
}
