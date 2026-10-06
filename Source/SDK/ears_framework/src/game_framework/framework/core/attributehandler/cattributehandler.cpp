#include "cattributehandler.h"

// Hooks
#include <Platform/MemUtils.h>

// Framework
#include "framework/core/component/component.h"
#include "framework/core/simmanager/simmanager.h"

// CPP
#include <assert.h>

static hook::Type<RWS::IDArray> s_RuntimeIdArray(0x120E710);
static hook::Type<uint32_t> s_CurrentRuntimeID(0x110ABD4);

RWS::CAttributeHandler::~CAttributeHandler()
{
	if (HasComponents())
	{
		MemUtils::CallClassMethod<void, RWS::CAttributeHandler*, EARS::Framework::Component**>(0x0482990, this, m_Components);
	}

	if (IsManagedBySimManager())
	{
		EARS::Framework::SimManager* SimMgr = EARS::Framework::SimManager::GetInstance();
		SimMgr->Remove(this);
	}

	FreeRuntimeUID();
}

void RWS::CAttributeHandler::HandleAttributes(const RWS::CAttributePacket& InPacket)
{
	// Doesn't do anything in release
#if DEBUG
	RWS::CAttributeCommandIterator CommandIt = RWS::CAttributeCommandIterator(InPacket, 0x8A157691);
	while (!CommandIt.IsFinished())
	{
		if (CommandIt->GetAs_uint32()) // acts as boolean
		{
			m_FlagsAndID |= 0x80000000;
		}
		else
		{
			m_FlagsAndID &= 0x7FFFFFFF;
		}

		CommandIt++;
	}
#endif // DEBUG
}

void RWS::CAttributeHandler::HandleAttributesFromProxy(const RWS::CAttributePacket& InPacket)
{
	// Engine default is a no-op; concrete handlers override HandleAttributes instead.
}

void RWS::CAttributeHandler::EnableMessagesToComponents()
{
	m_ComponentList->EnableMessagesToComponents(m_Components);
}

void RWS::CAttributeHandler::DisableMessagesToComponents()
{
	m_ComponentList->DisableMessagesToComponents(m_Components);
}

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

uint32_t RWS::CAttributeCommand::GetCommandId() const
{
	if (IsCompact())
	{
		return m_CommandData.m_CommandID;
	}
	else
	{
		return m_Chunk.m_Type;
	}
}

bool RWS::CAttributeCommand::IsCompact() const
{
	// TODO Expose this just like original game
	return (m_CommandData.m_CompactTag == 0x25E3);
}

const char* RWS::CAttributeCommand::GetAs_char_ptr() const
{
	return MemUtils::CallClassMethod<const char*, const RWS::CAttributeCommand*>(0x43AB90, this);
}

EARS::Common::guid128_t* RWS::CAttributeCommand::GetAs_RWS_GUID() const
{
	return MemUtils::CallClassMethod<EARS::Common::guid128_t*, const RWS::CAttributeCommand*>(0x043ABC0, this);
}

RWS::CAttributeCommandIterator::CAttributeCommandIterator(const CAttributePacket& InPacket, const uint32_t InTargetClassID)
{
	// RWS::CAttributeCommandIterator::Init
	MemUtils::CallClassMethod<void, RWS::CAttributeCommandIterator*, const RWS::CAttributePacket&, uint32_t>(0x043AFF0, this, InPacket, InTargetClassID);
}

bool RWS::CAttributeCommandIterator::IsFinished() const
{
	return MemUtils::CallClassMethod<bool, const RWS::CAttributeCommandIterator*>(0x043B120, this);
}

uint32_t RWS::CAttributeCommandIterator::GetCommandID() const
{
	if (m_bIsCompact)
	{
		return m_CurIdx;
	}
	else
	{
		// TODO: Complete this code. We don't run it in GF2 ever.
		assert(false);
		return 0;
	}
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

uint32_t RWS::CAttributePacket::GetIdOfClassToCreate() const
{
	return MemUtils::CallClassMethod<uint32_t, const CAttributePacket*>(0x043AAF0, this);
}

const EARS::Framework::EntityPacket* RWS::CAttributePacket::EntPacket() const
{
	// TODO: EARS ASSERT
	assert(IsCompact());

	return reinterpret_cast<const EARS::Framework::EntityPacket*>(&m_Flags);
}

const EARS::Common::guid128_t& RWS::CAttributePacket::GetInstanceId() const
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

void* RWS::CAttributeHandler::operator new(size_t Size, size_t AdditionalSize)
{
	EARS::Framework::SimManager* SimMgr = EARS::Framework::SimManager::GetInstance();
	EA::TagValuePair Pair = EA::TagValuePair(2u, 16);

	return SimMgr->GetBaseAllocator()->Alloc(Size + AdditionalSize, Pair);
}

void RWS::CAttributeHandler::operator delete(void* pointer, size_t size)
{
	EARS::Framework::SimManager* SimMgr = EARS::Framework::SimManager::GetInstance();
	SimMgr->GetBaseAllocator()->Free(pointer, 0);
}

void RWS::CAttributeHandler::FreeRuntimeUID()
{
	if (IsRuntimeIDValid())
	{
		s_CurrentRuntimeID = (m_FlagsAndID & 0xFFF);
		s_RuntimeIdArray->Clear(s_CurrentRuntimeID);
		m_FlagsAndID &= 0xEFFFF000;
	}
}
