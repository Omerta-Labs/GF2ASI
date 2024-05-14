#pragma once

// CPP
#include <cstdint>

// TODO: Move elsewhere when we find the code file for it
struct guid128_t
{
	uint32_t a, b, c, d = 0;
};

namespace EARS
{
	namespace Modules
	{
		/**
		 * The Player class for The Godfather II
		 */
		class Player
		{
		public:

			/**
			 * Get the Local Player
			 * *NOT PART OF MAIN GAME, UTILITY FUNCTION*
			 * @return Player - The Local Player
			 */
			static Player* GetLocalPlayer();

		private:

			void* VTABLE = nullptr; // Player vtable
			char m_Padding[0x248C];
			uint32_t m_CurrentModelType = 0;		// 0x2490 - 0x2494
			uint32_t m_PlayerID = 0;				// 0x2494 - 0x2498
			uint32_t m_ControllerID = 0;			// 0x2498 - 0x249C
			uint32_t m_PlayerFlags[3];				// 0x249C - 0x24A8
			char m_Padding2[0x460];
			guid128_t m_ModelGUID;
			uint32_t m_ModelStreamHandle = 0;
			guid128_t m_PlayerRCB_GUID;
			guid128_t m_FatherModelGUID;
			guid128_t m_FatherRCB_GUID;
			int32_t m_PlayerDisableCount = 0;

		};
	} // Modules
} // EARS
