#include "MaterialHash.h"

MaterialHashTableEntry* MaterialHashTable::Find(uint32_t InHash) const
{
	return m_MaterialHashTable.FindEntry(InHash);
}

MaterialHashTable::EmbeddedHashTable::Iterator MaterialHashTable::GetIterator()
{
	return m_MaterialHashTable.CreateIterator();
}

MaterialHashTable* MaterialHashTable::GetInstance()
{
	return *(MaterialHashTable**)0x12054AC;
}

Material* MatHash_FindMaterial(const uint32_t Hash)
{
	if (Hash == 0)
	{
		return nullptr;
	}

	if (const MaterialHashTableEntry* FoundEntry = MaterialHashTable::GetInstance()->Find(Hash))
	{
		return FoundEntry->GetMaterial();
	}

	return nullptr;
}
