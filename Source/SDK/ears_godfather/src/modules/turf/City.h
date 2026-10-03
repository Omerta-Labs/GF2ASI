#pragma once

// SDK (Common)
#include "SDK/ears_common/include/ears_common/Array.h"
#include "SDK/ears_common/include/ears_common/Bitflags.h"
#include "SDK/ears_common/include/ears_common/Guid.h"
#include "SDK/ears_common/include/ears_common/String.h"

// SDK (Framework)
#include "SDK/ears_framework/src/framework/core/base/Base.h"
#include "SDK/ears_framework/src/game_framework/framework/core/eventhandler/CEventHandler.h"

// CPP
#include <stdint.h>

namespace EARS
{
	namespace Modules
	{
		enum class CityFlags : uint32_t
		{
			CityFlags_REF = 0xFFFFFFFF,
			CITY_IS_HIDDEN_FROM_PLAYER = 0x1,
		};

		class City : public EARS::Framework::Base
		{
		public:

			/**
			 * Is the City Known to the Player?
			 * Meaning will it show up in menus
			 * (Implemented using engine code)
			 */
			bool IsKnownToPlayer() const;

			/**
			 * Should we hide the City away from the Player?
			 * Meaning the City will not show up in menus
			 * (Implemented using engine code)
			 */
			void HideFromPlayer();

			/**
			 * Should we reveal the City to the Player?
			 * Meaning the city will show up in menus
			 * (Implemented using engine code)
			 */
			void RevealToPlayer();

			/**
			 * Teleport to this City.
			 * (Implemented using engine code)
			 */
			void RequestTeleport() const;

			/**
			 * Fetch the Display Name of the City.
			 * Will be cached on first call.
			 * Consecutive calls will use cached value.
			 */
			String* GetDisplayName();

			// Getters
			uint32_t GetCityID() const { return m_CityID; }
			const char* GetInternalName() const { return m_InternalName.c_str(); }

		private:
			
			uint32_t m_CityID = 0;
			uint32_t m_DisplayNameHashID = 0;
			Flags32 m_Flags;
			String m_InternalName;
			String m_DisplayName;
			mutable RWS::CEventId m_TeleportMsg;
			EARS::Common::guid128_t m_WorldGraphPartitionGuidMap;
			Array<void*> m_Buildings;
			float m_MapMinPosX = 0.0f;
			float m_MapMaxPosX = 0.0f;
			float m_MapMinPosZ = 0.0f;
			float m_MapMaxPosZ = 0.0f;
			float m_MapInitalPosX = 0.0f;
			float m_MapInitalPosZ = 0.0f;
		};
	} // Modules
} // EARS
