#pragma once

// SDK Godfather
#include "SDK/ears_godfather/src/modules/sentient/sentient_constants.h"

// C++
#include <stdint.h>

namespace EARS
{
	namespace Modules
	{
		// forward declares
		class Family;
		class SimNPC;

		enum MadeManState : uint32_t
		{
			MADE_MAN_STATE_HIDDEN = 0x0,
			MADE_MAN_STATE_IDLE = 0x1,
			MADE_MAN_STATE_IN_COMBAT = 0x2,
			MADE_MAN_STATE_IN_COOLDOWN = 0x3,
			MADE_MAN_STATE_IN_TRANSIT = 0x4,
			MADE_MAN_STATE_IN_HOSPITAL = 0x5,
			MADE_MAN_STATE_IN_JAIL = 0x6,
			MADE_MAN_STATE_ELIMINATED = 0x7,
			MADE_MAN_STATE_CREW_UI_ONLY = 0x8,
		};

		class MadeMan
		{
		public:

			/** Request a new state for the Made Man */
			void SetState(MadeManState DesiredState, const float CooldownSec);

			/** Request that a MadeMan moves to another VenueID */
			void SetVenueID(const uint32_t InVenueID);

			/** Request that the Made Man is released from the venue */
			void ReleaseFromVenue(MadeManState DesiredState);

			/** Request that the MadeMan is sent back to the family compound */
			void SendToCompound();

			/** Get rank from the SimNPC (not stored on Made Man) */
			SentientRank GetRank() const;

			/** Calculate how much hospital time this Made Man would have */
			float GetHospitalTime() const;

			/** Calculate how much jail time this Made Man would have */
			float GetJailTime() const;

			// simple getters
			EARS::Modules::SimNPC* GetSimNPC() const { return m_SimNPC; }
			MadeManState GetState() const { return m_State; }
			uint32_t GetVenueID() const { return m_VenueID; }
			float GetCountdown() const { return m_CountdownSec; }
			float GetHospitalTimeFactor() const { return m_HospitalTimeFactor; }
			float GetJailTimeFactor() const { return m_JailTimeFactor; }

			/** Static function to convert MadeManState enum to display string */
			static const char* StateEnumToString(MadeManState State);

		private:

			void* vtable;

			MadeManState m_State = MadeManState::MADE_MAN_STATE_IDLE;
			EARS::Modules::SimNPC* m_SimNPC = nullptr;
			EARS::Modules::Family* m_Family = nullptr;
			void* m_HitMission = nullptr;
			float m_CountdownSec = 0.0f;
			uint32_t m_VenueID = 0;
			uint32_t m_UpdateCount = 0;
			float m_HospitalTimeFactor = 0.0f;
			float m_JailTimeFactor = 0.0f;
			float m_WitnessChanceFactor = 0.0f;
		};

		static_assert(sizeof(EARS::Modules::MadeMan) == 0x2C, "Expected EARS::Modules::MadeMan to have a size of 0x2C");

	} // Modules
} // EARS
