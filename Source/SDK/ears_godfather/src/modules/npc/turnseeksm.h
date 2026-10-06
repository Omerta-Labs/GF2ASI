#pragma once

#include "modules/sentient/statemachines/sentientsm.h"

// Addons
#include "Platform/MemUtils.h"

namespace EARS::Modules
{
	class TurnSeekStateMachine : public EARS::Modules::SentientSM
	{
	public:

		static constexpr uint32_t kStateMachine_TurnSeekStateMachine = 0x1B2D5C51;

		TurnSeekStateMachine() = delete;
		TurnSeekStateMachine(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~TurnSeekStateMachine();

		//~ Begin SentientSM Interface
		virtual uint32_t GetStateMachineID() const override { return kStateMachine_TurnSeekStateMachine; }
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
		//~ End SentientSM Interface

		static EARS::StateMachineSys::StateMachine* S_TurnSeekStateMachine_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

		// This one class backs seven separate state tables, which is why CompileAndRegister takes
		// the class ID apart from the table name.  Each builder registers kStateMachine_TurnSeek-
		// StateMachine against a different table, and the table the SM is created from decides
		// which behaviour it runs.
		static void BuildTurnToEntityStateTable();
		static void BuildTurnToEntityTerminateStateTable();
		static void BuildSetHeadingToEntityStateTable();
		static void BuildTurnToPositionTerminateStateTable();
		static void BuildSeekToPositionTerminateStateTable();
		static void BuildSeekTargetStateTable();
		static void BuildSeekTargetTerminateStateTable();

		/** Bits of the SM's flag word. */
		enum TurnSeekSMFlags : uint32_t
		{
			TURNSEEKFLAG_CROUCHED              = 0x1,
			TURNSEEKFLAG_USE_NPC_IDLE          = 0x2,
			TURNSEEKFLAG_DISABLE_IDLE          = 0x4,
			TURNSEEKFLAG_USE_ANIMATED_ROTATION = 0x8,
			TURNSEEKFLAG_ANIMATED_TURN_DONE    = 0x10,
			TURNSEEKFLAG_TURN_AWAY             = 0x20,
		};

	private:

		// TransIDs extending SentientSMTransID (base TRANSID_LAST = 10).
		enum TurnSeekSMTransID : uint32_t
		{
			TRANSID_NO_TARGET        = 0xA,  // the target entity went away
			TRANSID_TARGETDIRREACHED = 0xB,  // heading is within the close-enough cone
			TRANSID_LAST             = 0xC,
		};

		// MessageIDs extending SentientSM's message enum (base MESSAGE_LAST = 18).
		// UNVERIFIED: the enum tag is ours; the enumerator names and values are the original's.
		enum TurnSeekSMMessageID : uint32_t
		{
			MESSAGE_TURNTOTARGET                         = 0x12,
			MESSAGE_TURNTOTARGETPOS                      = 0x13,
			MESSAGE_TURNTOTARGET_NO_ANIMATIONS           = 0x14,
			MESSAGE_TURNTOTARGETPOS_NO_ANIMATIONS        = 0x15,
			MESSAGE_SETHEADINGTOTARGET                   = 0x16,
			MESSAGE_DRAWDEBUG                            = 0x17,
			MESSAGE_INIT_TURN_TO_POS_WITH_ANIMATED_ROT   = 0x18,
			MESSAGE_INIT_TURN_TO_POS_WITH_PROCEDURAL_ROT = 0x19,
			MESSAGE_DISABLE_IDLE_ANIMS                   = 0x1A,
			MESSAGE_LAST                                 = 0x1B,
		};

		// TODO: this class's members and its remaining methods (SetTarget and the turn/seek
		// helpers) are known but not reconstructed yet.  The shipping build is 0x84 where the
		// debug build is 0x9C, so the debug layout cannot be taken as-is; the padding stays until
		// the shipping layout is established.
		char m_Padding[0x2C];
	};
	static_assert(sizeof(TurnSeekStateMachine) == 0x84);
}
