#pragma once

// CPP
#include <cstdint>

namespace EARS
{
	namespace Framework
	{
		/**
		 * Stores all Players in a game session
		 * PlayerID zero is to be assumed to be local
		 */
		class PlayerManager
		{
		public:

			/**
			 * Fetch the Player associated with an ID.
			 * @param PlayerID - ID of the Player
			 * @return BasePlayer - The registered Player, associated with the ID.
			 */
			class BasePlayer* GetPlayer(const uint32_t PlayerID) const;

			/**
			 * Fetch the instance of this from the ingame address
			 * @return PlayerManager - The Singleton of the PlayerManager
			 */
			static PlayerManager* GetInstance();

		private:

			void* VTABLE = nullptr; // Singleton VTABLE
			class BasePlayer** m_PlayerArray = nullptr;
			uint32_t m_MaxPlayers = 0;
			uint32_t m_NumPlayers = 0;
		};
	} // Framework
} // EARS
