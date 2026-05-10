#include "Family.h"

// Addons
#include "Addons/Hook.h"

// SDK
#include "SDK/EARS_Godfather/Modules/Buildings/BuildingManager.h"
#include "SDK/EARS_Godfather/Modules/Buildings/BuildingStore.h"
#include "SDK/EARS_Godfather/Modules/Families/MadeMan.h"

// C++
#include <assert.h>

void EARS::Modules::Family::AddMedic(EARS::Modules::Sentient* NewMedic)
{
	MemUtils::CallClassMethod<void, EARS::Modules::Family*, EARS::Modules::Sentient*>(0x08C3C80, this, NewMedic);
}

float EARS::Modules::Family::ModifyBalance(const float Modifier, const LedgerItemType LedgerType)
{
	return MemUtils::CallClassMethod<float, EARS::Modules::Family*, float, LedgerItemType>(0x08C09A0, this, Modifier, LedgerType);
}

uint32_t EARS::Modules::Family::GetAllyFamilyID(const uint32_t Index) const
{
	assert(m_NumAllyFamilies > Index && "Expected to be less than number of current allies!");

	return m_FamilyAllyID[Index];
}

bool EARS::Modules::Family::QuickRecovery() const
{
	bool bChanged = false;
	for (EARS::Modules::MadeMan* CurMadeMan : m_MadeMen)
	{
		bChanged |= QuickRecovery(*CurMadeMan);
	}

	return bChanged;
}

bool EARS::Modules::Family::QuickRecovery(EARS::Modules::MadeMan& InMadeMan) const
{
	if (InMadeMan.GetState() == MADE_MAN_STATE_IN_HOSPITAL)
	{
		InMadeMan.SetVenueID(0);
		InMadeMan.SetState(MADE_MAN_STATE_IDLE, 0.0f);
		return true;
	}

	return false;
}

bool EARS::Modules::Family::Jailbreak() const
{
	bool bChanged = false;
	for (EARS::Modules::MadeMan* CurMadeMan : m_MadeMen)
	{
		bChanged |= Jailbreak(*CurMadeMan);
	}

	return bChanged;
}

bool EARS::Modules::Family::Jailbreak(EARS::Modules::MadeMan& InMadeMan) const
{
	if (InMadeMan.GetState() == MADE_MAN_STATE_IN_JAIL)
	{
		InMadeMan.SetVenueID(0);
		InMadeMan.SetState(MADE_MAN_STATE_IDLE, 0.0f);
		return true;
	}

	return false;
}

void EARS::Modules::Family::HospitalizeMadeMan(EARS::Modules::SimNPC& InSimNPC) const
{
	const uint32_t MadeManIdx = FindMadeManIndex(InSimNPC);
	if (MadeManIdx != -1)
	{
		EARS::Modules::MadeMan* TargetMadeMan = m_MadeMen[MadeManIdx];
		if (TargetMadeMan->GetState() != MADE_MAN_STATE_ELIMINATED
			&& TargetMadeMan->GetState() != MADE_MAN_STATE_IN_HOSPITAL)
		{
			TargetMadeMan->ReleaseFromVenue(MADE_MAN_STATE_IDLE);

			const float Cooldown = TargetMadeMan->GetHospitalTime();
			TargetMadeMan->SetState(MADE_MAN_STATE_IN_HOSPITAL, Cooldown);
		}
	}
}

void EARS::Modules::Family::IncarcerateMadeMan(EARS::Modules::SimNPC& InSimNPC) const
{
	// TODO: POSSIBLE BUG - Shouldn't we also check if the Made Man isn't in Jail?

	const uint32_t MadeManIdx = FindMadeManIndex(InSimNPC);
	if (MadeManIdx != -1)
	{
		EARS::Modules::MadeMan* TargetMadeMan = m_MadeMen[MadeManIdx];
		if (TargetMadeMan->GetState() != MADE_MAN_STATE_ELIMINATED)
		{
			TargetMadeMan->ReleaseFromVenue(MADE_MAN_STATE_IDLE);

			const float Cooldown = TargetMadeMan->GetJailTime();
			TargetMadeMan->SetState(MADE_MAN_STATE_IN_JAIL, Cooldown);
		}
	}
}

void EARS::Modules::Family::ReviveMadeMan(EARS::Modules::SimNPC& InSimNPC) const
{
	const uint32_t MadeManIdx = FindMadeManIndex(InSimNPC);
	if (MadeManIdx != -1)
	{
		EARS::Modules::MadeMan* TargetMadeMan = m_MadeMen[MadeManIdx];
		if (TargetMadeMan->GetState() == MADE_MAN_STATE_ELIMINATED)
		{
			TargetMadeMan->SetState(MADE_MAN_STATE_IDLE, 0.0f);
			TargetMadeMan->SendToCompound();
		}
	}
}

void EARS::Modules::Family::ReviveFamily()
{
	if (!m_Flags.Test((uint32_t)FamilyFlags::FAMILY_HAS_BEEN_ELIMINATED))
	{
		return;	
	}

	// first of all make sure the family is back in the game
	m_Flags.Clear((uint32_t)FamilyFlags::FAMILY_HAS_BEEN_ELIMINATED);
	m_Flags.Clear((uint32_t)FamilyFlags::FAMILY_NO_STRATEGY_ACTIONS);

	// give back the compound
	BuildingManager* BuildingMgr = BuildingManager::GetInstance();
	if (BuildingStore* CompoundVenue = BuildingMgr->GetStoreByVenueID(GetCompoundVenueID()))
	{
		CompoundVenue->ChangeOwnership(GetFamilyID(), false, nullptr, false);
	}

	// then set all made men to alive
	for (EARS::Modules::MadeMan* CurrentMadeMan : m_MadeMen)
	{
		CurrentMadeMan->SetState(MADE_MAN_STATE_IDLE, 0.0f);
		CurrentMadeMan->SendToCompound();
	}
}

EARS::Modules::MadeMan* EARS::Modules::Family::GetMadeManByIndex(const uint32_t Index) const
{
	assert(m_MadeMen.Size() > Index && "Expected to be less than number of current size of m_MadeMen!");

	return m_MadeMen[Index];
}

int32_t EARS::Modules::Family::FindMadeManIndex(const EARS::Modules::SimNPC& InSimNPC) const
{
	for (uint32_t Idx = 0; Idx < m_MadeMen.Size(); Idx++)
	{
		if (m_MadeMen[Idx]->GetSimNPC() == &InSimNPC)
		{
			return Idx;
		}
	}

	return -1;
}

void EARS::Modules::Family::ForEachOmertaTable(const TVisitOmertaEntryFunctor& InFunction)
{
	for (OmertaEntry& Entry : m_OmertaTable)
	{
		InFunction(Entry);
	}
}

const String* EARS::Modules::Family::GetInternalName() const
{
	if (m_InternalName)
	{
		return &m_InternalName;
	}

	return nullptr;
}

const String* EARS::Modules::Family::GetSingularName() const
{
	if (m_SingularDisplayName)
	{
		return &m_SingularDisplayName;
	}

	return nullptr;
}

const String* EARS::Modules::Family::GetPluralName() const
{
	if (m_PluralDisplayName)
	{
		return &m_PluralDisplayName;
	}

	return nullptr;
}
