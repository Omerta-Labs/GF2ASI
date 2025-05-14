#pragma once

// CPP
#include "SDK/EARS_Common/SafePtr.h"
#include "SDK/EARS_Framework/Toolkits/StateMachine/InterruptableStateMachine.h"

namespace EARS
{
	namespace Framework
	{
		// forward declares
		class Animated;

		struct AnimateStateMachineParams : public EARS::StateMachineSys::StateMachineParams
		{
		public:

			Animated* GetOwner() const { return m_AnimatedOwner; }

		private:

			// The owner of this Animated SM
			Animated* m_AnimatedOwner = nullptr;
		};
	
		class AnimateStateMachine : public EARS::Framework::InterruptableStateMachine
		{
		public:

			AnimateStateMachine() = delete;
			AnimateStateMachine(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
			virtual ~AnimateStateMachine();

			virtual int PlayAnim(const uint32_t AnimID, const bool bBlend, const bool bForceAnim, const bool bIgnoreGameMovementBlend, const float FrameRateScale, const bool bGameMovementTranslationScale) = 0;

		protected:

			SafePtr<EARS::Framework::Animated> m_AnimatedEntity;		// 0x48
		};
	}
}
