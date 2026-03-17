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

			//~ Begin EARS::StateMachineSys::StateMachine interface
			virtual uint32_t GetStateMachineID() const override { return 0x5795DD28; }
			virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
			virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
			virtual EARS::StateMachineSys::StateMachine* Update(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t& TerminateLevel, uint32_t EvalLevel) override;
			virtual void AddChild(uint32_t SimTime, float FrameTime, EARS::StateMachineSys::StateMachine* Child) override;
			//~ End EARS::StateMachineSys::StateMachine interface

			/** Rebuild m_InterruptsChildrenHandleCache from the subtree if dirty. Returns m_InterruptsHandled | m_InterruptsChildrenHandleCache. */
			uint32_t RecalculateInterruptsHandledBySM();

		protected:

			/** query whether interrupt flag is active */
			bool TestInterruptFlag(uint32_t Interrupt) const;

			/** clear interrupt flag from int */
			void ClearInterruptFlag(uint32_t Interrupt);

		private:

			// TransIDs extending EARS::StateMachineSys::Transition::StateMachineTransID (base TRANSID_LAST = 5)
			enum InterruptableSMTransID : uint32_t
			{
				TRANSID_TESTINTERRUPT = 5,  // test m_InterruptFlags & bitmask (read-only)
				TRANSID_TESTANDCLEARINTERRUPT = 6,  // test m_InterruptFlags & bitmask and clear on match
				TRANSID_LAST = 7
			};

			// MessageIDs extending EARS::StateMachineSys::State::StateMessageID (base MESSAGE_LAST = 3)
			enum InterruptableSMMessageID : uint32_t
			{
				MESSAGE_SETINTERRUPTHANDLE = 3,  // m_InterruptsHandled |= flag
				MESSAGE_CLEARINTERRUPTHANDLE = 4,  // m_InterruptsHandled &= ~flag
				MESSAGE_INITINTERRUPTHANDLE = 5,  // m_InterruptsHandled = flag
				MESSAGE_SENDINTERRUPT = 6,  // SendInterrupt(flag) through the tree
				MESSAGE_LAST = 7
			};

			/** Walk the tree from root, find the SM that handles flagID, and set its m_InterruptFlags. */
			void SendInterrupt(uint32_t flagID);

			/** Recursively find the SM in this subtree that handles flagID via m_InterruptsHandled. */
			InterruptableStateMachine* FindInterruptHandler(uint32_t flagID);

			/** Mark the SM's parent as dirty only if we're dirty. */
			void SetParentInterruptDirtyFlag() const;

			/** Mark that the SM is currently pending interrupt. Automatically calls on parent */
			void SetInterruptDirtyFlag();

			/** Mark the SM has as dirty */
			void SetInterruptsHandled(uint32_t InterruptsHandled);

			/** Mark the SM has as potentially no longer dirty */
			void ClearInterruptsHandled(uint32_t InterruptsHandled);

			/** initialise interrupts */
			void InitInterruptsHandled(uint32_t InterruptsHandled);

			/** Mark all interrupt values as 0 */
			void ClearInterrupts();

			uint32_t m_InterruptFlags = 0;
			uint32_t m_InterruptsHandled = 0;
			uint32_t m_InterruptsChildrenHandleCache = 0;
			bool m_bInterruptsCacheDirty = false;
		};
	}
}
