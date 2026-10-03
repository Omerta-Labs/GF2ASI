#include "controllermanager.h"

#include "Platform/MemUtils.h"

namespace
{
	/**
	 * g_ConfigMap - the process-wide registry of loaded CECs, keyed by config name hash.
	 *
	 * It is a RegArr<ControllerEventConfig*, uint>: a flat array of {key, value} pairs kept
	 * sorted ascending by key, so lookups can binary-search it. Capacity is fixed at
	 * ControllerManager::MAX_CONFIG_RES entries.
	 */
	struct RegData
	{
		uint32_t m_Key;
		EARS::Modules::ControllerEventConfig* m_Value;
	};

	struct ConfigRegArr
	{
		RegData* m_Data;
		int32_t m_Size;
		int32_t m_Capacity;
	};

	ConfigRegArr* GetConfigMap()
	{
		return *reinterpret_cast<ConfigRegArr**>(0x1205400);
	}

	/* RegArr::Insert. Returns a pointer to the value slot for the new key, or null when
	 * the key is already present. Compiled as a free function against the global map, so
	 * it takes no this pointer. (game: 0x00466930) */
	EARS::Modules::ControllerEventConfig** RegArrInsert(const uint32_t* pKey)
	{
		return MemUtils::CallStdMethod<EARS::Modules::ControllerEventConfig**, const uint32_t*>(0x466930, pKey);
	}

	/* RegArr::FindIndex. Negative when the key is not present. (game: 0x00466B00) */
	int32_t RegArrFindIndex(ConfigRegArr* pMap, const uint32_t* pKey)
	{
		return MemUtils::CallClassMethod<int32_t, ConfigRegArr*, const uint32_t*>(0x466B00, pMap, pKey);
	}

	/* RegArr::Remove by index. The retail build passes both operands in registers, which
	 * happens to match __fastcall. (game: 0x00466AB0) */
	void RegArrRemoveAt(ConfigRegArr* pMap, int32_t Index)
	{
		using RemoveFn = void(__fastcall*)(int32_t, ConfigRegArr*);
		reinterpret_cast<RemoveFn>(0x466AB0)(Index, pMap);
	}
}

namespace EARS
{
	namespace Modules
	{
		bool ControllerManager::AddConfigResource(ControllerEventConfig* pConfig)
		{
			if (pConfig == nullptr)
			{
				return false;
			}

			const uint32_t NameHash = pConfig->GetNameHash();

			ControllerEventConfig** ppSlot = RegArrInsert(&NameHash);
			if (ppSlot == nullptr)
			{
				// Already registered under this hash.
				return false;
			}

			*ppSlot = pConfig;
			return true;
		}

		bool ControllerManager::RemoveConfigResource(ControllerEventConfig* pConfig)
		{
			if (pConfig == nullptr)
			{
				return false;
			}

			ConfigRegArr* pMap = GetConfigMap();
			if (pMap == nullptr)
			{
				return false;
			}

			const uint32_t NameHash = pConfig->GetNameHash();

			const int32_t Index = RegArrFindIndex(pMap, &NameHash);
			if (Index < 0)
			{
				return false;
			}

			// Still pushed on some controller's stack - unregistering now would leave a
			// dangling entry on that stack.
			if (pConfig->GetRefCount() != 0)
			{
				return false;
			}

			RegArrRemoveAt(pMap, Index);
			return true;
		}

		bool ControllerManager::PushConfiguration(uint8_t ControllerID, const char* ConfigName, bool bKeepLastFrameBitmask)
		{
			return MemUtils::CallClassMethod<bool, ControllerManager*, uint8_t, const char*, bool>(0x464960, this, ControllerID, ConfigName, bKeepLastFrameBitmask);
		}

		bool ControllerManager::PopConfiguration(uint8_t ControllerID)
		{
			return MemUtils::CallClassMethod<bool, ControllerManager*, uint8_t>(0x464A10, this, ControllerID);
		}

		const char* ControllerManager::GetTopConfigName(uint8_t ControllerID) const
		{
			if (ControllerID >= MAX_CONTROLLERS)
			{
				return nullptr;
			}

			const ControllerEventConfig* pConfig = m_CMI[ControllerID].ctrlEventConfigStack.top();
			return (pConfig != nullptr) ? pConfig->GetName() : nullptr;
		}

		const char* ControllerManager::GetConfigNameFromRWSGUID(const EARS::Common::guid128_t* pGUID) const
		{
			return MemUtils::CallClassMethod<const char*, const ControllerManager*, const EARS::Common::guid128_t*>(0x464930, this, pGUID);
		}

		bool ControllerManager::IsEventHappening(uint8_t ControllerID, uint32_t EventHash)
		{
			return MemUtils::CallClassMethod<bool, ControllerManager*, uint8_t, uint32_t>(0x4657D0, this, ControllerID, EventHash);
		}

		bool ControllerManager::IsEventJustTriggered(uint8_t ControllerID, uint32_t EventHash)
		{
			return MemUtils::CallClassMethod<bool, ControllerManager*, uint8_t, uint32_t>(0x4657F0, this, ControllerID, EventHash);
		}

		int ControllerManager::GetEventList(uint8_t ControllerID, uint32_t EventHash, int* pOutList, int MaxListSize) const
		{
			if (ControllerID >= MAX_CONTROLLERS || pOutList == nullptr || MaxListSize <= 0)
			{
				return 0;
			}

			const ControllerEventConfig* pConfig = m_CMI[ControllerID].ctrlEventConfigStack.top();
			if (pConfig == nullptr)
			{
				return 0;
			}

			const int ConfigSize = static_cast<int>(pConfig->GetConfigSize());
			if (ConfigSize <= 0)
			{
				return 0;
			}

			// The descriptor array is sorted ascending by eventHashID, so bisect for the
			// first entry that is not below the hash we want.
			int Low = -1;
			int High = ConfigSize;

			while (Low + 1 != High)
			{
				const int Mid = (Low + High) >> 1;

				if (pConfig->GetEventDesc(static_cast<uint16_t>(Mid)).eventHashID >= EventHash)
				{
					High = Mid;
				}
				else
				{
					Low = Mid;
				}
			}

			// Duplicate hashes are legal and sit next to each other - collect the whole run.
			int NumFound = 0;

			for (int Index = High; Index < ConfigSize && NumFound < MaxListSize; ++Index)
			{
				if (pConfig->GetEventDesc(static_cast<uint16_t>(Index)).eventHashID != EventHash)
				{
					break;
				}

				pOutList[NumFound++] = Index;
			}

			return NumFound;
		}

		float ControllerManager::GetStickMag(uint8_t ControllerID, eStickIndex StickIndex, bool bRemapped) const
		{
			return MemUtils::CallClassMethod<float, const ControllerManager*, uint8_t, eStickIndex, bool>(0x465CA0, this, ControllerID, StickIndex, bRemapped);
		}

		float ControllerManager::GetStickHeading(uint8_t ControllerID, eStickIndex StickIndex, bool bRemapped) const
		{
			return MemUtils::CallClassMethod<float, const ControllerManager*, uint8_t, eStickIndex, bool>(0x465CE0, this, ControllerID, StickIndex, bRemapped);
		}

		bool ControllerManager::IsButtonHeld(uint8_t ControllerID, eButtonType ButtonType) const
		{
			return MemUtils::CallClassMethod<bool, const ControllerManager*, uint8_t, eButtonType>(0x466240, this, ControllerID, ButtonType);
		}

		bool ControllerManager::CheckButtonActivity(uint8_t ControllerID, eButtonType ButtonType, uint8_t Status) const
		{
			return MemUtils::CallClassMethod<bool, const ControllerManager*, uint8_t, eButtonType, uint8_t>(0x4660F0, this, ControllerID, ButtonType, Status);
		}

		void ControllerManager::UpdateAll()
		{
			MemUtils::CallClassMethod<void, ControllerManager*>(0x464B80, this);
		}

		ControllerManager* ControllerManager::GetInstance()
		{
			return *reinterpret_cast<ControllerManager**>(0x1223464);
		}
	} // Modules
} // EARS
