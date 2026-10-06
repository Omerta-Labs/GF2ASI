#pragma once

#include "modules/player/playersm.h"

// Addons
#include "Platform/MemUtils.h"

namespace EARS::Modules
{
	class PlayerGroundSM : public EARS::Modules::PlayerSM
	{
	public:

		PlayerGroundSM() = delete;
		PlayerGroundSM(uint32_t TableID, EARS::StateMachineSys::StateMachineParams* SMParams);
		virtual ~PlayerGroundSM();

		//~ Begin PlayerSM Interface
		virtual uint32_t GetStateMachineID() const override { return 0x0F1A2C8FE; }
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
		//~ End PlayerSM Interface

		static EARS::StateMachineSys::StateMachine* S_PlayerGroundSM_FactoryFn(unsigned int InID, EARS::StateMachineSys::StateMachineParams* InSMParams);

		// State indices for this SM's state table, as passed to SMBuilder::AddState.
		// UNVERIFIED: the enum tag is ours; the enumerator names and values are the original's.
		enum PlayerGroundSMStateID : uint32_t
		{
			STATE_INIT            = 0,
			STATE_IDLE            = 1,
			STATE_IDLETURN        = 2,
			STATE_FLICKTURN       = 3,
			STATE_LOCOMOTE        = 4,
			STATE_LOCOMOTE180TURN = 5,
			STATE_FACETOFACE      = 6,
			STATE_DISABLED        = 7,
		};

	private:
		char m_Padding[0x6C];
	};
	static_assert(sizeof(PlayerGroundSM) == 0xE0);
}
