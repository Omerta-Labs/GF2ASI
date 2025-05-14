#pragma once

// CPP
#include "SDK/EARS_Common/Bitflags.h"
#include "SDK/EARS_Framework/Toolkits/StateMachine/AnimateStateMachine.h"

namespace EARS
{
	namespace Modules
	{
		// forward declares
		class Sentient;
		
		class SentientSM : public EARS::Framework::AnimateStateMachine
		{
		public:

			SentientSM() = delete;
			SentientSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
			virtual ~SentientSM();

			//~ Begin AnimateStateMachine Interface
			virtual int PlayAnim(const uint32_t AnimID, const bool bBlend, const bool bForceAnim, const bool bIgnoreGameMovementBlend, const float FrameRateScale, const bool bGameMovementTranslationScale);
			//~ End AnimateStateMachine Interface

		private:

			// Disable head tracking on the owning Sentient
			void DeactivateHeadTracking();

			// Whether or not Gravity should be enabled/disabled on owning Sentient
			void EnableGravity(bool bEnable);

			// flags stored and queried using m_SharedFlags
			enum SharedFlags
			{
				SHARED_FLAG_HEADTRACK_ENABLED = 0x1,
				SHARED_FLAG_GRAVITY_ENABLED = 0x2,
				SHARED_FLAG_GRAVITY_TOGGLED = 0x4,
			};

			Sentient* m_Sentient = nullptr;				// 0x50

			// NB: Game engine originally didn't use this, but it just simplifies reimplemented code
			Flags32 m_SharedFlags;						// 0x54

		};
	}
}
