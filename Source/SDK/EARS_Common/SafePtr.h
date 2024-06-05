#pragma once

struct SafeObj;
struct SafePtrBase;

struct SafePtrData
{
	const SafeObj* m_Obj = nullptr;
	SafePtrBase* m_Next = nullptr;
};

struct SafePtrBase : public SafePtrData
{
};

struct SafeObj
{
	void* __vftable = nullptr /*VFT*/;
	SafePtrBase* m_SafePtrList = nullptr;
};

template <typename T>
struct SafePtr : public SafePtrBase
{
};
