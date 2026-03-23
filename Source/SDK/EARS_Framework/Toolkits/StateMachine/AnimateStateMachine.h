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

			AnimateStateMachineParams() = default;
			AnimateStateMachineParams(Animated* InOwner)
				: EARS::StateMachineSys::StateMachineParams()
				, m_AnimatedOwner(InOwner)
			{
				SetClassID(0xD42A2C49); // kStateMachine_AnimateStateMachine
			}

			Animated* GetOwner() const { return m_AnimatedOwner; }

		private:

			// The owner of this Animated SM
			Animated* m_AnimatedOwner = nullptr;
		};

		// Layout confirmed from both PC (sub_4AC700) and 360 ASM.
		struct AnimMsg
		{
			uint32_t AnimID;						// +0x0
			uint8_t  bBlend;						// +0x4
			uint8_t  bForceAnim;					// +0x5
			uint8_t  bIgnoreGameMovementBlend;		// +0x6
			uint8_t  _pad;							// +0x7
			float    FrameRateScale;				// +0x8
		};

		class AnimateStateMachine : public EARS::Framework::InterruptableStateMachine
		{
		public:

			AnimateStateMachine() = delete;
			AnimateStateMachine(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
			virtual ~AnimateStateMachine();

			//~ Begin EARS::Framework::InterruptableStateMachine interface
			virtual uint32_t GetStateMachineID() const override { return kStateMachine_AnimateStateMachine; }
			virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
			virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
			//~ End EARS::Framework::InterruptableStateMachine interface

			virtual int PlayAnim(const uint32_t AnimID, const bool bBlend, const bool bForceAnim, const bool bIgnoreGameMovementBlend, const float FrameRateScale, const bool bGameMovementTranslationScale);

		protected:

			//~ Begin InterruptableStateMachine Interface
			virtual void SpawnChild(uint32_t SimTime, float FrameTime, uint32_t TableID) override;
			//~ End InterruptableStateMachine Interface

			/** True if the current primary animation has finished playing. */
			bool AnimDone() const;

			/** True if the current primary animation is a looping animation that is done. */
			bool IsLoopingAnim() const;

			/** True if the secondary animation on the given slot has finished playing. */
			bool SecondaryAnimDone(int Slot) const;

			/** Play a secondary animation on the given slot. */
			int PlaySecondaryAnim(int Slot, uint32_t AnimID, bool bBlend, bool bForceAnim, bool bIgnoreGameMovementBlend, float FrameRateScale, bool bGameMovementTranslationScale, bool bUnk);

			/** Play a secondary animation without triggering a state transition.
		 *  PC version has a reduced signature vs PlaySecondaryAnim (no bIgnoreGMB, bGMTScale, bUnk). */
			int PlaySecondaryAnimWithoutTransition(int Slot, uint32_t AnimID, bool bBlend, bool bForceAnim, float FrameRateScale);

			/** Reset the secondary animation on the given slot. */
			void ResetSecondaryAnimation(int Slot);

			/** Force-reset the secondary animation on the given slot, regardless of state. */
			void ForceResetSecondaryAnimation(int Slot);

			/** True if a secondary animation is currently playing on the given slot. */
			bool IsPlayingSecondaryAnim(int Slot) const;

			/** Get the Animated entity that owns this state machine. */
			EARS::Framework::Animated* GetAnimated() const;

			SafePtr<EARS::Framework::Animated> m_AnimatedOwner;		// 0x48

		private:

			// TransIDs extending InterruptableSMTransID (base TRANSID_LAST = 7)
			enum AnimateSMTransID : uint32_t
			{
				TRANSID_ANIMDONE             = 7,   // primary anim finished (remaining_time <= 0 OR anim flags 0x2000003)
				TRANSID_ANIMDONE_NEXTFRAME   = 8,   // primary anim looping-done check
				TRANSID_SECONDARYANIMDONE    = 9,   // secondary anim on slot [TransData+4] finished
				TRANSID_LAST                 = 10
			};

			// MessageIDs extending InterruptableSMMessageID (base MESSAGE_LAST = 7)
			enum AnimateSMMessageID : uint32_t
			{
				MESSAGE_PLAYANIM         = 7,   // PlayAnim() with AnimMsg struct at MsgData+4
				MESSAGE_STOPANIM         = 8,   // stop/complete current anim (Animated vtable slot 89)
				MESSAGE_SETFRAMERATESCALE = 9,  // set anim frame rate scale (AnimPlayer+0x130)
				MESSAGE_LAST             = 10
			};

			static constexpr uint32_t kStateMachine_AnimateStateMachine = 0xD42A2C49;
		};
	}
}
