#include "CorleoneData.h"

EARS::Modules::CorleoneFamilyData::HonorData::HonorData(const HonorData& OtherData)
	: m_SimNpcGuid(OtherData.m_SimNpcGuid)
	, m_WeaponLicenseLevel(OtherData.m_WeaponLicenseLevel)
	, m_Unused(OtherData.m_Unused)
{
	for (uint32_t i = 0; i < 12; i++)
	{
		m_HonorTotals[i] = OtherData.m_HonorTotals[i];
	}
}

EARS::Modules::CorleoneFamilyData::HonorData::HonorData()
	: m_SimNpcGuid({})
	, m_WeaponLicenseLevel(MIN_WEAPON_LICENSE)
	, m_Unused(0)
{
	for (uint32_t i = 0; i < 12; i++)
	{
		m_HonorTotals[i] = 0;
	}
}

EARS::Modules::CorleoneFamilyData::HonorData::HonorData(const EARS::Common::guid128_t& InSimNPC, const uint8_t InWeaponLicenseLevel)
	: m_SimNpcGuid(InSimNPC)
	, m_WeaponLicenseLevel(InWeaponLicenseLevel)
	, m_Unused(0)
{
	for (uint32_t i = 0; i < 12; i++)
	{
		m_HonorTotals[i] = 0;
	}
}

uint8_t EARS::Modules::CorleoneFamilyData::GetWeaponLicense(const EARS::Common::guid128_t& SimNpcGuid) const
{
	const int32_t HonourIndex = FindHonourData(SimNpcGuid);
	if (HonourIndex != -1)
	{
		return m_Honors[HonourIndex].m_WeaponLicenseLevel;
	}

	return MIN_WEAPON_LICENSE;
}

void EARS::Modules::CorleoneFamilyData::SetWeaponLicense(const EARS::Common::guid128_t& SimNPCGuid, const uint8_t NewWeaponLevel)
{
	const int32_t HonourIndex = FindHonourData(SimNPCGuid);
	if (HonourIndex != -1)
	{
		m_Honors[HonourIndex].m_WeaponLicenseLevel = NewWeaponLevel;
	}
	else
	{
		// TODO: Check if this actually works as intended because i'm not sure tbh
		//const CorleoneFamilyData::HonorData NewHonorData = CorleoneFamilyData::HonorData(SimNPCGuid, NewWeaponLevel);
		//m_Honors.Add(NewHonorData);
	}
}

void EARS::Modules::CorleoneFamilyData::ForEachHonourData(const TVisitHonorDataFunctor& InFunction)
{
	for (uint32_t i = 0; i < m_Honors.Size(); i++)
	{
		CorleoneFamilyData::HonorData& HonourData = m_Honors[i];
		InFunction(HonourData);
	}
}

EARS::Modules::CorleoneFamilyData* EARS::Modules::CorleoneFamilyData::GetInstance()
{
	return *(CorleoneFamilyData**)0x1129908;
}

int32_t EARS::Modules::CorleoneFamilyData::FindHonourData(const EARS::Common::guid128_t& SimNpcGuid) const
{
	if (m_Honors.IsEmpty() == false)
	{
		for(int32_t i = 0; i < (int32_t)m_Honors.Size(); i++)
		{
			if (m_Honors[i].m_SimNpcGuid == SimNpcGuid)
			{
				return i;
			}
		}
	}

	return -1;
}
