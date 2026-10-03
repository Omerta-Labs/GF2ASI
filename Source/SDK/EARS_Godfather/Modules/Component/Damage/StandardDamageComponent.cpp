#include "StandardDamageComponent.h"

// HOOK
#include "Addons/Hook.h"

void EARS::Modules::StandardDamageComponent::SetInvincible(const bool bIsInvincible)
{
    if (bIsInvincible)
    {
        m_InvincibleRefCount++;
    }
    else if (m_InvincibleRefCount > 0)
    {
        m_InvincibleRefCount--;
    }

    if (m_InvincibleRefCount)
    {
        m_DamageableFlags.Set(1);
    }
    else
    {
        m_DamageableFlags.Clear(1);
    }
}

bool EARS::Modules::StandardDamageComponent::IsInvicible() const
{
    return m_DamageableFlags.Test(1);
}
