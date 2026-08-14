#pragma once

// SDK
#include "SDK/EARS_Common/HashTable.h"

// C++
#include <stdint.h>

struct ImmutableStringPoolEntry
{
	uint32_t m_Hash = 0;
	ImmutableStringPoolEntry* m_HashNext = nullptr;
	int m_ReferenceCount = 0;
	char* m_SharedString = nullptr;
};


struct ImmutableString
{
	ImmutableStringPoolEntry* m_StringRef = nullptr;
};

enum MatParamType : int32_t
{
	MAT_PARAM_INVALID = -1,
	MAT_PARAM_FLOAT1 = 0x0,
	MAT_PARAM_FLOAT2 = 0x1,
	MAT_PARAM_FLOAT3 = 0x2,
	MAT_PARAM_FLOAT4 = 0x3,
	MAT_PARAM_MAT44 = 0x4,
	MAT_PARAM_TEX = 0x5,
	MAT_PARAM_TEX_FOUND = 0x6,
};

struct MatPass
{
	struct SM_Shader_Base* m_ShaderWrapper = nullptr;
	uint32_t m_Bind = 0;
};

struct MatParam
{
	union ParamValue
	{
		float value;
		float* pValues;
		unsigned int textureRef;
	};

	uint32_t m_ParamNameHash = 0;
	uint32_t m_ShaderHandle = 0;
	uint32_t m_ParamHandle = 0;
	uint8_t m_ParamType = 0;
	ParamValue m_ParamValue;

	char m_Padding[12];
};

struct Material
{
	uint32_t m_MatFlags = 0;
	float m_AlphaBlendConstant = 0.0f;
	MatPass pass[5];
	uint32_t m_NameHash = 0;
	uint32_t m_AlternativeNameHash = 0;
	uint8_t m_SurfaceType = 0;
	uint16_t m_ParamAllocSize = 0;
	uint16_t m_ValuesAllocSize = 0;
	uint8_t m_PassStart[5];
	uint8_t m_PassEnd[5];
	uint32_t m_ParamCount = 0;
	MatParam* m_ParamData = nullptr;
	float* m_Values = nullptr;
};

struct MaterialHashTableEntry
{
public:

	Material* GetMaterial() const { return m_Material; }

	uint32_t GetKey() const { return m_Hash; }
	MaterialHashTableEntry* GetHashNext() const { return m_HashNext; }
	void SetHashNext(MaterialHashTableEntry* NextEntry) { m_HashNext = NextEntry; }

private:

	int m_ReferenceCount; // actually AtomicInt
	int m_DelayedDecrementCount = 0; // actually AtomicInt
	Material* m_Material = nullptr;
	uint32_t m_Hash = 0;
	MaterialHashTableEntry* m_HashNext = nullptr;
};

struct MaterialHashTable
{
public:

	// just to simplify things
	typedef EARS::Common::IntrusiveHashTableFast<uint32_t, MaterialHashTableEntry, 8192> EmbeddedHashTable;

	MaterialHashTableEntry* Find(uint32_t InHash) const;

	EmbeddedHashTable::Iterator GetIterator();

	/** resolve game instance of the material table */
	static MaterialHashTable* GetInstance();

private:

	EmbeddedHashTable m_MaterialHashTable;
	char m_Padding[0x24]; // fast pool?
};

// static functions
extern Material* MatHash_FindMaterial(const uint32_t Hash);

/** required implementation for intrusive hash lookup system */
template<>
struct EARS::Common::GetKeyFunc<MaterialHashTableEntry, uint32_t>
{
public:

	static uint32_t GetKey(const MaterialHashTableEntry& Value)
	{
		return Value.GetKey();
	}
};

/** required implementation for intrusive hash lookup system */
template<>
struct EARS::Common::HashNext<MaterialHashTableEntry>
{
public:

	static MaterialHashTableEntry* GetHashNext(const MaterialHashTableEntry& Value)
	{
		return Value.GetHashNext();
	}

	static void SetHashNext(MaterialHashTableEntry& Value, MaterialHashTableEntry* Next)
	{
		Value.SetHashNext(Next);
	}
};