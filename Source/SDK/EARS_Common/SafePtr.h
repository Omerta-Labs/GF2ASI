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

	virtual ~SafeObj() = 0;

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

#include "SafePtr.inl"
