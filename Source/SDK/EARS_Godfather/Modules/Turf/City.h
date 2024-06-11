#pragma once

// SDK
#include "SDK/EARS_Common/Array.h"
#include "SDK/EARS_Common/Bitflags.h"
#include "SDK/EARS_Common/Guid.h"
#include "SDK/EARS_Common/String.h"
#include "SDK/EARS_Framework/Game_Framework/Core/EventHandler/CEventHandler.h"

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

		class City
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

			// Getters
			uint32_t GetCityID() const { return m_CityID; }
			String GetInternalName() const { return m_InternalName; }

		private:

			char m_Padding9[0x50]; // assuming padding is 0x50
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
