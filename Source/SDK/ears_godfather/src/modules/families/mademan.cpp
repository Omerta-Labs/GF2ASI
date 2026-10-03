#include "mademan.h"

// SDK
#include "modules/families/family.h"
#include "modules/sentient/sentient_constants.h"
#include "modules/npcscheduling/simnpc.h"

// CPP
#include "Addons/Hook.h"

namespace EARS
{
    namespace Modules
    {
        void MadeMan::SetState(MadeManState DesiredState, const float CooldownSec)
        {
            MemUtils::CallClassMethod<void, MadeMan*, MadeManState, const float>(0x08C8A20, this, DesiredState, CooldownSec);
        }

        void MadeMan::SetVenueID(const uint32_t InVenueID)
        {
            MemUtils::CallClassMethod<void, MadeMan*, uint32_t>(0x08C80D0, this, InVenueID);
        }

        void MadeMan::ReleaseFromVenue(MadeManState DesiredState)
        {
            MemUtils::CallClassMethod<void, MadeMan*, MadeManState>(0x08C8A80, this, DesiredState);
        }

        void MadeMan::SendToCompound()
        {
            assert(m_Family && "A Made Man should have a reference to their Family during runtime gameplay");

            const uint32_t CompoundVenueID = m_Family->GetCompoundVenueID();
            if (m_VenueID != CompoundVenueID && m_State != MADE_MAN_STATE_ELIMINATED)
            {
                SetState(MADE_MAN_STATE_IDLE, 0.0f);
                SetVenueID(CompoundVenueID);
            }
        }

        EARS::Modules::SentientRank EARS::Modules::MadeMan::GetRank() const
        {
            assert(m_SimNPC && "Expects SimNPC");

            return m_SimNPC->GetRank();
        }

        float MadeMan::GetHospitalTime() const
        {
            assert(m_Family);
        
            return m_Family->GetMadeManHospitalTime() * GetHospitalTimeFactor();
        }

        float MadeMan::GetJailTime() const
        {
            assert(m_Family);

            return m_Family->GetMadeManJailTime() * GetJailTimeFactor();
        }

        const char* MadeMan::StateEnumToString(MadeManState State)
        {
            static const char* StringStates[9]
            {
                "HIDDEN",
                "IDLE",
                "IN_COMBAT",
                "IN_COOLDOWN",
                "IN_TRANSIT",
                "IN_HOSPITAL",
                "IN_JAIL",
                "ELIMINATED",
                "CREW_UI_ONLY",
            };

            return StringStates[State];
        }
    }
}
