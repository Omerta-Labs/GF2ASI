#include "PartedModelMgr.h"

void PartedModelMgr::ForEachAssemblyHeader(const TVisitAssemblyHeaderFunctor& InFunction) const
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

PartedModelMgr* PartedModelMgr::GetInstance()
{
	return *(PartedModelMgr**)0x112990C;
}
