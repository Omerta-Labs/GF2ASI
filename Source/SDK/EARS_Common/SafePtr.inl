#pragma once

/**
 * This inline code file is for explicitly overriding the GetPtr functions.
 * I imagine once we have setup the class hierarchies properly this might not be required,
 * But for this its all we can do without causing code bloat.
 */

// Forward declaring here
namespace EARS
{
	namespace Framework
	{
		class Animated;
		class Entity;
	}

	namespace Modules
	{
		class Family;
		class Item;
		class SimNPC;
		class NPC;
	}

	namespace Vehicles
	{
		class WhiteboxCar;
	}
}

// Explicit defines here
// TODO: We could macro this
template<>
inline EARS::Framework::Entity* SafePtr<EARS::Framework::Entity>::GetPtr() const
{
	if (m_Obj)
	{
		return (EARS::Framework::Entity*)(m_Obj - 0x9);
	}

	return nullptr;
}

template<>
inline EARS::Framework::Animated* SafePtr<EARS::Framework::Animated>::GetPtr() const
{
	if (m_Obj)
	{
		return (EARS::Framework::Animated*)(m_Obj - 0x9);
	}

	return nullptr;
}

template<>
inline EARS::Modules::SimNPC* SafePtr<EARS::Modules::SimNPC>::GetPtr() const
{
	if (m_Obj)
	{
		return (EARS::Modules::SimNPC*)(m_Obj - 0x9);
	}

	return nullptr;
}

template<>
inline EARS::Modules::NPC* SafePtr<EARS::Modules::NPC>::GetPtr() const
{
	if (m_Obj)
	{
		return (EARS::Modules::NPC*)(m_Obj - 0x9);
	}

	return nullptr;
}

template<>
inline EARS::Vehicles::WhiteboxCar* SafePtr<EARS::Vehicles::WhiteboxCar>::GetPtr() const
{
	if (m_Obj)
	{
		return (EARS::Vehicles::WhiteboxCar*)(m_Obj - 0x9);
	}

	return nullptr;
}

template<>
inline EARS::Modules::Family* SafePtr<EARS::Modules::Family>::GetPtr() const
{
	if (m_Obj)
	{
		return (EARS::Modules::Family*)(m_Obj - 0x9);
	}

	return nullptr;
}

template<>
inline EARS::Modules::Item* SafePtr<EARS::Modules::Item>::GetPtr() const
{
	if (m_Obj)
	{
		return (EARS::Modules::Item*)(m_Obj - 0x9);
	}

	return nullptr;
}