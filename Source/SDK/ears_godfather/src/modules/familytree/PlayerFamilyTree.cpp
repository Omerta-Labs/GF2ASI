#include "PlayerFamilyTree.h"

// Hooks
#include "Addons/Hook.h"

// SDK
#include "SDK/ears_godfather/src/modules/npc/NPC.h"
#include "SDK/ears_godfather/src/modules/npcscheduling/SimNPC.h"
#include "SDK/ears_godfather/src/modules/ui/UIHud.h"

// CPP
#include <bitset>

namespace EARS
{
	namespace Modules
	{
		PlayerFamilyTree::FamilyTreeSlot PlayerFamilyTree::FindTreeSlotIndex(const SimNPC* InSimNPC) const
		{
			// NB: This is completed implementation of the engine function

			constexpr uint32_t MAX_SLOTS = (uint32_t)PlayerFamilyTree::FamilyTreeSlot::FAMILYTREE_NUM_SLOTS;
			for (uint32_t i = 0; i < MAX_SLOTS; i++)
			{
				const PlayerFamilyMember& Member = m_FamilyMembers[i];
				if (Member.IsSlotFilled() && Member.GetSimNPC() == InSimNPC)
				{
					return (FamilyTreeSlot)i;
				}
			}

			return FamilyTreeSlot::FAMILYTREE_SLOT_INVALID;
		}

		void PlayerFamilyTree::SetCurrentTreeType(const FamilyTreeType InTreeType) const
		{
			MemUtils::CallClassMethod<void, const PlayerFamilyTree*, FamilyTreeType>(0x90B8E0, this, InTreeType);
		}

		bool PlayerFamilyTree::ExpandToFit(EARS::Modules::SentientRank InRank)
		{
			return MemUtils::CallClassMethod<bool, PlayerFamilyTree*, SentientRank>(0x090B690, this, InRank);
		}

		bool PlayerFamilyTree::AddFamilyMember(EARS::Modules::SentientRank InRank, EARS::Modules::SimNPC* InSimNPC, uint32_t InSpecialties, FamilyTreeSlot InTreeSlot, EARS::Common::guid128_t* InWeaponGuid)
		{
			return MemUtils::CallClassMethod<bool, PlayerFamilyTree*, SentientRank, SimNPC*, uint32_t, FamilyTreeSlot, EARS::Common::guid128_t*>(0x090B4E0, this, InRank, InSimNPC, InSpecialties, InTreeSlot, InWeaponGuid);
		}

		bool PlayerFamilyTree::RemoveFamilyMember(PlayerFamilyTree::FamilyTreeSlot InSlotIndex, bool bUpdateFamily)
		{
			return MemUtils::CallClassMethod<bool, PlayerFamilyTree*, FamilyTreeSlot, bool>(0x090B5D0, this, InSlotIndex, bUpdateFamily);
		}

		void PlayerFamilyTree::ForEachMember(const TVisitFamilyMemberFunctor& InFunction)
		{
			constexpr uint32_t MAX_SLOTS = (uint32_t)PlayerFamilyTree::FamilyTreeSlot::FAMILYTREE_NUM_SLOTS;
			for (uint32_t i = 0; i < MAX_SLOTS; i++)
			{
				InFunction(m_FamilyMembers[i]);
			}
		}

		PlayerFamilyTree* PlayerFamilyTree::GetInstance()
		{
			return *(PlayerFamilyTree**)0x112989C;
		}

		bool PlayerFamilyMember::HasSpecialty(const Specialties NewSpeciality) const
		{
			std::bitset<32> SpecialityBits = m_Specialties;
			return SpecialityBits.test((uint32_t)NewSpeciality);
		}

		void PlayerFamilyMember::AddSpecialty(const Specialties NewSpeciality)
		{
			std::bitset<32> SpecialityBits = m_Specialties;
			SpecialityBits[(uint32_t)NewSpeciality] = 1;
			m_Specialties = (uint32_t)SpecialityBits.to_ulong();

			OnSpecialitiesUpdated();
		}

		void PlayerFamilyMember::ToggleSpecialty(const Specialties NewSpeciality)
		{
			std::bitset<32> SpecialityBits = m_Specialties;
			SpecialityBits[(uint32_t)NewSpeciality].flip();
			m_Specialties = (uint32_t)SpecialityBits.to_ulong();

			OnSpecialitiesUpdated();
		}

		void PlayerFamilyMember::RemoveSpecialty(const Specialties Speciality)
		{
			std::bitset<32> SpecialityBits = m_Specialties;
			SpecialityBits[(uint32_t)Speciality] = 0;
			m_Specialties = (uint32_t)SpecialityBits.to_ulong();

			OnSpecialitiesUpdated();
		}

		bool PlayerFamilyMember::IsSlotFilled() const
		{
			return (TestPlayerFamilyMemberFlags((uint32_t)Flags::FLAG_FAMILYMEMBER_SLOT_FILLED) == true);
		}

		bool PlayerFamilyMember::IsSlotUnlocked() const
		{
			return (TestPlayerFamilyMemberFlags((uint32_t)Flags::FLAG_FAMILYMEMBER_SLOT_UNLOCKED) == true);
		}

		void PlayerFamilyMember::JoinCrew()
		{
			MemUtils::CallClassMethod<void, PlayerFamilyMember*>(0x090A660, this);
		}

		void PlayerFamilyMember::LeaveCrew()
		{
			MemUtils::CallClassMethod<void, PlayerFamilyMember*>(0x090A700, this);
		}

		void PlayerFamilyMember::SetSpecialties(const uint32_t Specialties)
		{
			MemUtils::CallClassMethod<void, PlayerFamilyMember*, uint32_t>(0x090A610, this, m_Specialties);
		}

		void PlayerFamilyMember::OnSpecialitiesUpdated()
		{
			// TODO: If Medic, Init medic data? (TODO: How do we validate this?)

			SetSpecialties(m_Specialties);

			// TODO: This works for now, by forcing the UIHUD to remove then immediately add onto the list
			// This ensures that the specialty update is reflected appropriately.
			// This should only have an effect if the NPC is currently hired by the Player
			if (const SimNPC* const SimulationNPC = m_SimNPC.GetPtr())
			{
				if (NPC* const CrewNPC = SimulationNPC->GetNPC())
				{
					EARS::Apt::UIHUD* HUD = EARS::Apt::UIHUD::GetInstance();
					HUD->RemoveCrew(CrewNPC);
					HUD->AddCrew(CrewNPC);
				}
			}
		}

		bool PlayerFamilyMember::TestPlayerFamilyMemberFlags(uint32_t Flag) const
		{
			return m_Flags.Test(Flag);
		}
	} // Modules
} // EARS
