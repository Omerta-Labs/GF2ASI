#include "PartedModelMgr.h"

void EARS::Modules::PartedModelMgr::ForEachAssemblyHeader(const TVisitAssemblyHeaderFunctor& InFunction) const
{
	PartedModel::AssemblyListHeader* CurrentHeader = m_AssembliesList;
	while (CurrentHeader)
	{
		// Trigger function on assembly
		InFunction(*CurrentHeader);

		// Iterate to next header
		CurrentHeader = CurrentHeader->m_NextHeader;
	}
}

EARS::Modules::PartedModelMgr* EARS::Modules::PartedModelMgr::GetInstance()
{
	return *(EARS::Modules::PartedModelMgr**)0x112990C;
}
