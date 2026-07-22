#include "HashedNameRegistry.h"

// SDK
#include "SDK/EARS_Common/CommonTypes.h"

SH::HashedNameRegistry& SH::HashedNameRegistry::GetInstance()
{
	static HashedNameRegistry Instance;
	return Instance;
}

void SH::HashedNameRegistry::Register(uint32_t Hash, const char* Name)
{
	if (!Name || !*Name)
	{
		return;
	}

	// First non-empty name wins; skip the string construction for hashes we already know.
	Entry& Slot = m_Names[Hash];
	if (Slot.Name.empty())
	{
		Slot.Name = Name;
	}
}

uint32_t SH::HashedNameRegistry::RegisterName(const char* Name)
{
	const uint32_t Hash = EARS::Common::HashString_SDBM(Name);
	Register(Hash, Name);
	return Hash;
}

void SH::HashedNameRegistry::MarkAsMessage(uint32_t Hash)
{
	m_Names[Hash].bRegisteredMessage = true;
}

const char* SH::HashedNameRegistry::Find(uint32_t Hash) const
{
	const auto It = m_Names.find(Hash);
	return (It != m_Names.end() && !It->second.Name.empty()) ? It->second.Name.c_str() : nullptr;
}

bool SH::HashedNameRegistry::IsKnown(uint32_t Hash) const
{
	return Find(Hash) != nullptr;
}

bool SH::HashedNameRegistry::IsMessage(uint32_t Hash) const
{
	const auto It = m_Names.find(Hash);
	return (It != m_Names.end()) && It->second.bRegisteredMessage;
}
