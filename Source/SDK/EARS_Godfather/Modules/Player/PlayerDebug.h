#pragma once

// SDK
#include "SDK/EARS_Common/Bitflags.h"
#include "SDK/EARS_Common/Singleton.h"
#include "SDK/EARS_Framework/Core/EventHandler/CEventHandler.h"

namespace EARS 
{
	namespace Modules 
	{
		/**
		 * A reimplementation of the debug options system found in the Xbox debug version of the game
		 * This doesn't exist in the PC version, so this 
		 */
		class PlayerDebugOptions : public Singleton<PlayerDebugOptions>, RWS::CEventHandler
		{
		public:

			PlayerDebugOptions();
			virtual ~PlayerDebugOptions();

			void SetIsInDebugFly(const bool bInDebugFly);
			bool IsInDebugFly() const;

			static PlayerDebugOptions* GetInstance();

		private:

			enum class DebugFlyOptions : int32_t
			{
				DEBUGFLY_OFF = 0x0,
				DEBUGFLY_COLLIDE = 0x1,
				DEBUGFLY_NOCOLLIDE = 0x2,
				// DEBUGFLY_COLLIDE_2NDCONTROLLER = 0x3, <- won't be implemented
			};

			DebugFlyOptions m_DebugFlyOptions = DebugFlyOptions::DEBUGFLY_OFF;

			Flags32 m_Flags; // <- xbox says 0x2080 for offset
		};
	}
}
