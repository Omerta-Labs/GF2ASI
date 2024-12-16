#include "ScripthookEvents.h"

// Hook
#include "Addons/Hook.h"

// Scripthook
#include "Scripthook/ScripthookEvents.h"

// GF2
#include <SDK/EARS_Godfather/Modules/Scoring/ScoreEvent.h>

namespace Mod
{
	RWS::CEventId iMsgOnAchievementUnlocked;
	RWS::CEventId iMsgOnTrophyUnlocked;

	// private function to determine score event using Score OpCode
	bool IsScoreEventAnUnlockable(const EARS::Modules::ScoreEventOperation::OpCode InOpCode)
	{
		return (InOpCode == EARS::Modules::ScoreEventOperation::OpCode::OPCODE_SIMPLE_UNLOCK_ACHIEVEMENT
			|| InOpCode == EARS::Modules::ScoreEventOperation::OpCode::OPCODE_SIMPLE_UNLOCK_TROPHY);
	}
}

void Mod::ScripthookEvents_Open()
{
	MemUtils::CallCdeclMethod<void, RWS::CEventId*, const char*>(0x0408240, &iMsgOnAchievementUnlocked, "iMsgOnAchievementUnlocked");
	MemUtils::CallCdeclMethod<void, RWS::CEventId*, const char*>(0x0408240, &iMsgOnTrophyUnlocked, "iMsgOnTrophyUnlocked");
}

void Mod::ScripthookEvents_Close()
{
	// TODO:
	// Find unregister functions
}

void Mod::DispatchPlatformAgnosticUnlockEvent(const EARS::Modules::ScoreEventOperation& ScoreEventOp)
{
	if (Mod::IsScoreEventAnUnlockable(ScoreEventOp.m_Opcode))
	{
		const uint32_t UnlockID = reinterpret_cast<uint32_t>(ScoreEventOp.m_Data);
		Mod::PlatformAgnosticUnlockData* MsgData = new Mod::PlatformAgnosticUnlockData(UnlockID);

		RWS::CMsg MsgToSend;
		if (ScoreEventOp.m_Opcode == EARS::Modules::ScoreEventOperation::OpCode::OPCODE_SIMPLE_UNLOCK_ACHIEVEMENT)
		{
			MsgToSend = RWS::CMsg(Mod::iMsgOnAchievementUnlocked, MsgData);
		}
		else if (ScoreEventOp.m_Opcode == EARS::Modules::ScoreEventOperation::OpCode::OPCODE_SIMPLE_UNLOCK_TROPHY)
		{
			MsgToSend = RWS::CMsg(Mod::iMsgOnTrophyUnlocked, MsgData);
		}

		MemUtils::CallCdeclMethod<bool, const RWS::CMsg&, bool>(0x0408A00, MsgToSend, false);
		delete MsgData;
	}
}

