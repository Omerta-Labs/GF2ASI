#pragma once

// CPP
#include "SDK/EARS_StateMachine/StateMachine.h"

namespace EARS
{
	namespace Framework
	{
		class InterruptableStateMachine : public EARS::StateMachineSys::StateMachine
		{
		public:

			InterruptableStateMachine() = delete;
			InterruptableStateMachine(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
			virtual ~InterruptableStateMachine();

		private:

			/** Mark the SM's parent as dirty only if we're dirty. */
			void SetParentInterruptDirtyFlag() const;

			/** Mark that the SM is currently pending interrupt. Automatically calls on parent */
			void SetInterruptDirtyFlag();

			uint32_t m_InterruptFlags = 0;
			uint32_t m_InterruptsHandled = 0;
			uint32_t m_InterruptsChildrenHandleCache = 0;
			bool m_bInterruptsCacheDirty = false;
		};
	}
}
