#include "CorleoneData.h"

void EARS::Modules::CorleoneFamilyData::ForEachHonourData(const TVisitHonorDataFunctor& InFunction) const
{
	for (uint32_t i = 0; i < m_Honors.Size(); i++)
	{
		const CorleoneFamilyData::HonorData& HonourData = m_Honors[i];
		InFunction(HonourData);
	}
}

EARS::Modules::CorleoneFamilyData* EARS::Modules::CorleoneFamilyData::GetInstance()
{
	return *(CorleoneFamilyData**)0x1129908;
}
