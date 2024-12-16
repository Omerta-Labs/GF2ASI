#pragma once

// Renderware
#include <SDK/EARS_Framework/Core/EventHandler/CEventHandler.h>

// TODO: When we get a manager we can probably move a lot of the achievement/trophy related stuff into said folder

// forward declares
namespace EARS
{
	namespace Modules
	{
		struct ScoreEventOperation;
	}
}

namespace Mod
{
	// Event to listen for specific achievement unlock
	// The IMsgData stores index to achievement, using original Xbox 360 list
	extern RWS::CEventId iMsgOnAchievementUnlocked;

	// Event to listen for specific achievement unlock
	// The IMsgData stores index to achievement, using original PS3 list
	extern RWS::CEventId iMsgOnTrophyUnlocked;

	// Initialise / Unitialise functions for Scripthook Events
	void ScripthookEvents_Open();
	void ScripthookEvents_Close();

	// Dispatch an RWS::CMsg for the Unlock operation triggered by the ScoreKeeper.
	// Can be listened to using iMsgOnAchievementUnlocked and iMsgOnTrophyUnlocked,
	// Then access the data using PlatformAgnosticUnlockData. Note that this pointer will not last beyond HandleEvents!
	void DispatchPlatformAgnosticUnlockEvent(const EARS::Modules::ScoreEventOperation& ScoreEventOp);

	// Data to use to access trophy or achievement data
	// Works with iMsgOnAchievementUnlocked and iMsgOnTrophyUnlocked
	struct PlatformAgnosticUnlockData
	{
	public:

		PlatformAgnosticUnlockData(const uint32_t InUnlockID)
			: UnlockID(InUnlockID)
		{
			// empty because we use initialiser list
		}

		// NB: While both of these functions return same index, its more for future proofing

		// Get the Unlock ID for Achievement message
		uint32_t GetAchievementID() const { return UnlockID; }

		// Get the Unlock ID for Trophy message
		uint32_t GetTrophyID() const { return UnlockID; }

	private:

		// this is either the Achievement ID or Trophy ID.
		uint32_t UnlockID = 0;
	};
}