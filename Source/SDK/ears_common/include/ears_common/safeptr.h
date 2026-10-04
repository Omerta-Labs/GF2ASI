#pragma once

// CPP
#include <assert.h>
#include <type_traits>

struct SafeObj;
struct SafePtrBase;

struct SafePtrData
{
public:

	SafeObj* m_Obj = nullptr;
	SafePtrBase* m_Next = nullptr;
};

struct SafePtrBase : public SafePtrData
{
public:

	~SafePtrBase();

protected:

	void ClearSafePtr();

	void InitSafePtr(SafeObj* NewObj);
};

struct SafeObj
{
public:

	SafeObj();
	virtual ~SafeObj();

private:

	void AddSafePtr(SafePtrBase* InBase);

	void RemoveSafePtr(SafePtrBase* InBase);

	SafePtrBase* m_SafePtrList = nullptr;

	friend SafePtrBase;
};

template <typename T>
struct SafePtr : public SafePtrBase
{
public:

	T* GetPtr() const
	{
		assert(false && "This must be explicitly defined, otherwise you'll crash");
		return nullptr;
	}

	void Clear() { ClearSafePtr(); }

	bool IsValid() const { return m_Obj != nullptr; }

	/* operator overloads */
	SafePtr<T>& operator=(T* SafeObjPtr)
	{
		static_assert(std::is_base_of<SafeObj, T>::value, "Must be inheriting SafeObj");

		InitSafePtr(dynamic_cast<SafeObj*>(SafeObjPtr));

		return *this;
	}

private:

	
};

// Explicit GetPtr overrides, inlined here. The original had no .inl file:
// safeptr.h and safeptr.cpp are the only two in the manifest.
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
		class CustomCameraInfo;
		class Family;
		class Item;
		class SimNPC;
		class MarketingCamera;
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

template<>
inline EARS::Modules::CustomCameraInfo* SafePtr<EARS::Modules::CustomCameraInfo>::GetPtr() const
{
	if (m_Obj)
	{
		return (EARS::Modules::CustomCameraInfo*)(m_Obj - 0x9);
	}

	return nullptr;
}

template<>
inline EARS::Modules::MarketingCamera* SafePtr<EARS::Modules::MarketingCamera>::GetPtr() const
{
	if (m_Obj)
	{
		return (EARS::Modules::MarketingCamera*)(m_Obj);
	}

	return nullptr;
}
