#pragma once

#include "SDK/ears_common/include/ears_common/bitflags.h"
#include "SDK/ears_common/include/ears_common/singleton.h"
#include "SDK/ears_framework/src/game_framework/framework/core/eventhandler/ceventhandler.h"
#include "SDK/ears_godfather/src/modules/player/playersm.h"

namespace EARS::Modules
{
	/**
	 * A reimplementation of the debug options system found in the Xbox debug version of the game
	 * This doesn't exist in the PC version, so this 
	 */
	class PlayerDebugOptions : public Singleton<PlayerDebugOptions>, RWS::CEventHandler
	{
	public:

		PlayerDebugOptions();
		virtual ~PlayerDebugOptions();

		void SetIsInDebugFly(const bool bInDebugFly);
		bool IsInDebugFly() const;

		static PlayerDebugOptions* GetInstance();

	private:

		enum class DebugFlyOptions : int32_t
		{
			DEBUGFLY_OFF = 0x0,
			DEBUGFLY_COLLIDE = 0x1,
			DEBUGFLY_NOCOLLIDE = 0x2,
			// DEBUGFLY_COLLIDE_2NDCONTROLLER = 0x3, <- won't be implemented
		};

		DebugFlyOptions m_DebugFlyOptions = DebugFlyOptions::DEBUGFLY_OFF;

		Flags32 m_Flags; // <- xbox says 0x2080 for offset
	};

	/**
	 * A reimplementation of the 'PlayerDebugFlySM' StateMachine found in Godfather II dev builds.
	 * To make this work and included in the PlayerMasterSM, you need to inject some code into the SMBuilder.
	 * See HookMods.cpp for more, but this is generally outside of the SDK remit as it is still theoretically mod code.
	 */
	class PlayerDebugFlySM : public EARS::Modules::PlayerSM
	{
	public:

		PlayerDebugFlySM() = delete;
		PlayerDebugFlySM(unsigned int TableID, EARS::StateMachineSys::StateMachineParams* SmParams);
		virtual ~PlayerDebugFlySM();

		//~ Begin EARS::Modules::PlayerSM interface
		virtual uint32_t GetStateMachineID() const override;
		virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData) override;
		virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, EARS::StateMachineSys::Transition::TransitionData* TransData) override;
		//~ End  EARS::Modules::PlayerSM interface

		static void BuildStateMachine();

	private:

		void ProcessMovement(float FrameTime);
	};
}
