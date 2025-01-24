#include "CAttributeHandler.h"

// Hooks
#include <Addons/Hook.h>

// Framework
#include "SDK/EARS_Framework/Core/Component/Component.h"

// CPP
#include <assert.h>

RWS::CAttributeHandler* RWS::CAttributePacketEntityList::GetNext() const
{
	assert(m_Head);

	return m_Head->m_NextHandlerFromPacket;
}

RWS::CAttributePacketEntityList::Iterator::Iterator(const RWS::CAttributePacketEntityList& InEntityList)
{
	m_EntityList = &InEntityList;
	m_CurrentHandler = InEntityList.GetFront();
}

RWS::CAttributePacketEntityList::Iterator& RWS::CAttributePacketEntityList::Iterator::operator++(int a1)
{
	m_CurrentHandler = m_CurrentHandler->m_NextHandlerFromPacket;
	return *this;
}

const char* RWS::CAttributeCommand::GetAs_char_ptr() const
{
	return MemUtils::CallClassMethod<const char*, const RWS::CAttributeCommand*>(0x43AB90, this);
}

EARS::Common::guid128_t* RWS::CAttributeCommand::GetAs_RWS_GUID() const
{
	return MemUtils::CallClassMethod<EARS::Common::guid128_t*, const RWS::CAttributeCommand*>(0x043ABC0, this);
}

bool RWS::CAttributeCommandIterator::IsFinished() const
{
	return MemUtils::CallClassMethod<bool, const RWS::CAttributeCommandIterator*>(0x043B120, this);
}

bool RWS::CAttributeCommandIterator::TestBit(uint32_t m_Idx) const
{
	return m_ZeroValueBitVec[m_Idx >> 3] & (1 << (m_Idx & 7));
}

void RWS::CAttributeCommandIterator::SeekTo(const uint32_t NewIdx)
{
	MemUtils::CallClassMethod<void, RWS::CAttributeCommandIterator*, uint32_t>(0x043AF00, this, NewIdx);
}

RWS::CAttributeDataChunkIterator& RWS::CAttributeCommandIterator::operator++(int a1)
{
	return MemUtils::CallClassMethod<RWS::CAttributeDataChunkIterator&, RWS::CAttributeCommandIterator*>(0x043B1A0, this);
}

const RWS::CAttributeCommand* RWS::CAttributeCommandIterator::operator->() const
{
	return MemUtils::CallClassMethod<const RWS::CAttributeCommand*, const RWS::CAttributeCommandIterator*>(0x043B210, this);
}

RWS::CAttributeCommandIterator RWS::CAttributeCommandIterator::MakeIterator(const RWS::CAttributePacket& InPacket, uint32_t InTargetClassID)
{
	RWS::CAttributeCommandIterator NewIterator = {};
	MemUtils::CallClassMethod<void, RWS::CAttributeCommandIterator*, const RWS::CAttributePacket&, uint32_t>(0x043AFF0, &NewIterator, InPacket, InTargetClassID);
	return NewIterator;
}

uint32_t RWS::CAttributePacket::GetIdOfClassToCreate() const
{
	return MemUtils::CallClassMethod<uint32_t, const CAttributePacket*>(0x043AAF0, this);
}

const EARS::Common::guid128_t& RWS::CAttributePacket::GetInstanceID() const
{
	return MemUtils::CallClassMethod<const EARS::Common::guid128_t&, const RWS::CAttributePacket*>(0x043AB10, this);
}

RWS::CAttributePacketEntityList::Iterator RWS::CAttributePacket::GetEntityIterator() const
{
	RWS::CAttributePacketEntityList::Iterator NewIt = RWS::CAttributePacketEntityList::Iterator(m_EntityList);
	return NewIt;
}

bool RWS::CAttributeHandler::HasAttributeHandlerFlag(const uint32_t InFlag) const
{
	const uint32_t Flags = GetAttributeHandlerFlags();
	if ((Flags & InFlag) != 0)
	{
		return true;
	}

	return false;
}

bool RWS::CAttributeHandler::HasComponents() const
{
	return (m_ComponentList != nullptr && m_Components != nullptr);
}

EARS::Framework::Component* RWS::CAttributeHandler::GetComponent(const uint32_t Index) const
{
	if (HasComponents())
	{
		return m_Components[m_ComponentList->GetIndex(Index)];
	}

	return nullptr;
}
