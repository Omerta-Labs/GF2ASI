#include "CharacterProxy.h"

// Addons
#include "Addons/Hook.h"

void EARS::Havok::CharacterProxy::EnableGravity(bool bValue)
{
	m_Flags.Set((uint32_t)CharacterProxyFlag::FLAG_DISABLE_GRAVITY, bValue == false);
}

bool EARS::Havok::CharacterProxy::IsGravityEnabled() const
{
	return m_Flags.Test((uint32_t)CharacterProxyFlag::FLAG_DISABLE_GRAVITY) == false;
}

void EARS::Havok::CharacterProxy::SetCollisionState(const CollisionState NewCollisionState)
{
	MemUtils::CallClassMethod<void, EARS::Havok::CharacterProxy*, CollisionState>(0x05512C0, this, NewCollisionState);
}
