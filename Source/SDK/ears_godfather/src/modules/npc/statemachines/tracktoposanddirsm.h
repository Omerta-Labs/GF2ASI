#pragma once

#include "framework/toolkits/statemachine/animatesm.h"

// Addons
#include "Platform/MemUtils.h"

namespace EARS::Modules
{
	class TrackToPositionAndDirectionSM : public EARS::Framework::AnimateStateMachine
	{
	public:

		static constexpr uint32_t kStateMachine_TrackToPositionAndDirectionSM = 0x27A0F69C;

		TrackToPositionAndDirectionSM() = delete;
		TrackToPositionAndDirectionSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~TrackToPositionAndDirectionSM();

		//~ Begin AnimateStateMachine Interface
		virtual uint32_t GetStateMachineID() const override { return kStateMachine_TrackToPositionAndDirectionSM; }
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
		virtual int PlayAnim(const uint32_t AnimID, const bool bBlend, const bool bForceAnim, const bool bIgnoreGameMovementBlend, const float FrameRateScale, const bool bGameMovementTranslationScale) override;
		//~ End AnimateStateMachine Interface

		static EARS::StateMachineSys::StateMachine* S_TrackToPositionAndDirectionSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

		/** Describe and register the "trackToPositionAndDirectionStateTable" state table. */
		static void BuildStateMachine();

		/** Bits of the flag word the two Setup* entry points take. */
		enum TrackToPositionAndDirectionSMFlags : uint32_t
		{
			FLAGS_WAITFORANIMDONE     = 0x1,
			FLAGS_FORCEDIRECTION      = 0x2,
			FLAGS_FORCEDDIRECTION_CCW = 0x4,
		};

		enum TrackToPosAndDirFlags : uint32_t
		{
			FLAG_TRACKTOPOSANDDIR_USEPROXY = 0x1,
		};

	private:

		// TransIDs extending AnimateSMTransID (base TRANSID_LAST = 10).
		enum TrackToPositionAndDirectionSMTransID : uint32_t
		{
			TRANSID_DONETRACKING = 0xA,  // tracking finished, or was never set up
			TRANSID_IS_GHOST     = 0xB,  // owner is a network ghost, so the SM must not drive it
			TRANSID_LAST         = 0xC,
		};

		// MessageIDs extending AnimateSMMessageID (base MESSAGE_LAST = 10).
		// UNVERIFIED: the enum tag is ours; the enumerator names and values are the original's.
		enum TrackToPositionAndDirectionSMMessageID : uint32_t
		{
			MESSAGE_UPDATETRACKING = 0xA,  // per-frame tracking step; dispatches to ProcessUpdate
			MESSAGE_LAST           = 0xB,
		};

		// TODO: this class's members and its remaining methods (SetupTrackToPositionAndDirection,
		// SetupTrackToHeightCorrectedPositionAndDirection, SnapToPosition, GetTrackingPos,
		// ForceUndershoot, GetTransIDString and the private tracking helpers) are known but not
		// reconstructed yet.  Nothing has confirmed the member layout against the PC build, so the
		// padding stays until it is.
		char m_Padding[0x48];
	};
	static_assert(sizeof(TrackToPositionAndDirectionSM) == 0x98);
}
