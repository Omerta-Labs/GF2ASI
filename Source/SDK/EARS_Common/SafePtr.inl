#pragma once

/**
 * This inline code file is for explicitly overriding the GetPtr functions.
 * I imagine once we have setup the class hierarchies properly this might not be required,
 * But for this its all we can do without causing code bloat.
 */

// Forward declaring here
namespace EARS
{
	namespace Modules
	{
		class SimNPC;

	}

	namespace Vehicles
	{
		class WhiteboxCar;
	}
}

// Explicit defines here
// TODO: We could macro this
template<>
inline EARS::Modules::SimNPC* SafePtr<EARS::Modules::SimNPC>::GetPtr() const
{
	return (EARS::Modules::SimNPC*)(m_Obj - 0x9);
}

template<>
inline EARS::Vehicles::WhiteboxCar* SafePtr<EARS::Vehicles::WhiteboxCar>::GetPtr() const
{
	return (EARS::Vehicles::WhiteboxCar*)(m_Obj - 0x9);
}
