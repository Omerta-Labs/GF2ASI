#include "SentientSM.h"

// SDK
#include "SDK/EARS_Godfather/Modules/Sentient/Sentient.h"

#include "Addons/Hook.h"

namespace EARS
{
	namespace Modules
	{
		SentientSM::SentientSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams)
			: EARS::Framework::AnimateStateMachine(TableID, SMParams)
			, m_SharedFlags(0)
		{
			// need to assign as sentient if possible
			EARS::Framework::Animated* AnimEntity = m_AnimatedEntity.GetPtr();
			m_Sentient = EARS::Framework::_GetInterface<Sentient>(AnimEntity, 0x55859EFAu);
		}

		SentientSM::~SentientSM()
		{
			DeactivateHeadTracking();

			// TODO: Is this correct? Makes no sense as to why they are trying to do here,
			// Unless IDA is trying to shoot me in the foot.
			// Is it essentially trying to make sure the Sentient is matching state with the SM?
			if (m_SharedFlags.Test(SharedFlags::SHARED_FLAG_GRAVITY_TOGGLED))
			{
				if (m_SharedFlags.Test(SharedFlags::SHARED_FLAG_GRAVITY_ENABLED))
				{
					EnableGravity(true);
				}
				else
				{
					EnableGravity(false);
				}
			}
		}

		int SentientSM::PlayAnim(const uint32_t AnimID, const bool bBlend, const bool bForceAnim, const bool bIgnoreGameMovementBlend, const float FrameRateScale, const bool bGameMovementTranslationScale)
		{
			return MemUtils::CallClassMethod<int, SentientSM*, uint32_t, bool, bool, bool, float, bool>(0x080E570, this, AnimID, bBlend, bForceAnim, bIgnoreGameMovementBlend, FrameRateScale, bGameMovementTranslationScale);
		}

		void SentientSM::DeactivateHeadTracking()
		{
			if (m_SharedFlags.Test(SharedFlags::SHARED_FLAG_HEADTRACK_ENABLED))
			{
				m_Sentient->DeactivateHeadTracking(false);
				m_SharedFlags.Clear(SharedFlags::SHARED_FLAG_HEADTRACK_ENABLED);
			}
		}

		void SentientSM::EnableGravity(bool bEnable)
		{
			EARS::Havok::CharacterProxy& SentientProxy = m_Sentient->GetCharacterProxyChecked();
			const bool bCurrentGravityState = SentientProxy.IsGravityEnabled();
			SentientProxy.EnableGravity(bEnable);

			// TODO: Validate whether this is actually working against a proper use case
			if (m_SharedFlags.Test(SharedFlags::SHARED_FLAG_GRAVITY_TOGGLED))
			{
				m_SharedFlags.Clear(SharedFlags::SHARED_FLAG_GRAVITY_TOGGLED);
			}
			else
			{
				m_SharedFlags.Set(SharedFlags::SHARED_FLAG_GRAVITY_TOGGLED);

				if (bCurrentGravityState)
				{
					m_SharedFlags.Set(SharedFlags::SHARED_FLAG_GRAVITY_ENABLED);
				}
				else
				{
					m_SharedFlags.Clear(SharedFlags::SHARED_FLAG_GRAVITY_ENABLED);
				}
			}

			// TODO:
		}
	} // Modules
} // EARS
