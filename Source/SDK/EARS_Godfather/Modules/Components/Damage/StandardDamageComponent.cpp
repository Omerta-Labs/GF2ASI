#include "StandardDamageComponent.h"

// HOOK
#include "Addons/Hook.h"

void EARS::Modules::StandardDamageComponent::SetInvincible(const bool bIsInvincible)
{
	MemUtils::CallClassMethod<void, EARS::Modules::StandardDamageComponent*, bool>(0x9AE900, this, bIsInvincible);
}
