#include "demographicregion.h"

// GF2
#include "framework/core/simmanager/simmanager.h"


void EARS::Modules::DemographicRegion::ApplyUserSettings()
{
	// NPCs
	m_NPCFilters[0].m_MaxCount = 500;
	m_NPCFilters[1].m_MaxCount = 500;
	m_NPCFilters[2].m_MaxCount = 500;

	// Cars
	m_VehicleFilters[0].m_MaxParkedCount = 500;
	m_VehicleFilters[1].m_MaxParkedCount = 500;
	m_VehicleFilters[2].m_MaxParkedCount = 500;
	m_VehicleFilters[0].m_MaxCivilianCount = 500;
	m_VehicleFilters[1].m_MaxCivilianCount = 500;
	m_VehicleFilters[2].m_MaxCivilianCount = 500;
}

std::string EARS::Modules::DemographicRegion::GetDebugName() const
{
	EARS::Framework::SimManager* SimMgr = EARS::Framework::SimManager::GetInstance();

	const EARS::Common::guid128_t LocalGuid = InqInstanceID();
	if(const RWS::CAttributePacket* Packet = SimMgr->GetAttributePacket(&LocalGuid, 0))
	{
		RWS::CAttributeCommandIterator PacketIt = RWS::CAttributeCommandIterator(*Packet, 0x84BF4579);
		PacketIt.SeekTo(0);

		return PacketIt->GetAs_char_ptr();
	}

	return {};
}
